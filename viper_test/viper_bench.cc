#include <benchmark/benchmark.h>
#include "viper/viper.hpp"
#include <filesystem>

typedef viper::Viper<std::string, std::string>::Client VCType;
typedef std::unique_ptr<viper::Viper<std::string, std::string>> VDBType;

int single_thread_append(benchmark::State &state, uint64_t key_size, uint64_t value_size, int nums, VCType &v_client)
{
    /* create the pmemlog pool or open it if it already exists */
    state.PauseTiming();

    std::string key(key_size, '1');
    std::string value(value_size, '1');

    state.ResumeTiming();

    for (; nums > 0; nums--)
    {
        v_client.put(key, value);
    }
    return 0;
}

static void BM_SingleThread(benchmark::State &state)
{
    const size_t initial_size = 1 << 30; // 1 GiB
    auto viper_db = viper::Viper<std::string, std::string>::create("/pmem-fs/viper", initial_size);
    // To modify records in Viper, you need to use a Viper Client.
    auto v_client = viper_db->get_client();

    for (auto _ : state)
    {
        single_thread_append(state, state.range(0), state.range(0), 100, v_client);
        state.PauseTiming();
        std::filesystem::remove_all("/pmem-fs/viper");
        state.ResumeTiming();
    }
}


static void BM_MultiThread(benchmark::State &state)
{
    const size_t initial_size = 1 << 31;
    static VDBType viper_db;
    if (state.thread_index == 0)
    {
        viper_db = viper::Viper<std::string, std::string>::create("/pmem-fs/viper", initial_size);
    }

    for (auto _ : state)
    {
        // single_thread_append(state, state.range(0), state.range(0), 1024 / state.threads, *v_client_ptr);
        state.PauseTiming();
        auto nums = 1;
        std::string key(100, '1');
        std::string value(100, '1');

        state.ResumeTiming();
        VCType vc_client = viper_db->get_client();
        for (; nums > 0; nums--)
        {
            vc_client.put(key, value);
        }
    }

    if (state.thread_index == 0)
    {
        std::filesystem::remove_all("/pmem-fs/viper");
    }
}

// BENCHMARK(BM_SingleThread)
//     ->Iterations(1)
//     ->RangeMultiplier(2)
//     ->Range(8, 2 << 10)
//     ;

BENCHMARK(BM_MultiThread)
    ->Iterations(1)
    ->Threads(2)
    // ->Threads(4)
    // ->ThreadRange(1, 16)
    // ->UseRealTime()
    // ->Unit(benchmark::kMillisecond)
    ;

BENCHMARK_MAIN();
