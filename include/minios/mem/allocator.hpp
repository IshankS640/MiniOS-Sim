#pragma once
#include <map>
#include <cstddef>
#include <string>

namespace minios::mem {

enum class Strategy { FirstFit, BestFit, WorstFit, NextFit };

struct Block { size_t size; bool free; };

class Allocator {
public:
    explicit Allocator(size_t heap_size, Strategy strat)
        : heap_size_(heap_size), strat_(strat) {
        by_addr_[0] = {heap_size, true};
        free_by_size_.insert({heap_size, 0});
    }

    // Returns the starting address of the allocated block, or (size_t)-1 on failure.
    size_t allocate(size_t req_size);
    void free(size_t addr);

    double external_fragmentation() const; // 1 - largest_free/total_free

private:
    size_t find_first_fit(size_t req) const;
    size_t find_best_fit(size_t req) const;
    size_t find_worst_fit(size_t req) const;
    size_t find_next_fit(size_t req);

    void split_and_take(size_t addr, size_t req);
    void coalesce(size_t addr);

    std::map<size_t, Block> by_addr_;                 // addr -> block info
    std::multimap<size_t, size_t> free_by_size_;       // size -> addr (free only)
    size_t heap_size_;
    Strategy strat_;
    size_t next_fit_rover_ = 0;
};

} // namespace minios::mem
