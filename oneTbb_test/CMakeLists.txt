cmake_minimum_required(VERSION 3.14)

project(oneTbb_test)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out )

find_package(TBB REQUIRED)

add_executable(ontTbb_test tbb_test.cc)
target_link_libraries(ontTbb_test gtest gtest_main TBB::tbb)

# add_executable(libpmemlog_bench log_bench.cc)
# target_link_libraries(libpmemlog_bench pmemlog benchmark)

# find_library(LIBPMEMLOG pmemlog HINTS /usr/lib/x86_64-linux-gnu)
