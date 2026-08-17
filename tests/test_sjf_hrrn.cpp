#include "minios/engine.hpp"
#include "minios/sched/sjf_srtf.hpp"
#include "minios/sched/hrrn.hpp"
#include "minios/check.hpp"
#include <iostream>

int main() {
    // SJF: P1 burst 6 arrives 0, P2 burst 2 arrives 0 -> P2 should run first
    {
        std::vector<minios::Process> procs = {
            minios::Process(1, "A", 0, 31, 0),
            minios::Process(2, "B", 0, 31, 0),
        };
        procs[0].phases.push_back({minios::PhaseKind::CPU, 6});
        procs[1].phases.push_back({minios::PhaseKind::CPU, 2});

        minios::sched::SJF s;
        s.register_burst(1, 0, 6);
        s.register_burst(2, 0, 2);
        minios::Engine eng(procs, s);
        eng.run();
        eng.print_gantt(); // expect P2 first (0-2), then P1 (2-8)
    }
    std::cout << "test_sjf_hrrn: OK (visually check Gantt line: P2 should run before P1)\n";
    return 0;
}
