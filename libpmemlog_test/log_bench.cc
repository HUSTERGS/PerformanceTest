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

    std::string key(key_size, '1');
    std::string value(value_size, '1');
    std::string buf;
    buf.append(reinterpret_cast<char *>(&key_size));
    buf.append(reinterpret_cast<char *>(&value_size));
    buf.append(key);
    buf.append(value);

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

    std::string key(key_size, '1');
    std::string value(value_size, '1');
    std::string buf;
    buf.append(reinterpret_cast<char *>(&key_size));
    buf.append(reinterpret_cast<char *>(&value_size));
    buf.append(key);
    buf.append(value);

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

    const char path[] = "/pmem-fs/myfile";
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

    for (auto _ : state)
    {
        state.PauseTiming();
        pmemlog_rewind(plp);
        state.ResumeTiming();
        single_thread_append(state, state.range(0), state.range(0), 500, plp);
    }
    pmemlog_close(plp);
}

static void BM_MultiThread(benchmark::State &state)
{
    // Perform setup here
    static PMEMlogpool *plp;
    const char path[] = "/pmem-fs/myfile";

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

    for (auto _ : state)
    {
        single_thread_append(state, 64, 64, 1024 / state.threads, plp);
    }
    //
    if (state.thread_index == 0)
    {
        // std::cout << "current offset: " << pmemlog_tell(plp) << "current threads: " << state.threads << std::endl;
        pmemlog_close(plp);
    }
}

// Register the function as a benchmark
BENCHMARK(BM_SingleThread)
    ->Iterations(100)
    ->RangeMultiplier(2)
    ->Range(8, 2 << 10);

BENCHMARK(BM_MultiThread)
    ->Iterations(1)
    ->ThreadRange(1, 16)
    ->UseRealTime()
    // ->Unit(benchmark::kMillisecond)
    ;

// Run the benchmark

BENCHMARK_MAIN();

