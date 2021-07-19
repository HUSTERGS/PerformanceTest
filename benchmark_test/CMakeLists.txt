cmake_minimum_required(VERSION 3.14)

project(learn_bench)

# viper need cxx 17 to compile
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out )


# add_subdirectory(concurrentqueue)

# add_subdirectory(viper)
# include_directories(${viper_SOURCE_DIR}/include)

# add_executable(viper_test viper_test.cc)
# target_link_libraries(viper_test gtest gtest_main pmem viper)

add_executable(learn_bench benchmark_bench.cc)
target_link_libraries(learn_bench benchmark)
