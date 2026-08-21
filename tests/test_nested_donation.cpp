#include "minios/sched/lock.hpp"
#include "minios/sched/donation.hpp"
#include "minios/check.hpp"
#include <iostream>
#include <unordered_map>

int main() {
    using namespace minios::sched;
    DonationTracker dt;
    LockTable locks;

    // Setup: L(pid 10, prio 10) holds lock A.
    //        M(pid 20, prio 20) holds lock B, and waits on lock A.
    //        H(pid 30, prio 30) waits on lock B.
    dt.set_base(10, 10);
    dt.set_base(20, 20);
    dt.set_base(30, 30);

    locks.try_acquire(100, 10); // lock A -> L
    dt.add_held_lock(10, 100);

    locks.try_acquire(200, 20); // lock B -> M
    dt.add_held_lock(20, 200);

    // M tries lock A, blocked by L
    locks.add_waiter(100, 20, dt.base(20));
    // H tries lock B, blocked by M
    locks.add_waiter(200, 30, dt.base(30));

    // Small lookup tables so propagate_donation can walk the chain:
    std::unordered_map<int,int> owner_of = {{100, 10}, {200, 20}};
    std::unordered_map<int,int> waiting_lock_of = {{20, 100}}; // M is waiting on lock A

    dt.propagate_donation(30, 200,
        [&](int lock_id){ return owner_of.count(lock_id) ? owner_of[lock_id] : -1; },
        [&](int pid){ return waiting_lock_of.count(pid) ? waiting_lock_of[pid] : -1; });

    std::cout << "L's effective priority = " << dt.effective(10) << " (expected 30)\n";
    std::cout << "M's effective priority = " << dt.effective(20) << " (expected 30)\n";
    CHECK(dt.effective(10) == 30); // donation reached all the way down to L
    CHECK(dt.effective(20) == 30);

    std::cout << "test_nested_donation: OK\n";
    return 0;
}
