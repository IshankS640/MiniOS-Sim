#pragma once
#include <queue>
#include <vector>
#include <functional>

namespace minios::sched {

class SleepQueue {
public:
    // Rule from the spec: sleeping for 0 or negative ticks returns immediately.
    void sleep(int pid, int now, int ticks) {
        if (ticks <= 0) { ready_immediately_.push_back(pid); return; }
        heap_.push({now + ticks, pid});
    }

    // Call once per tick. Returns the pids that should wake up now.
    std::vector<int> wake_ready(int now) {
        std::vector<int> woken = std::move(ready_immediately_);
        ready_immediately_.clear();
        while (!heap_.empty() && heap_.top().first <= now) {
            woken.push_back(heap_.top().second);
            heap_.pop();
        }
        return woken;
    }

    bool empty() const { return heap_.empty() && ready_immediately_.empty(); }

private:
    using Entry = std::pair<int,int>; // (wake_tick, pid)
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> heap_;
    std::vector<int> ready_immediately_;
};

} // namespace minios::sched
