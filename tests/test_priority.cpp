#include "minios/sched/priority.hpp"
#include "minios/check.hpp"
#include <iostream>

int main() {
    minios::sched::PriorityScheduler s;
    s.set_base_priority(1, 20);
    s.set_base_priority(2, 40); // higher priority
    s.on_arrival(1);
    s.on_arrival(2);

    int first = s.pick_next(0);
    CHECK(first == 2); // higher priority runs first
    std::cout << "picked pid=" << first << " (expected 2)\n";
    std::cout << "test_priority: OK\n";
    return 0;
}
