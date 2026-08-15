#include "minios/process.hpp"
#include "minios/check.hpp"
#include <iostream>

int main() {
    auto procs = minios::load_workload("workloads/basic.txt");
    CHECK(procs.size() == 3);
    CHECK(procs[0].pid == 1);
    CHECK(procs[0].arrival == 0);
    CHECK(procs[1].pid == 2);
    CHECK(procs[2].phases[0].kind == minios::PhaseKind::CPU);
    CHECK(procs[2].phases[0].arg == 4);
    std::cout << "test_workload: OK\n";
    return 0;
}
