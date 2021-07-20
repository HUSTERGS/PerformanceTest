#include <benchmark/benchmark.h>
#include "viper/viper.hpp"
#include <filesystem>
#include <mutex>
#include "random_string.h"

typedef viper::Viper<std::string, std::string>::Client VCType;
typedef std::unique_ptr<viper::Viper<std::string, std::string>> VDBType;

const uint64_t key_begin_size = 8;
const uint64_t key_end_size = 256;
const uint64_t value_begin_size = 8;
const uint64_t value_end_size = 256;
const uint64_t nums = 1024;
const uint64_t thread_end = 32;
std::string viper_path = "/mnt/AEP1/gs/viper";

int single_thread_append(benchmark::State &state, uint64_t key_size, uint64_t value_size, int nums, VCType &v_client)
{
    /* create the pmemlog pool or open it if it already exists */
    state.PauseTiming();

    std::string key = gen_random(key_size);
    std::string value = gen_random(value_size);
    
    state.ResumeTiming();

    for (; nums > 0; nums--)
    {
        v_client.put(key, value);
    }
    return 0;
}

static void BM_SingleThread(benchmark::State &state)
{
    std::filesystem::remove_all(viper_path);

    const size_t initial_size = 1 << 30; // 1 GiB
    auto viper_db = viper::Viper<std::string, std::string>::create(viper_path, initial_size);
    // To modify records in Viper, you need to use a Viper Client.
    auto v_client = viper_db->get_client();

    auto key_size = state.range(0);
    auto value_size = state.range(1);
    auto nums = state.range(2);

    for (auto _ : state)
    {
        single_thread_append(state, key_size, value_size, nums, v_client);
        state.PauseTiming();
        std::filesystem::remove_all(viper_path);
        state.ResumeTiming();
    }
    state.SetBytesProcessed((key_size + value_size) * nums * state.iterations());
}



static void BM_MultiThread(benchmark::State &state)
{
    const size_t initial_size = 1 << 30;
    static VDBType viper_db;
    if (state.thread_index == 0)
    {
        viper_db = viper::Viper<std::string, std::string>::create(viper_path, initial_size);
    }

    auto key_size = state.range(0);
    auto value_size = state.range(1);
    auto nums = state.range(2) / state.threads;

    for (auto _ : state)
    {
        // single_thread_append(state, state.range(0), state.range(0), 1024 / state.threads, *v_client_ptr);
        state.PauseTiming();
        std::string key = gen_random(key_size);
        std::string value = gen_random(value_size);

        state.ResumeTiming();
        VCType vc_client = viper_db->get_client();
        for (int count = 0; count < nums; count++)
        {
            vc_client.put(key, value);
        }
    }

    if (state.thread_index == 0)
    {
        std::filesystem::remove_all(viper_path);
    }
    
    // data processed by each thread
    state.SetBytesProcessed((key_size + value_size) * nums);
}

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
    // {key_size, value_size, total_num}
    ->Args({256, 256, 1024})
    // Use real time to calculate `bytes_per_second`
    ->UseRealTime()
    ->ThreadRange(1, 64)
    ->Unit(benchmark::kMillisecond)
    ;

BENCHMARK_MAIN();
