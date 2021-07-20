# PerformanceTest

```sh



git clone --recurse-submodules git@github.com:HUSTERGS/PerformanceTest.git
mkdir build
cd build

# viper need gcc/g++10 to correctly run (std::filesystem)
export CC=/usr/bin/gcc-10
export CXX=/usr/bin/g++-10
export CPP=/usr/bin/gcc-10

cmake .. -DCMAKE_BUILD_TYPE=Release
cd out/

# ./viper_bench
# ./libpmemlog_bench
```