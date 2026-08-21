#pragma once
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace minios::sched {

// Tracks, per pid: its base priority and which locks it currently holds.
// effective_priority(pid) = max(base, best donation from any lock it holds).
class DonationTracker {
public:
    void set_base(int pid, int prio) { base_[pid] = prio; }
    int base(int pid) const { auto it = base_.find(pid); return it==base_.end()?0:it->second; }

    void add_held_lock(int pid, int lock_id) { held_[pid].push_back(lock_id); }
    void remove_held_lock(int pid, int lock_id) {
        auto& v = held_[pid];
        v.erase(std::remove(v.begin(), v.end(), lock_id), v.end());
    }

    // donors[lock_id] = highest priority currently waiting on that lock
    void set_donor_for_lock(int lock_id, int donor_priority) {
        lock_top_donor_[lock_id] = donor_priority;
    }
    void clear_donor_for_lock(int lock_id) { lock_top_donor_.erase(lock_id); }

    int effective(int pid) const {
        int best = base(pid);
        auto it = held_.find(pid);
        if (it != held_.end()) {
            for (int lock_id : it->second) {
                auto d = lock_top_donor_.find(lock_id);
                if (d != lock_top_donor_.end() && d->second > best) best = d->second;
            }
        }
        return best;
    }

    // Walk the chain: pid_waiting is blocked on lock_id, held by some owner.
    // Push pid_waiting's effective priority down through however many locks
    // it takes to reach the final holder. `owner_of_lock` and
    // `waiting_lock_of` are small lookup functions you pass in so this class
    // doesn't need to know about LockTable directly.
    template <typename OwnerOfLockFn, typename WaitingLockOfFn>
    void propagate_donation(int pid_waiting, int lock_id,
                             OwnerOfLockFn owner_of_lock,
                             WaitingLockOfFn waiting_lock_of) {
        int donor_priority = effective(pid_waiting);
        int current_lock = lock_id;
        int guard = 0; // safety: chain length can never exceed number of pids
        while (current_lock != -1 && guard++ < 10000) {
            set_donor_for_lock(current_lock, donor_priority);
            int owner = owner_of_lock(current_lock);
            if (owner == -1) break;
            int next_lock = waiting_lock_of(owner); // -1 if owner isn't blocked
            current_lock = next_lock;
        }
    }

private:
    std::unordered_map<int,int> base_;
    std::unordered_map<int, std::vector<int>> held_;
    std::unordered_map<int,int> lock_top_donor_; // lock_id -> top waiter priority
};

} // namespace minios::sched
