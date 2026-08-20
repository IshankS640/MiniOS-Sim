#pragma once
#include <unordered_map>
#include <set>
#include <vector>

namespace minios::sched {

struct WaiterKey {
    int neg_prio;
    int pid;
    bool operator<(const WaiterKey& o) const {
        if (neg_prio != o.neg_prio) return neg_prio < o.neg_prio;
        return pid < o.pid;
    }
};

struct Lock {
    int owner = -1;
    std::multiset<WaiterKey> waiters;
};

class LockTable {
public:
    Lock& get(int lock_id) { return locks_[lock_id]; }

    // Returns true if the lock was free and pid now owns it (no wait needed).
    bool try_acquire(int lock_id, int pid) {
        Lock& lk = locks_[lock_id];
        if (lk.owner == -1) { lk.owner = pid; return true; }
        return false;
    }

    void add_waiter(int lock_id, int pid, int priority) {
        locks_[lock_id].waiters.insert({-priority, pid});
    }

    void update_waiter_priority(int lock_id, int pid, int old_prio, int new_prio) {
        auto& w = locks_[lock_id].waiters;
        w.erase({-old_prio, pid});
        w.insert({-new_prio, pid});
    }

    // Release: give the lock to the highest-priority waiter, if any.
    // Returns the pid that now owns it, or -1 if nobody was waiting.
    int release(int lock_id) {
        Lock& lk = locks_[lock_id];
        lk.owner = -1;
        if (lk.waiters.empty()) return -1;
        auto it = lk.waiters.begin();
        int next_owner = it->pid;
        lk.waiters.erase(it);
        lk.owner = next_owner;
        return next_owner;
    }

private:
    std::unordered_map<int, Lock> locks_;
};

} // namespace minios::sched
