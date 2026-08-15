#include "minios/core/check.hpp"
#include "minios/core/types.hpp"

void test_pintos_constants() {
    CHECK_EQ(minios::PRI_MIN, 0);
    CHECK_EQ(minios::PRI_DEFAULT, 31);
    CHECK_EQ(minios::PRI_MAX, 63);
    CHECK_LT(minios::PRI_MIN, minios::PRI_MAX);
    CHECK_EQ(minios::TIMER_FREQ, 100);
    CHECK_EQ(minios::TIME_SLICE, 4);
}

void test_comparison_macros() {
    int a = 42;
    int b = 42;
    int c = 100;

    CHECK(a == b);
    CHECK_EQ(a, b);
    CHECK_NE(a, c);
    CHECK_LT(a, c);
    CHECK_LE(a, b);
    CHECK_LE(a, c);
    CHECK_GT(c, a);
    CHECK_GE(b, a);
}

int main() {
    test_pintos_constants();
    test_comparison_macros();
    return minios::core::TestRunner::instance().report();
}
