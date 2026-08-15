#pragma once

#include <cstdint>
#include <string>

namespace minios {

using Tick = int64_t;
using Pid = int32_t;
using Priority = int;

// CS162 Pintos scheduling constants
constexpr Priority PRI_MIN = 0;
constexpr Priority PRI_DEFAULT = 31;
constexpr Priority PRI_MAX = 63;

// MLFQS constants (Pintos / 4.4BSD)
constexpr int TIMER_FREQ = 100;     // 100 timer ticks per second
constexpr int TIME_SLICE = 4;       // MLFQS priority recomputation interval (ticks)
constexpr int NICE_MIN = -20;
constexpr int NICE_DEFAULT = 0;
constexpr int NICE_MAX = 20;

} // namespace minios
