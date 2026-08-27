#include "minios/workload_gen.hpp"
#include "minios/compare.hpp"
#include "minios/mem/paging.hpp"
#include "minios/dead/bankers.hpp"
#include <iostream>
#include <string>
#include <vector>

void print_usage() {
    std::cout << "Usage:\n"
              << "  osim compare               (run scheduler comparison table)\n"
              << "  osim page <frames>         (run built-in paging demo with N frames)\n"
              << "  osim bank                  (run built-in Banker's demo)\n";
}

int main(int argc, char** argv) {
    if (argc < 2) { print_usage(); return 1; }
    std::string cmd = argv[1];

    if (cmd == "compare") {
        auto workload = minios::generate_random_workload(6, 42);
        minios::run_comparison(workload);
    } else if (cmd == "page") {
        int frames = (argc >= 3) ? std::stoi(argv[2]) : 3;
        std::vector<int> refs = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};
        std::cout << "FIFO faults: " << minios::mem::fifo_faults(refs, frames) << "\n";
        std::cout << "LRU  faults: " << minios::mem::lru_faults(refs, frames) << "\n";
        std::cout << "OPT  faults: " << minios::mem::opt_faults(refs, frames) << "\n";
    } else if (cmd == "bank") {
        std::vector<int> available = {3,3,2};
        std::vector<std::vector<int>> allocation = {
            {0,1,0}, {2,0,0}, {3,0,2}, {2,1,1}, {0,0,2}
        };
        std::vector<std::vector<int>> max_demand = {
            {7,5,3}, {3,2,2}, {9,0,2}, {2,2,2}, {4,3,3}
        };
        auto r = minios::dead::check_safety(available, allocation, max_demand);
        std::cout << "safe=" << r.safe << ", sequence: ";
        for (int p : r.safe_sequence) std::cout << "P" << p << " ";
        std::cout << "\n";
    } else {
        print_usage();
        return 1;
    }
    return 0;
}
