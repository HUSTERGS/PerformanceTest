#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <string.h>
#include <libpmemlog.h>
#include <iostream>
#include <benchmark/benchmark.h>
#include <thread>
#include <numeric>
/* size of the pmemlog pool -- 1 GB */
#define POOL_SIZE ((size_t)(1 << 30))

const char path[] = "/mnt/AEP1/gs/logfile";


const uint64_t key_begin_size = 8;
const uint64_t key_end_size = 256;
const uint64_t value_begin_size = 8;
const uint64_t value_end_size = 256;
const uint64_t nums = 1024 * 1024;
const uint64_t thread_end = 32;
/*
 * printit -- log processing callback for use with pmemlog_walk()
 */
static int
printit(const void *buf, size_t len, void *arg)
{
    fwrite(buf, len, 1, stdout);
    return 0;
}

int single_thread_append(benchmark::State &state, uint64_t key_size, uint64_t value_size, int nums, PMEMlogpool *plp)
{
    /* create the pmemlog pool or open it if it already exists */
    state.PauseTiming();
    // simple append the same data
    std::string buf(key_size + value_size, '1');

    state.ResumeTiming();

    for (; nums > 0; nums--)
    {
        if (pmemlog_append(plp, buf.data(), buf.size()) < 0)
        {
            perror("pmemlog_append");
            exit(1);
        }
    }
    return 0;
}

int multi_thread_test(uint64_t key_size, uint64_t value_size, int nums, PMEMlogpool *plp)
{
    /* create the pmemlog pool or open it if it already exists */

    std::string buf(key_size + value_size, '1');

    for (; nums > 0; nums--)
    {
        if (pmemlog_append(plp, buf.data(), buf.size()) < 0)
        {
            perror("pmemlog_append");
            exit(1);
        }
    }
    return 0;
}

static void BM_SingleThread(benchmark::State &state)
{
    // Perform setup here

    PMEMlogpool *plp;

    /* create the pmemlog pool or open it if it already exists */
    plp = pmemlog_create(path, POOL_SIZE, 0666);

    if (plp == NULL)
        plp = pmemlog_open(path);

    if (plp == NULL)
    {
        perror(path);
        exit(1);
    }
    auto key_size = state.range(0);
    auto value_size = state.range(1);
    auto nums = state.range(2);
    for (auto _ : state)
    {
        state.PauseTiming();
        pmemlog_rewind(plp);
        state.ResumeTiming();
        
        single_thread_append(state, key_size, value_size, nums, plp);
    }

    state.SetBytesProcessed((key_size + value_size) * nums * state.iterations());
    pmemlog_close(plp);

}

static void BM_MultiThread(benchmark::State &state)
{
    // Perform setup here
    static PMEMlogpool *plp;

    if (state.thread_index == 0)
    {
        /* create the pmemlog pool or open it if it already exists */
        plp = pmemlog_create(path, POOL_SIZE, 0666);

        if (plp == NULL)
            plp = pmemlog_open(path);

        if (plp == NULL)
        {
            perror(path);
            exit(1);
        }
        pmemlog_rewind(plp);
    }

    auto key_size = state.range(0);
    auto value_size = state.range(1);
    auto nums = state.range(2);
    for (auto _ : state)
    {
        single_thread_append(state, key_size, value_size, nums / state.threads, plp);
    }
    //
    if (state.thread_index == 0)
    {
        // std::cout << "current offset: " << pmemlog_tell(plp) << "current threads: " << state.threads << std::endl;
        pmemlog_close(plp);
    }

    // data processed by each thread
    state.SetBytesProcessed((key_size + value_size) * nums / state.threads);
}


// each thread write to seperate log file
static void BM_MultiThread_Sep(benchmark::State &state)
{
    // Perform setup here
    PMEMlogpool *plp;

    std::string thread_log(path);
    thread_log += std::to_string(state.thread_index);

    plp = pmemlog_create(thread_log.c_str(), POOL_SIZE, 0666);

        if (plp == NULL)
            plp = pmemlog_open(thread_log.c_str());

        if (plp == NULL)
        {
            perror(thread_log.c_str());
            exit(1);
        }
        pmemlog_rewind(plp);


    auto key_size = state.range(0);
    auto value_size = state.range(1);
    auto nums = state.range(2);

    for (auto _ : state)
    {
        state.PauseTiming();
        pmemlog_rewind(plp);
        state.ResumeTiming();
        single_thread_append(state, key_size, value_size, nums / state.threads, plp);
    }
    //
    pmemlog_close(plp);
    state.SetBytesProcessed((key_size + value_size) * nums * state.iterations() / state.threads);
}

// Register the function as a benchmark
// BENCHMARK(BM_SingleThread)
//     ->Iterations(1)
//     ->UseRealTime()
//     ->ArgsProduct({
//         // key_size, from 8 bytes to 256 bytes
//         benchmark::CreateRange(key_begin_size, key_end_size, 2),
//         // value_size, from 8 bytes to 256 bytes
//         benchmark::CreateRange(value_begin_size, value_end_size, 2),
//         // nums always 256 bytes
//         benchmark::CreateRange(nums, nums, 1)
//     })
//     ;
BENCHMARK(BM_MultiThread)
    ->Iterations(1)
    // {key_size, value_size, total_nums}
    ->Args({128, 128, 1 << 20})
    ->ThreadRange(1, thread_end)
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond)
    ;


BENCHMARK(BM_MultiThread_Sep)
    ->Iterations(1)
    ->Args({128, 128, (1 << 20) * 2})
    ->ThreadRange(1, thread_end)
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond)
    ;
// Run the benchmark

BENCHMARK_MAIN();

