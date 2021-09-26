cmake_minimum_required(VERSION 3.14)

project(libpmemlog)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out )


add_executable(libpmemlog_test log_test.cc)
target_link_libraries(libpmemlog_test gtest gtest_main pmemlog)

add_executable(libpmemlog_bench log_bench.cc)
target_link_libraries(libpmemlog_bench pmemlog benchmark)

# find_library(LIBPMEMLOG pmemlog HINTS /usr/lib/x86_64-linux-gnu)
