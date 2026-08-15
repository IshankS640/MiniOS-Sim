#pragma once
#include <vector>

namespace minios {

// Every scheduler algorithm implements this interface.
class Scheduler {
public:
    virtual ~Scheduler() = default;
    virtual void on_arrival(int pid) = 0;      // process just became ready
    virtual int pick_next(int now) = 0;        // which pid runs this tick? -1 = idle
    virtual void on_tick_end(int running_pid, int now) = 0; // housekeeping
    virtual bool ready_empty() const = 0;
};

} // namespace minios
