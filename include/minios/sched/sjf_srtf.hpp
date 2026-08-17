#pragma once
#include "minios/scheduler.hpp"
#include <queue>
#include <vector>
#include <unordered_map>

namespace minios::sched {

// key = (remaining_burst, arrival, pid) so ties break by arrival then pid
using SjfKey = std::tuple<int,int,int>;

class SJF : public Scheduler { // non-preemptive
public:
    void register_burst(int pid, int arrival, int burst) {
        burst_[pid] = burst;
        heap_.push({burst, arrival, pid});
    }
    void on_arrival(int) override {} // pid enters via register_burst instead
    int pick_next(int) override {
        if (current_ == -1 && !heap_.empty()) {
            current_ = std::get<2>(heap_.top());
            heap_.pop();
        }
        return current_;
    }
    void on_tick_end(int, int) override {}
    void on_finished(int pid) override { if (pid == current_) current_ = -1; }
    bool ready_empty() const override { return heap_.empty() && current_ == -1; }

private:
    std::priority_queue<SjfKey, std::vector<SjfKey>, std::greater<SjfKey>> heap_;
    std::unordered_map<int,int> burst_;
    int current_ = -1;
};

class SRTF : public Scheduler { // preemptive
public:
    void on_arrival(int) override {} // use register_burst like SJF
    void register_burst(int pid, int arrival, int burst) {
        heap_.push({burst, arrival, pid});
    }
    // engine must call this every tick with the running process's *updated*
    // remaining time so we can re-insert it into the heap for comparison
    void requeue_running(int pid, int arrival, int remaining) {
        if (remaining > 0) heap_.push({remaining, arrival, pid});
        current_ = -1;
    }
    int pick_next(int) override {
        if (!heap_.empty()) {
            current_ = std::get<2>(heap_.top());
            heap_.pop();
        }
        return current_;
    }
    void on_tick_end(int, int) override {}
    void on_finished(int pid) override { if (pid == current_) current_ = -1; }
    bool ready_empty() const override { return heap_.empty() && current_ == -1; }

private:
    std::priority_queue<SjfKey, std::vector<SjfKey>, std::greater<SjfKey>> heap_;
    int current_ = -1;
};

} // namespace minios::sched
