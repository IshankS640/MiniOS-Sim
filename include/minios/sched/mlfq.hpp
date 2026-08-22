#pragma once
#include "minios/scheduler.hpp"
#include <array>
#include <list>
#include <unordered_map>
#include <cstdint>

namespace minios::sched {

constexpr int PRI_MIN = 0, PRI_MAX = 63;

class MLFQ : public Scheduler {
public:
    void set_nice(int pid, int nice) { nice_[pid] = nice; }

    void on_arrival(int pid) override {
        recent_cpu_[pid] = 0;
        int prio = compute_priority(pid);
        insert(pid, prio);
    }

    int pick_next(int) override {
        if (current_ != -1) return current_;
        if (mask_ == 0) return -1;
        int level = highest_nonempty_level();
        current_ = queues_[level].front();
        queues_[level].pop_front();
        if (queues_[level].empty()) mask_ &= ~(1ULL << level);
        return current_;
    }

    void on_tick_end(int running_pid, int now) override {
        if (running_pid != -1) recent_cpu_[running_pid] += 1;

        // recompute priority every 4th tick for every known process
        if ((now + 1) % 4 == 0) {
            for (auto& [pid, prio] : priority_) {
                if (pid == current_) continue; // handled when it re-enters ready
                int newp = compute_priority(pid);
                if (newp != prio) requeue(pid, newp);
            }
            if (current_ != -1) priority_[current_] = compute_priority(current_);
        }

        // once per 100 ticks: recompute load_avg and recent_cpu for everyone
        if ((now + 1) % 100 == 0) {
            int ready_threads = ready_count() + (current_ != -1 ? 1 : 0);
            load_avg_ = (59.0/60.0) * load_avg_ + (1.0/60.0) * ready_threads;
            for (auto& [pid, rc] : recent_cpu_) {
                double coeff = (2.0 * load_avg_) / (2.0 * load_avg_ + 1.0);
                rc = static_cast<int>(coeff * rc + nice_[pid]);
            }
        }
    }

    void on_finished(int pid) override {
        if (pid == current_) { current_ = -1; priority_.erase(pid); recent_cpu_.erase(pid); }
    }

    // Called by the engine right after pick_next() returns -2 meaning
    // "the running process used up this tick and must go back to ready."
    // For MLFQ every tick is effectively a 1-tick quantum re-evaluation,
    // so the engine calls this once per tick when running_pid == current_.
    void requeue_current_if_needed(int now) {
        if (current_ == -1) return;
        if ((now + 1) % 4 == 0) {
            int newp = compute_priority(current_);
            int pid = current_;
            current_ = -1;
            insert(pid, newp);
        }
    }

    bool ready_empty() const override { return mask_ == 0 && current_ == -1; }

private:
    int compute_priority(int pid) {
        int rc = recent_cpu_.count(pid) ? recent_cpu_[pid] : 0;
        int ni = nice_.count(pid) ? nice_[pid] : 0;
        int p = PRI_MAX - (rc / 4) - (ni * 2);
        if (p < PRI_MIN) p = PRI_MIN;
        if (p > PRI_MAX) p = PRI_MAX;
        return p;
    }

    void insert(int pid, int prio) {
        priority_[pid] = prio;
        queues_[prio].push_back(pid);
        mask_ |= (1ULL << prio);
        pos_[pid] = std::prev(queues_[prio].end());
    }

    void requeue(int pid, int new_prio) {
        int old_prio = priority_[pid];
        queues_[old_prio].erase(pos_[pid]);
        if (queues_[old_prio].empty()) mask_ &= ~(1ULL << old_prio);
        insert(pid, new_prio);
    }

    int highest_nonempty_level() const {
        // find the highest set bit -> O(1) via compiler builtin
        return 63 - __builtin_clzll(mask_);
    }

    int ready_count() const {
        int c = 0;
        for (int i = 0; i <= PRI_MAX; ++i) c += static_cast<int>(queues_[i].size());
        return c;
    }

    std::array<std::list<int>, PRI_MAX+1> queues_;
    std::unordered_map<int, std::list<int>::iterator> pos_;
    uint64_t mask_ = 0;
    std::unordered_map<int,int> priority_;
    std::unordered_map<int,int> recent_cpu_;
    std::unordered_map<int,int> nice_;
    double load_avg_ = 0.0;
    int current_ = -1;
};

} // namespace minios::sched
