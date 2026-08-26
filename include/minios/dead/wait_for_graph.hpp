#pragma once
#include <vector>

namespace minios::dead {

// Returns true if the directed graph (adjacency list, edge i->j means
// "process i is waiting for a resource held by process j") has a cycle.
bool has_cycle(const std::vector<std::vector<int>>& adj);

} // namespace minios::dead
