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


#include <string>
#include <gtest/gtest.h>


TEST(a, b) {
    // creat a new CLHT instance
    clht_t *hash_table = clht_create(128);
    clht_gc_thread_init(hash_table, 1);
    clht_gc_thread_init(hash_table, 1);
    // for (uint64_t a = 0; a < 2; a++) {
    //   clht_put(hash_table, a, 1);
    // }
    // for (uint64_t a = 0; a < 2; a++) {
    //   EXPECT_EQ(clht_get(hash_table->ht, a), 1);
    // }
    std::cout << clht_put(hash_table, 1, 1) << std::endl;
    std::cout << clht_put(hash_table, 1, 2) << std::endl;
    std::cout << clht_get(hash_table->ht, 1) << std::endl;

}


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
