# PerformanceTest

## Compile and install the `Benchmark`
```sh
# Check out the library.
$ git clone https://github.com/google/benchmark.git
# Benchmark requires Google Test as a dependency. Add the source tree as a subdirectory.
$ git clone https://github.com/google/googletest.git benchmark/googletest
# Go to the library root directory
$ cd benchmark
# Make a build directory to place the build output.
$ cmake -E make_directory "build"
# Generate build system files with cmake.
# Customize the GOOGLETEST_PATH
$ cmake -E chdir "build" cmake -DCMAKE_BUILD_TYPE=Release ../ --DGOOGLETEST_PATH=../../gtest

```