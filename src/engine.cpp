#include "minios/engine.hpp"
#include <iostream>
#include <algorithm>

namespace minios {

Process* find_by_pid(std::vector<Process>& v, int pid) {
    for (auto& p : v) if (p.pid == pid) return &p;
    return nullptr;
}

void Engine::run() {
    size_t next_arrival_idx = 0;
    int now = 0;
    int remaining_procs = static_cast<int>(procs_.size());
    int last_pid = -2; // -2 = "nothing yet", so first slice always starts fresh

    // initialize remaining_burst for the first CPU phase of each process
    for (auto& p : procs_) {
        if (!p.phases.empty() && p.phases.front().kind == PhaseKind::CPU)
            p.remaining_burst = p.phases.front().arg;
    }

    while (remaining_procs > 0) {
        // admit anyone arriving exactly at "now"
        while (next_arrival_idx < procs_.size() &&
               procs_[next_arrival_idx].arrival == now) {
            sched_.on_arrival(procs_[next_arrival_idx].pid);
            ++next_arrival_idx;
        }

        int pid = sched_.pick_next(now);

        if (pid == -1) {
            // CPU idle this tick
            if (last_pid != -1) timeline_.push_back({-1, now, now + 1});
            else timeline_.back().end = now + 1;
            last_pid = -1;
        } else {
            Process* p = find_by_pid(procs_, pid);
            if (p->start_time == -1) p->start_time = now;

            if (last_pid == pid) timeline_.back().end = now + 1;
            else timeline_.push_back({pid, now, now + 1});
            last_pid = pid;

            ++busy_ticks_;
            p->remaining_burst -= 1;

            if (p->remaining_burst == 0) {
                // this CPU phase is done; move to the next phase
                p->phases.pop_front();
                if (p->phases.empty()) {
                    p->finish_time = now + 1;
                    --remaining_procs;
                    sched_.on_finished(pid);
                } else if (p->phases.front().kind == PhaseKind::CPU) {
                    p->remaining_burst = p->phases.front().arg;
                }
                // SLEEP/LOCK/UNLOCK phases are handled in later days
            }
        }

        sched_.on_tick_end(pid, now);
        ++now;
        ++total_ticks_;

        if (now > 100000) { std::cerr << "engine: runaway loop, aborting\n"; break; }
    }
}

void Engine::print_gantt() const {
    for (const auto& s : timeline_) {
        std::string label = (s.pid == -1) ? "IDLE" : ("P" + std::to_string(s.pid));
        std::cout << "[" << label << " " << s.start << "-" << s.end << "] ";
    }
    std::cout << "\n";
}

Metrics Engine::compute_metrics() const {
    Metrics m;
    double sum_wait = 0, sum_turn = 0, sum_resp = 0;
    for (const auto& p : procs_) {
        int turnaround = p.finish_time - p.arrival;
        int response = p.start_time - p.arrival;
        // waiting = turnaround - total CPU time actually used
        // (we recompute total CPU time from original workload if needed;
        //  for Day 2's pure-CPU workloads, turnaround - burst == waiting)
        sum_turn += turnaround;
        sum_resp += response;
    }
    size_t n = procs_.size();
    m.avg_turnaround = sum_turn / n;
    m.avg_response = sum_resp / n;
    m.avg_waiting = m.avg_turnaround; // refined once phases include non-CPU time
    m.cpu_utilization = total_ticks_ ? (double)busy_ticks_ / total_ticks_ : 0.0;
    return m;
}

} // namespace minios
