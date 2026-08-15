#include "minios/process.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace minios {

static Phase parse_phase(const std::string& tok) {
    // token looks like "C5", "S3", "L2", "U2"
    char c = tok[0];
    int val = std::stoi(tok.substr(1));
    switch (c) {
        case 'C': return {PhaseKind::CPU, val};
        case 'S': return {PhaseKind::SLEEP, val};
        case 'L': return {PhaseKind::LOCK, val};
        case 'U': return {PhaseKind::UNLOCK, val};
        default: throw std::runtime_error("bad phase token: " + tok);
    }
}

std::vector<Process> load_workload(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("cannot open workload file: " + path);

    std::vector<Process> procs;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        int pid, arrival, prio, nice;
        std::string name;
        iss >> pid >> name >> arrival >> prio >> nice;
        Process p(pid, name, arrival, prio, nice);
        std::string tok;
        while (iss >> tok) p.phases.push_back(parse_phase(tok));
        procs.push_back(std::move(p));
    }

    // sorted by arrival, then pid, so scheduling code can rely on order
    std::sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
        if (a.arrival != b.arrival) return a.arrival < b.arrival;
        return a.pid < b.pid;
    });
    return procs;
}

} // namespace minios
