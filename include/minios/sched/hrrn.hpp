#pragma once
#include "minios/scheduler.hpp"
#include <vector>
#include <algorithm>

namespace minios::sched {

class HRRN : public Scheduler { // non-preemptive
public:
    void on_arrival(int) override {}
    void register_burst(int pid, int burst) {
        ready_.push_back({pid, burst, 0});
    }
    int pick_next(int) override {
        if (current_ != -1) return current_;
        if (ready_.empty()) return -1;
        // scan for highest response ratio = (wait + burst) / burst
        auto best = std::max_element(ready_.begin(), ready_.end(),
            [](const Entry& a, const Entry& b) {
                double ra = double(a.wait + a.burst) / a.burst;
                double rb = double(b.wait + b.burst) / b.burst;
                return ra < rb;
            });
        current_ = best->pid;
        ready_.erase(best);
        return current_;
    }
    void on_tick_end(int running_pid, int) override {
        for (auto& e : ready_) if (e.pid != running_pid) e.wait += 1;
    }
    void on_finished(int pid) override { if (pid == current_) current_ = -1; }
    bool ready_empty() const override { return ready_.empty() && current_ == -1; }

private:
    struct Entry { int pid, burst, wait; };
    std::vector<Entry> ready_;
    int current_ = -1;
};

} // namespace minios::sched
