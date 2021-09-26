#include <gtest/gtest.h>
#include "src/hash.h"
#include "src/CCEH.h"

TEST(a, b) {
    const size_t initialTableSize = 16*1024;
    Hash* table = new CCEH(initialTableSize/Segment::kNumSlot);
    CCEH
}