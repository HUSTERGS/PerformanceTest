cmake_minimum_required(VERSION 3.14)

project(CLHT)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out )

include_directories(CLHT_test)

link_directories(${PROJECT_SOURCE_DIR})

find_library(libclht clht ${PROJECT_SOURCE_DIR})
find_library(libssmem ssmem ${PROJECT_SOURCE_DIR})

add_executable(CLHT_test CLHT_test.cc)
target_link_libraries(CLHT_test gtest gtest_main clht ssmem)

add_executable(CLHT_bench CLHT_bench.cc)
target_link_libraries(CLHT_bench pmemlog benchmark rt pthread m clht ssmem)

# find_library(LIBPMEMLOG pmemlog HINTS /usr/lib/x86_64-linux-gnu)
