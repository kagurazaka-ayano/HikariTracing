#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "Metal.hpp"
#include <iostream>

int main() {
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();
    std::cout << "hello world!";
    pool->release();
    return 0;
}