#include "minios/dead/bankers.hpp"
#include "minios/dead/wait_for_graph.hpp"
#include "minios/check.hpp"
#include <iostream>

int main() {
    // Classic textbook example: 5 processes, 3 resource types (A,B,C) = 10,5,7
    std::vector<int> available = {3,3,2};
    std::vector<std::vector<int>> allocation = {
        {0,1,0}, {2,0,0}, {3,0,2}, {2,1,1}, {0,0,2}
    };
    std::vector<std::vector<int>> max_demand = {
        {7,5,3}, {3,2,2}, {9,0,2}, {2,2,2}, {4,3,3}
    };

    auto result = minios::dead::check_safety(available, allocation, max_demand);
    CHECK(result.safe);
    std::cout << "safe=" << result.safe << ", sequence: ";
    for (int p : result.safe_sequence) std::cout << "P" << p << " ";
    std::cout << "(expected P1 P3 P4 P0 P2)\n";

    // Wait-for graph: P0 waits on P1, P1 waits on P2, P2 waits on P0 -> cycle
    std::vector<std::vector<int>> adj = { {1}, {2}, {0} };
    CHECK(minios::dead::has_cycle(adj) == true);
    std::cout << "cycle detected (expected true): " << minios::dead::has_cycle(adj) << "\n";

    // No cycle: P0 -> P1 -> P2 (chain, not a loop)
    std::vector<std::vector<int>> adj2 = { {1}, {2}, {} };
    CHECK(minios::dead::has_cycle(adj2) == false);

    std::cout << "test_deadlock: OK\n";
    return 0;
}
