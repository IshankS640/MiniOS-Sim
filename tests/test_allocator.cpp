#include "minios/mem/allocator.hpp"
#include "minios/check.hpp"
#include <iostream>

int main() {
    minios::mem::Allocator a(100, minios::mem::Strategy::FirstFit);
    size_t p1 = a.allocate(30);
    size_t p2 = a.allocate(20);
    CHECK(p1 == 0);
    CHECK(p2 == 30);
    a.free(p1);
    size_t p3 = a.allocate(10); // should reuse the freed 30-byte block (first-fit)
    CHECK(p3 == 0);
    std::cout << "p1=" << p1 << " p2=" << p2 << " p3=" << p3 << "\n";
    std::cout << "test_allocator: OK\n";
    return 0;
}
