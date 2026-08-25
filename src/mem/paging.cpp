#include "minios/mem/paging.hpp"
#include <climits>

namespace minios::mem {

int fifo_faults(const std::vector<int>& refs, int frames) {
    std::queue<int> order;
    std::unordered_set<int> in_memory;
    int faults = 0;
    for (int page : refs) {
        if (in_memory.count(page)) continue;
        ++faults;
        if ((int)in_memory.size() == frames) {
            int victim = order.front(); order.pop();
            in_memory.erase(victim);
        }
        in_memory.insert(page);
        order.push(page);
    }
    return faults;
}

int lru_faults(const std::vector<int>& refs, int frames) {
    // list front = most recently used, back = least recently used
    std::list<int> recency;
    std::unordered_map<int, std::list<int>::iterator> pos;
    int faults = 0;
    for (int page : refs) {
        auto it = pos.find(page);
        if (it != pos.end()) {
            recency.erase(it->second);          // O(1) thanks to stored iterator
            recency.push_front(page);
            pos[page] = recency.begin();
            continue;
        }
        ++faults;
        if ((int)recency.size() == frames) {
            int victim = recency.back();
            recency.pop_back();
            pos.erase(victim);
        }
        recency.push_front(page);
        pos[page] = recency.begin();
    }
    return faults;
}

int opt_faults(const std::vector<int>& refs, int frames) {
    int n = (int)refs.size();
    std::unordered_set<int> in_memory;
    int faults = 0;
    for (int i = 0; i < n; ++i) {
        int page = refs[i];
        if (in_memory.count(page)) continue;
        ++faults;
        if ((int)in_memory.size() == frames) {
            // find the page in memory used furthest in the future (or never again)
            int victim = -1, farthest = -1;
            for (int p : in_memory) {
                int next_use = INT_MAX;
                for (int j = i + 1; j < n; ++j) if (refs[j] == p) { next_use = j; break; }
                if (next_use > farthest) { farthest = next_use; victim = p; }
            }
            in_memory.erase(victim);
        }
        in_memory.insert(page);
    }
    return faults;
}

} // namespace minios::mem
