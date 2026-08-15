#include "minios/engine.hpp"
#include "minios/check.hpp"
#include <queue>
#include <iostream>

// A minimal scheduler just for this test: run pids in the order they arrive.
class TinyFCFS : public minios::Scheduler {
public:
    void on_arrival(int pid) override { q_.push(pid); }
    int pick_next(int) override {
        if (!current_active_ && !q_.empty()) { current_ = q_.front(); q_.pop(); current_active_ = true; }
        return current_active_ ? current_ : -1;
    }
    void on_tick_end(int, int) override {}
    void done_with_current() { current_active_ = false; }
    bool ready_empty() const override { return q_.empty(); }
private:
    std::queue<int> q_;
    int current_ = -1;
    bool current_active_ = false;
};

int main() {
    std::vector<minios::Process> procs = {
        minios::Process(1, "A", 0, 31, 0),
        minios::Process(2, "B", 0, 31, 0),
    };
    procs[0].phases.push_back({minios::PhaseKind::CPU, 3});
    procs[1].phases.push_back({minios::PhaseKind::CPU, 2});

    TinyFCFS sched;
    minios::Engine eng(procs, sched);
    eng.run();
    eng.print_gantt();
    auto m = eng.compute_metrics();
    std::cout << "avg_turnaround=" << m.avg_turnaround << "\n";
    CHECK(m.cpu_utilization > 0.9);
    std::cout << "test_engine: OK\n";
    return 0;
}
