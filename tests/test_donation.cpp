#include "minios/sched/lock.hpp"
#include "minios/sched/donation.hpp"
#include "minios/check.hpp"
#include <iostream>

int main() {
    using namespace minios::sched;
    DonationTracker dt;
    LockTable locks;

    // L (pid 10, prio 10) holds lock 1.
    // H (pid 30, prio 30) wants lock 1 -> must wait -> donates to L.
    dt.set_base(10, 10);
    dt.set_base(30, 30);

    CHECK(locks.try_acquire(1, 10) == true); // L grabs it, free lock
    dt.add_held_lock(10, 1);

    CHECK(dt.effective(10) == 10); // before H even shows up

    // H tries to acquire lock 1, but it's held -> H waits and donates.
    bool got_it = locks.try_acquire(1, 30);
    CHECK(got_it == false);
    locks.add_waiter(1, 30, dt.base(30));
    dt.set_donor_for_lock(1, dt.base(30));

    // Without a scheduler in this simple test, we assert on the DonationTracker
    // directly: L's effective priority should now be boosted to H's (30),
    // which is exactly what prevents a Medium-priority thread from starving H.
    CHECK(dt.effective(10) == 30);
    std::cout << "L's effective priority after donation = " << dt.effective(10)
              << " (expected 30)\n";

    // L releases the lock -> H should get it -> L's boost goes away.
    int next_owner = locks.release(1);
    CHECK(next_owner == 30);
    dt.remove_held_lock(10, 1);
    dt.clear_donor_for_lock(1);
    dt.add_held_lock(30, 1);

    CHECK(dt.effective(10) == 10); // back to normal
    std::cout << "L's effective priority after release = " << dt.effective(10)
              << " (expected 10)\n";

    std::cout << "test_donation: OK\n";
    return 0;
}
