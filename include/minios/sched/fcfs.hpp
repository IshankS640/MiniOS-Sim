#pragma once
#include "minios/scheduler.hpp"
#include <queue>

namespace minios::sched {

class FCFS : public Scheduler {
public:
    void on_arrival(int pid) override { q_.push(pid); }

    int pick_next(int) override {
        if (current_ == -1 && !q_.empty()) {
            current_ = q_.front();
            q_.pop();
        }
        return current_;
    }

    void on_finished(int pid) override {
        if (pid == current_) current_ = -1;
    }

    void on_tick_end(int, int) override {}
    bool ready_empty() const override { return q_.empty() && current_ == -1; }

private:
    std::queue<int> q_;
    int current_ = -1;
};

} // namespace minios::sched
