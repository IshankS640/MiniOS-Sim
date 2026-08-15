#pragma once
#include <string>
#include <deque>
#include <vector>

namespace minios {

enum class PhaseKind { CPU, SLEEP, LOCK, UNLOCK };

struct Phase {
    PhaseKind kind;
    int arg; // ticks for CPU/SLEEP, lock id for LOCK/UNLOCK
};

struct Process {
    int pid;
    std::string name;
    int arrival;
    int base_priority;
    int nice;
    std::deque<Phase> phases;

    // filled in as the simulation runs
    int effective_priority = 0;
    int remaining_burst = 0;   // ticks left in current CPU phase
    int start_time = -1;       // first time it got the CPU
    int finish_time = -1;
    int waiting_time = 0;
    int recent_cpu = 0;        // used later for MLFQ

    Process() = default;
    Process(int p, std::string n, int arr, int prio, int ni)
        : pid(p), name(std::move(n)), arrival(arr),
          base_priority(prio), nice(ni), effective_priority(prio) {}
};

std::vector<Process> load_workload(const std::string& path);

} // namespace minios
