#include <immintrin.h>

#ifdef __cplusplus
extern "C"
{
#endif

// C header here
#include "clht.h"

#ifdef __cplusplus
}
#endif


#include <string>
#include <gtest/gtest.h>


#define TEST_NUM 10000

TEST(a, b) {
    // creat a new CLHT instance
    clht_t *hash_table = clht_create(128);
    clht_gc_thread_init(hash_table, 1);
    for (uint64_t a = 0; a < TEST_NUM; a++) {
        clht_put(hash_table, a, 1);
    }
    for (uint64_t a = 0; a < TEST_NUM; a++) {
        EXPECT_EQ(clht_get(hash_table->ht, a), 1);
    }
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
