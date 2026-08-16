#pragma once
#include "minios/scheduler.hpp"
#include <queue>

namespace minios::sched {

class RoundRobin : public Scheduler {
public:
    explicit RoundRobin(int quantum) : quantum_(quantum) {}

    void on_arrival(int pid) override { q_.push(pid); }

    int pick_next(int) override {
        if (current_ == -1 && !q_.empty()) {
            current_ = q_.front();
            q_.pop();
            ticks_this_slice_ = 0;
        }
        return current_;
    }

    void on_tick_end(int running_pid, int) override {
        if (running_pid == -1) return;
        ++ticks_this_slice_;
        if (ticks_this_slice_ >= quantum_) {
            // quantum expired: this process goes to the BACK of the queue,
            // unless it just finished (finished() will have already cleared it)
            if (current_ != -1) {
                q_.push(current_);
                current_ = -1;
            }
        }
    }

    // Engine calls this the instant a CPU phase completes (before quantum
    // expiry check would matter) so a finished process is not re-queued.
    void on_finished(int pid) override {
        if (pid == current_) current_ = -1;
    }

    bool ready_empty() const override { return q_.empty() && current_ == -1; }

private:
    std::queue<int> q_;
    int current_ = -1;
    int quantum_;
    int ticks_this_slice_ = 0;
};

} // namespace minios::sched
