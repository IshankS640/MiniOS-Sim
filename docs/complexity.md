# MiniOS-Sim Complexity Reference Guide

This document tracks the asymptotic time and space complexity of each subsystem in MiniOS-Sim, along with brief explanations suitable for technical interviews.

---

## Day 1: Test Harness & Core Types (`minios::core`)

- **Time Complexity:** $O(1)$ per assertion (`CHECK`, `CHECK_EQ`). Comparison and counter updates run in constant time without any dynamic heap reallocations.
- **Space Complexity:** $O(1)$ auxiliary space. The singleton `TestRunner` only maintains two 32-bit integers (`total_assertions_`, `failed_assertions_`).
- **Interview Pitch:** "Our test framework operates with zero external dependencies and zero heap allocations per check. In-place assertion tracking guarantees zero performance overhead during testing loops."

---
