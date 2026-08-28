# MiniOS-Sim — Complexity Notes

## CPU scheduling
- FCFS / Round Robin: `std::queue`. Push/pop O(1). Total simulation is
  O(T) where T = total simulated ticks.
- SJF / SRTF: `std::priority_queue` (min-heap). Insert/pop O(log n).
- HRRN: plain `std::vector`, linear scan O(n) per pick, because the
  response-ratio key changes every tick for every waiting process — a
  heap's ordering would go stale immediately, so sorting isn't worth it.
- Priority (with donation): `std::set`, O(log n) insert/erase/re-insert.
  Chosen over `priority_queue` specifically because priorities change
  while a process is queued (donation), and only a set supports removing
  an arbitrary element in O(log n).
- Sleep queue: min-heap on wake tick, O(log n) per sleep, O(k log n) to
  wake k processes.
- Nested donation: chain length is bounded by n (number of processes), so
  worst case O(n log n) to propagate one donation.
- MLFQ: array of 64 `std::list`s + a 64-bit bitmask. Picking the top
  non-empty queue is O(1) using `__builtin_clzll`. Re-queuing on priority
  change is O(1) because each process's list iterator is cached.

## Memory allocator
- `std::map<addr, Block>` gives address order for O(log B) neighbor
  lookup during coalescing (B = number of blocks).
- `std::multimap<size, addr>` makes best-fit and worst-fit O(log B)
  instead of an O(B) scan.
- First-fit and next-fit are O(B) worst case (next-fit is faster in
  practice because it starts from where it left off).

## Paging
- FIFO: `queue` + `unordered_set`, O(1) per reference.
- LRU: `list` + `unordered_map` of iterators, O(1) per reference — this
  is the one to highlight in interviews, since a naive LRU (scanning all
  frames for the oldest) is O(frames) per reference.
- OPT: O(N) per eviction in this simple version (N = length of remaining
  reference string), so O(N²) overall. Real implementations precompute
  next-use indices in one O(N) pass to bring this down to O(N log F).

## Deadlock
- Banker's safety check: O(n² · m) — n processes, m resource types, up to
  n passes each scanning n processes and m resources.
- Wait-for graph cycle detection: O(V + E) via 3-color DFS.

## The one big caveat
This simulator is **tick-driven**: it loops once per time unit, so total
runtime depends on T (total simulated ticks), not just n (process count).
A production-grade simulator would be **event-driven** — jumping straight
to the next interesting event (arrival, quantum expiry, lock release) —
which is O(n log n) regardless of how long the simulated time span is.
