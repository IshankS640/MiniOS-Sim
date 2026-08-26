#include "minios/dead/wait_for_graph.hpp"

namespace minios::dead {

// 0 = unvisited, 1 = visiting (on current DFS stack), 2 = fully done
static bool dfs(int u, const std::vector<std::vector<int>>& adj, std::vector<int>& color) {
    color[u] = 1;
    for (int v : adj[u]) {
        if (color[v] == 1) return true;               // back edge -> cycle
        if (color[v] == 0 && dfs(v, adj, color)) return true;
    }
    color[u] = 2;
    return false;
}

bool has_cycle(const std::vector<std::vector<int>>& adj) {
    int n = (int)adj.size();
    std::vector<int> color(n, 0);
    for (int i = 0; i < n; ++i)
        if (color[i] == 0 && dfs(i, adj, color)) return true;
    return false;
}

} // namespace minios::dead
