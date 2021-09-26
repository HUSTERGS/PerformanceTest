cmake_minimum_required(VERSION 3.14)

project(viper_test)

# viper need cxx 17 to compile
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out )


add_subdirectory(concurrentqueue)
include_directories(${concurrentqueue_SOURCE_DIR})

add_subdirectory(viper)
include_directories(${viper_SOURCE_DIR}/include)

add_executable(viper_test viper_test.cc)
target_link_libraries(viper_test gtest gtest_main pmem viper)

add_executable(viper_bench viper_bench.cc)
target_link_libraries(viper_bench benchmark pmem viper)
