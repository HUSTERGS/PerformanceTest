#include <benchmark/benchmark.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <numeric>
#include <iostream>
std::mutex m;

void t() {
    
    std::lock_guard<std::mutex> guard(m);
    // usleep(1);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

static void BM_Test(benchmark::State &state) {
    for (auto _ : state) {
        // t();
        std::lock_guard<std::mutex> guard(m);
        // usleep(1);
        std::this_thread::sleep_for(std::chrono::seconds(state.thread_index + 1));
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    // std::cout << "iterations: " << state.total_iterations_ << std::endl;
    // assert(state.threads != state.iterations());
    // state.SetBytesProcessed((state.thread_index + 1) * state.threads);
    state.SetBytesProcessed(100 / state.threads);
    // state.counters["time_sleeped"] = benchmark::Counter(1 * state.threads, benchmark::Counter::kIsRate);
}

// BENCHMARK(BM_Test)
//     ->Iterations(1)
//     // ->ThreadRange(1, 16)
//     ->UseRealTime()
//     ->Unit(benchmark::kMillisecond)
//     ;
BENCHMARK(BM_Test)
    ->Iterations(1)
    ->ThreadRange(1, 16)
    ->UseRealTime()
    ->Unit(benchmark::kSecond)
    ;


// void BM_spin_empty(benchmark::State& state) {
//   for (auto _ : state) {
//     for (int x = 0; x < state.range(0); ++x) {
//       benchmark::DoNotOptimize(x);
//     }
//   }
// }

// BENCHMARK(BM_spin_empty)
//   ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
//       for (auto e : v) {
//             std::cout << "lalala " << e << std::endl;
//         }
//         std::cout << "length of vector: " << v.size() << std::endl;
//     // return *(std::max_element(std::begin(v), std::end(v)));
//     return std::accumulate(v.begin(), v.end(), 0);
//   })
//   ->Arg(512);

BENCHMARK_MAIN();