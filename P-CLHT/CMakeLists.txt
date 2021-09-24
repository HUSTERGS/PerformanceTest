cmake_minimum_required(VERSION 3.14)

project(P-CLHT)


add_subdirectory(P-CLHT)

include_directories(P-CLHT/include)

add_executable(pclht-test pclht.cc)

target_link_libraries(pclht-test gtest gtest_main pclht)

# find_library(LIBPMEMLOG pmemlog HINTS /usr/lib/x86_64-linux-gnu)
