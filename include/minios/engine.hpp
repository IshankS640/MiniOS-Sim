#pragma once
#include "minios/process.hpp"
#include "minios/scheduler.hpp"
#include <vector>
#include <string>

namespace minios {

struct Slice { int pid; int start; int end; };

struct Metrics {
    double avg_waiting = 0, avg_turnaround = 0, avg_response = 0;
    double cpu_utilization = 0;
};

class Engine {
public:
    Engine(std::vector<Process> procs, Scheduler& sched)
        : procs_(std::move(procs)), sched_(sched) {}

    void run();
    void print_gantt() const;
    Metrics compute_metrics() const;

private:
    std::vector<Process> procs_;
    Scheduler& sched_;
    std::vector<Slice> timeline_;
    int busy_ticks_ = 0;
    int total_ticks_ = 0;
};

} // namespace minios
