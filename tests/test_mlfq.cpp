#include "minios/sched/mlfq.hpp"
#include "minios/check.hpp"
#include <iostream>

int main() {
    minios::sched::MLFQ m;
    m.set_nice(1, 0);
    m.set_nice(2, 5); // higher nice -> should end up with lower priority

    m.on_arrival(1);
    m.on_arrival(2);

    int first = m.pick_next(0);
    // At tick 0, both have recent_cpu=0, so priority = 63 - 0 - nice*2.
    // pid 1 (nice 0) -> 63; pid 2 (nice 5) -> 53. pid 1 should win.
    CHECK(first == 1);
    std::cout << "MLFQ picked pid=" << first << " (expected 1, lower nice wins)\n";
    std::cout << "test_mlfq: OK\n";
    return 0;
}
