cmake_minimum_required(VERSION 3.14)

project(viper_test)

# viper need cxx 17 to compile
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out )

#add_subdirectory(CCEH)
include_directories(CCEH)


add_executable(cceh_mem_test cceh_mem_test.cc CCEH/src/CCEH_MSB.cpp)

target_link_libraries(cceh_mem_test gtest gtest_main)
