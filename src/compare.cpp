#include "minios/compare.hpp"
#include "minios/engine.hpp"
#include "minios/sched/fcfs.hpp"
#include "minios/sched/round_robin.hpp"
#include <iostream>
#include <iomanip>

namespace minios {

void run_comparison(const std::vector<Process>& base_workload) {
    std::cout << std::left << std::setw(15) << "Algorithm"
              << std::setw(15) << "AvgTurnaround"
              << std::setw(15) << "AvgResponse"
              << "CPU-Util\n";

    {
        auto procs = base_workload;
        sched::FCFS s;
        Engine eng(procs, s);
        eng.run();
        auto m = eng.compute_metrics();
        std::cout << std::setw(15) << "FCFS" << std::setw(15) << m.avg_turnaround
                  << std::setw(15) << m.avg_response << m.cpu_utilization << "\n";
    }
    {
        auto procs = base_workload;
        sched::RoundRobin s(4);
        Engine eng(procs, s);
        eng.run();
        auto m = eng.compute_metrics();
        std::cout << std::setw(15) << "RoundRobin(4)" << std::setw(15) << m.avg_turnaround
                  << std::setw(15) << m.avg_response << m.cpu_utilization << "\n";
    }
}

} // namespace minios
