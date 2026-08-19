#include "minios/sched/sleep_queue.hpp"
#include "minios/check.hpp"
#include <iostream>

int main() {
    minios::sched::SleepQueue sq;
    sq.sleep(1, /*now=*/0, /*ticks=*/5);   // wakes at tick 5
    sq.sleep(2, /*now=*/0, /*ticks=*/0);   // edge case: wakes immediately
    sq.sleep(3, /*now=*/0, /*ticks=*/-2);  // edge case: negative -> immediate

    auto at0 = sq.wake_ready(0);
    CHECK(at0.size() == 2); // pids 2 and 3 wake immediately
    std::cout << "woken at tick 0: " << at0.size() << " (expected 2)\n";

    auto at4 = sq.wake_ready(4);
    CHECK(at4.empty()); // pid 1 not ready yet

    auto at5 = sq.wake_ready(5);
    CHECK(at5.size() == 1);
    CHECK(at5[0] == 1);
    std::cout << "woken at tick 5: pid " << at5[0] << " (expected 1)\n";

    std::cout << "test_sleep_queue: OK\n";
    return 0;
}
