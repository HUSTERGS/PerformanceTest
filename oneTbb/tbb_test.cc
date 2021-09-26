#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <map>
#include <array>
#include <functional>

#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
#include <tbb/concurrent_vector.h>


using namespace tbb;

std::vector<int> *Foo(int i)  {
    auto a = new std::vector<int>;
    for (int c = 0; c < i; c++) {
        a->push_back(i);
    }
    return a;
}

void pri(std::vector<int> & a) {
    for (auto &i : a) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}


class SumFoo {
    std::vector<int> * origin;
public:
    std::vector<int> local_kv;
    void operator()( const blocked_range<size_t>& r ) {
        

        size_t end = r.end();
        for( size_t i=r.begin(); i!=end; ++i) {
            auto new_kv = Foo(origin->at(i));
            // pri(*new_kv);
            // std::cout << "i = " << i << std::endl;
            local_kv.insert(local_kv.end(), new_kv->begin(), new_kv->end());
            pri(local_kv);
        }
    }


    SumFoo( SumFoo& x, split ) : origin(x.origin) {
        std::cerr << "split called " << std::endl;
    }


    void join( const SumFoo& y ) {
        std::cout << "join called\norigin:\n" << std::endl;
        local_kv.insert(local_kv.end(), y.local_kv.begin(), y.local_kv.end());
    }


    SumFoo(std::vector<int> * a ) :
        origin(a)
    {}
};

std::vector<int> ParallelSumFoo(std::vector<int> &data, size_t n ) {
    SumFoo sf(&data);
    parallel_reduce( blocked_range<size_t>(0,n), sf);
    return sf.local_kv;
}


int main()
{
    std::vector<int> a;
    for (auto i = 1; i < 100; i++) {
        a.push_back(i);
    }
    
    auto b = ParallelSumFoo(a, a.size());
    pri(b);
}