#pragma once
#include "minios/process.hpp"
#include <random>
#include <vector>
#include <algorithm>

namespace minios {

inline std::vector<Process> generate_random_workload(int n, unsigned seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> arrival_d(0, 20);
    std::uniform_int_distribution<int> burst_d(1, 10);
    std::uniform_int_distribution<int> prio_d(0, 63);

    std::vector<Process> procs;
    for (int i = 1; i <= n; ++i) {
        Process p(i, "P" + std::to_string(i), arrival_d(rng), prio_d(rng), 0);
        p.phases.push_back({PhaseKind::CPU, burst_d(rng)});
        procs.push_back(p);
    }
    std::sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        if (a.arrival != b.arrival) return a.arrival < b.arrival;
        return a.pid < b.pid;
    });
    return procs;
}

} // namespace minios
