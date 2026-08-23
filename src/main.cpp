#include "minios/workload_gen.hpp"
#include "minios/compare.hpp"
#include <iostream>

int main() {
    auto workload = minios::generate_random_workload(6, /*seed=*/42);
    minios::run_comparison(workload);
    return 0;
}
