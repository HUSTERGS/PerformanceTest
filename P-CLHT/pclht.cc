#include <gtest/gtest.h>
#include <immintrin.h>
#include "clht_lb_res.h"





TEST(a, b) {
    clht_t *hashtable = clht_create(512, 1024, "abc");
    clht_gc_thread_init(hashtable, 0);

    clht_put(hashtable, 0, 0);
    clht_get(hashtable, 0);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
