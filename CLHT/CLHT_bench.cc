#include <immintrin.h>

#ifdef __cplusplus
extern "C"
{
#endif

// C header here
#include <immintrin.h>
#include "clht.h"

#ifdef __cplusplus
}
#endif


#include <iostream>
#include <benchmark/benchmark.h>

static void BM_Multithread(benchmark::State &states) {
    

     static clht_t *hash_table;
     if (states.thread_index == 0) {
         hash_table = clht_create(123);
     }
     // hash_table = clht_create(123);
     clht_gc_thread_init(hash_table, states.thread_index);

     for (auto _ : states) {
         for (uint64_t a = 0; a < 10000; a++) {
             clht_put(hash_table, 10000 * states.thread_index + a, 10000 * states.thread_index + a);
             if (100 * states.thread_index + a == 0) {
                 std::cout << "llulala" << std::endl;
                 std::cout << clht_get(hash_table->ht, 0) << std::endl;
             }
         }
     }
    
     if (states.thread_index == 0) {
         for (uint64_t a = 0; a < 10000 * states.threads; a++) {
             if (clht_get(hash_table->ht, a) != a) {
                 std::cout << "fail" << a << std::endl;
                 // break;
             }
         }
     }
}

BENCHMARK(BM_Multithread)
->ThreadRange(1, 8)
->Unit(benchmark::kMillisecond)
->Iterations(1);

BENCHMARK_MAIN();
