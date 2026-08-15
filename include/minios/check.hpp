#pragma once
#include <iostream>
#include <cstdlib>

#define CHECK(cond) \
    do { \
        if (!(cond)) { \
            std::cerr << "CHECK FAILED: " #cond \
                      << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
            std::exit(1); \
        } \
    } while (0)
