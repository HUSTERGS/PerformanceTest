#include <iostream>
#include <string>
#include <cstring>
#include <string_view>
struct A {
    A() {
        a = 10; 
    }
    thread_local static int a;
};

A a1;
thread_local int A::a;
int main(void) {
    char * buf = new char[sizeof(std::string)];
    std::string *p = new (buf) std::string("hi");
    std::cout << *p << std::endl;
    std::string *q = new (buf) std::string("hi too");
    std::cout << *p << std::endl;
    std::cout << *p << std::endl;
    // a1.a = 1;
    // std::cout << a1.a << std::endl;
}