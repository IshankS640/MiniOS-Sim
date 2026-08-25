#pragma once
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <queue>

namespace minios::mem {

int fifo_faults(const std::vector<int>& refs, int frames);
int lru_faults(const std::vector<int>& refs, int frames);
int opt_faults(const std::vector<int>& refs, int frames);

} // namespace minios::mem
