#include "minios/dead/bankers.hpp"

namespace minios::dead {

BankersResult check_safety(
    const std::vector<int>& available,
    const std::vector<std::vector<int>>& allocation,
    const std::vector<std::vector<int>>& max_demand) {

    int n = (int)allocation.size();
    int m = (int)available.size();

    std::vector<std::vector<int>> need(n, std::vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            need[i][j] = max_demand[i][j] - allocation[i][j];

    std::vector<int> work = available;
    std::vector<bool> finished(n, false);
    std::vector<int> sequence;

    for (int iter = 0; iter < n; ++iter) {
        bool found = false;
        for (int i = 0; i < n; ++i) {
            if (finished[i]) continue;
            bool can_run = true;
            for (int j = 0; j < m; ++j)
                if (need[i][j] > work[j]) { can_run = false; break; }
            if (!can_run) continue;

            for (int j = 0; j < m; ++j) work[j] += allocation[i][j];
            finished[i] = true;
            sequence.push_back(i);
            found = true;
            break;
        }
        if (!found) break; // stuck: no process can proceed -> unsafe
    }

    bool all_done = true;
    for (bool f : finished) if (!f) { all_done = false; break; }

    return { all_done, all_done ? sequence : std::vector<int>{} };
}

} // namespace minios::dead
