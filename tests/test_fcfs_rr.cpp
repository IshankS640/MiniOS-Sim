#include "minios/engine.hpp"
#include "minios/sched/fcfs.hpp"
#include "minios/sched/round_robin.hpp"
#include "minios/check.hpp"
#include <iostream>

int main() {
    // FCFS: P1(burst 3) then P2(burst 2), both arrive at 0
    {
        std::vector<minios::Process> procs = {
            minios::Process(1, "A", 0, 31, 0),
            minios::Process(2, "B", 0, 31, 0),
        };
        procs[0].phases.push_back({minios::PhaseKind::CPU, 3});
        procs[1].phases.push_back({minios::PhaseKind::CPU, 2});

        minios::sched::FCFS s;
        minios::Engine eng(procs, s);
        eng.run();
        eng.print_gantt();
        // Expect P1 runs 0-3, P2 runs 3-5
    }

    // Round Robin, quantum 2: P1(burst 5), P2(burst 3), both arrive at 0
    {
        std::vector<minios::Process> procs = {
            minios::Process(1, "A", 0, 31, 0),
            minios::Process(2, "B", 0, 31, 0),
        };
        procs[0].phases.push_back({minios::PhaseKind::CPU, 5});
        procs[1].phases.push_back({minios::PhaseKind::CPU, 3});

        minios::sched::RoundRobin s(2);
        minios::Engine eng(procs, s);
        eng.run();
        eng.print_gantt();
        // Expect pattern: P1 0-2, P2 2-4, P1 4-6, P2 6-7, P1 7-8
    }

    std::cout << "test_fcfs_rr: OK (visually check the two Gantt lines above)\n";
    return 0;
}
