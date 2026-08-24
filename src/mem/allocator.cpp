#include "minios/mem/allocator.hpp"
#include <limits>

namespace minios::mem {

size_t Allocator::find_first_fit(size_t req) const {
    for (const auto& [addr, blk] : by_addr_)
        if (blk.free && blk.size >= req) return addr;
    return static_cast<size_t>(-1);
}

size_t Allocator::find_best_fit(size_t req) const {
    auto it = free_by_size_.lower_bound(req); // smallest free block that still fits
    return it == free_by_size_.end() ? static_cast<size_t>(-1) : it->second;
}

size_t Allocator::find_worst_fit(size_t req) const {
    if (free_by_size_.empty()) return static_cast<size_t>(-1);
    auto it = free_by_size_.rbegin(); // largest free block
    if (it->first < req) return static_cast<size_t>(-1);
    return it->second;
}

size_t Allocator::find_next_fit(size_t req) {
    auto it = by_addr_.lower_bound(next_fit_rover_);
    for (int pass = 0; pass < 2; ++pass) { // wrap around once
        for (; it != by_addr_.end(); ++it)
            if (it->second.free && it->second.size >= req) {
                next_fit_rover_ = it->first;
                return it->first;
            }
        it = by_addr_.begin();
    }
    return static_cast<size_t>(-1);
}

void Allocator::split_and_take(size_t addr, size_t req) {
    Block& blk = by_addr_[addr];
    size_t old_size = blk.size;

    // remove old free-size entry
    auto range = free_by_size_.equal_range(old_size);
    for (auto it = range.first; it != range.second; ++it)
        if (it->second == addr) { free_by_size_.erase(it); break; }

    blk.size = req;
    blk.free = false;

    if (old_size > req) {
        size_t remainder_addr = addr + req;
        by_addr_[remainder_addr] = {old_size - req, true};
        free_by_size_.insert({old_size - req, remainder_addr});
    }
}

size_t Allocator::allocate(size_t req_size) {
    size_t addr;
    switch (strat_) {
        case Strategy::FirstFit: addr = find_first_fit(req_size); break;
        case Strategy::BestFit:  addr = find_best_fit(req_size); break;
        case Strategy::WorstFit: addr = find_worst_fit(req_size); break;
        case Strategy::NextFit:  addr = find_next_fit(req_size); break;
        default: addr = static_cast<size_t>(-1);
    }
    if (addr == static_cast<size_t>(-1)) return addr;
    split_and_take(addr, req_size);
    return addr;
}

void Allocator::coalesce(size_t addr) {
    // merge with the block right after, if it's also free
    auto it = by_addr_.find(addr);
    auto next = std::next(it);
    if (next != by_addr_.end() && next->second.free) {
        auto range = free_by_size_.equal_range(next->second.size);
        for (auto r = range.first; r != range.second; ++r)
            if (r->second == next->first) { free_by_size_.erase(r); break; }
        it->second.size += next->second.size;
        by_addr_.erase(next);
    }
    // merge with the block right before, if it's also free
    if (it != by_addr_.begin()) {
        auto prev = std::prev(it);
        if (prev->second.free) {
            auto range = free_by_size_.equal_range(prev->second.size);
            for (auto r = range.first; r != range.second; ++r)
                if (r->second == prev->first) { free_by_size_.erase(r); break; }
            prev->second.size += it->second.size;
            by_addr_.erase(it);
            it = prev;
        }
    }
    // re-insert this block's final size into the free index
    auto range = free_by_size_.equal_range(it->second.size);
    bool already = false;
    for (auto r = range.first; r != range.second; ++r)
        if (r->second == it->first) { already = true; break; }
    if (!already) free_by_size_.insert({it->second.size, it->first});
}

void Allocator::free(size_t addr) {
    by_addr_[addr].free = true;
    coalesce(addr);
}

double Allocator::external_fragmentation() const {
    if (free_by_size_.empty()) return 0.0;
    size_t total = 0, largest = 0;
    for (const auto& [sz, a] : free_by_size_) { total += sz; if (sz > largest) largest = sz; }
    if (total == 0) return 0.0;
    return 1.0 - (double)largest / (double)total;
}

} // namespace minios::mem
