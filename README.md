# MiniOS-Sim

A C++17 simulator of OS scheduling, memory management, and deadlock
handling, built to understand the algorithms in Berkeley CS162's Pintos
Project 2 and a standard OS lab manual — not to hack on a real kernel.

## Build
    cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
    cmake --build build

## Run
    ./build/osim.exe compare
    ./build/osim.exe page 3
    ./build/osim.exe bank

## Test
    cd build && ctest --output-on-failure

## What's implemented
- Schedulers: FCFS, Round Robin, SJF, SRTF, HRRN, preemptive Priority with
  nested donation, MLFQ (BSD-style, 64 priority levels).
- Memory: allocator (first/best/worst/next-fit, split + coalesce), page
  replacement (FIFO, LRU, OPT).
- Deadlock: Banker's algorithm, wait-for-graph cycle detection.

See `docs/complexity.md` for time/space analysis of every module.
