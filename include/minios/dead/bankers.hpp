#pragma once
#include <vector>

namespace minios::dead {

struct BankersResult {
    bool safe;
    std::vector<int> safe_sequence; // valid only if safe == true
};

BankersResult check_safety(
    const std::vector<int>& available,
    const std::vector<std::vector<int>>& allocation,
    const std::vector<std::vector<int>>& max_demand);

} // namespace minios::dead
