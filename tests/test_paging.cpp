#include "minios/mem/paging.hpp"
#include "minios/check.hpp"
#include <iostream>
#include <vector>

int main() {
    std::vector<int> refs = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};

    int f = minios::mem::fifo_faults(refs, 3);
    int l = minios::mem::lru_faults(refs, 3);
    int o = minios::mem::opt_faults(refs, 3);

    std::cout << "FIFO=" << f << " (expected 15)\n";
    std::cout << "LRU=" << l << " (expected 12)\n";
    std::cout << "OPT=" << o << " (expected 9)\n";

    CHECK(f == 15);
    CHECK(l == 12);
    CHECK(o == 9);

    // Belady's anomaly check
    std::vector<int> belady = {1,2,3,4,1,2,5,1,2,3,4,5};
    int f3 = minios::mem::fifo_faults(belady, 3);
    int f4 = minios::mem::fifo_faults(belady, 4);
    std::cout << "Belady FIFO frames=3: " << f3 << " (expected 9)\n";
    std::cout << "Belady FIFO frames=4: " << f4 << " (expected 10, MORE frames but MORE faults!)\n";
    CHECK(f3 == 9);
    CHECK(f4 == 10);

    std::cout << "test_paging: OK\n";
    return 0;
}
