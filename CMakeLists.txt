cmake_minimum_required(VERSION 3.14)

project(Performance_Test)
set(CMAKE_CXX_STANDARD 14)
# set all binary to build/bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

# loading gtest
add_subdirectory(gtest)
include_directories(${gtest_SOURCE_DIR}/include/ ${gtest_SOURCE_DIR})

#loading benchmark
add_subdirectory(benchmark)
include_directories(${benchmark_SOURCE_DIR}/include/)

# add sub test project
add_subdirectory(libpmemlog_test)

add_subdirectory(viper_test)

## script to print all variables
# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()