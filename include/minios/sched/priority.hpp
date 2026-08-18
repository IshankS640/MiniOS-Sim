#pragma once
#include "minios/scheduler.hpp"
#include <set>
#include <unordered_map>

namespace minios::sched {

// Key = (-priority, insertion_seq, pid). Negating priority makes the
// *smallest* key in the set correspond to the *highest* priority, so
// set.begin() is always "who should run next." insertion_seq breaks ties
// in FIFO order among equal priorities.
struct PKey {
    int neg_prio;
    long long seq;
    int pid;
    bool operator<(const PKey& o) const {
        if (neg_prio != o.neg_prio) return neg_prio < o.neg_prio;
        return seq < o.seq;
    }
};

class PriorityScheduler : public Scheduler {
public:
    void on_arrival(int pid) override { insert(pid, base_priority_[pid]); }

    void set_base_priority(int pid, int prio) { base_priority_[pid] = prio; }

    int pick_next(int) override {
        if (current_ == -1 && !ready_.empty()) {
            auto it = ready_.begin();
            current_ = it->pid;
            current_key_ = *it;
            ready_.erase(it);
        }
        return current_;
    }

    void on_tick_end(int, int) override {}

    void on_finished(int pid) override {
        if (pid == current_) current_ = -1;
    }

    // Called when a running process's priority changes (Day 7+). If it is
    // no longer the highest, the engine should preempt it back to ready.
    void change_priority(int pid, int new_prio) {
        base_priority_[pid] = new_prio;
        if (pid == current_) {
            current_key_.neg_prio = -new_prio;
            // engine decides whether to preempt by comparing against ready_.begin()
        } else {
            // find and re-insert with new key (linear scan by pid is O(n);
            // acceptable for a learning project - a pid->iterator map would
            // make this O(log n) too, left as a stretch improvement)
            for (auto it = ready_.begin(); it != ready_.end(); ++it) {
                if (it->pid == pid) {
                    ready_.erase(it);
                    break;
                }
            }
            insert(pid, new_prio);
        }
    }

    bool should_preempt_current() const {
        if (current_ == -1 || ready_.empty()) return false;
        return ready_.begin()->neg_prio < current_key_.neg_prio;
    }

    // Engine calls this to actually perform the preemption.
    void preempt_current() {
        if (current_ == -1) return;
        ready_.insert(current_key_);
        current_ = -1;
    }

    bool ready_empty() const override { return ready_.empty() && current_ == -1; }

private:
    void insert(int pid, int prio) {
        ready_.insert({-prio, seq_++, pid});
    }
    std::set<PKey> ready_;
    std::unordered_map<int,int> base_priority_;
    int current_ = -1;
    PKey current_key_{};
    long long seq_ = 0;
};

} // namespace minios::sched
