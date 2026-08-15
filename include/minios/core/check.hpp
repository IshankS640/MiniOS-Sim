#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

namespace minios {
namespace core {

class TestRunner {
public:
    static TestRunner& instance() {
        static TestRunner runner;
        return runner;
    }

    void record_pass() {
        total_assertions_++;
    }

    void record_fail(const char* file, int line, const std::string& expression, const std::string& message = "") {
        total_assertions_++;
        failed_assertions_++;
        std::cerr << "[FAIL] " << file << ":" << line << ": " << expression;
        if (!message.empty()) {
            std::cerr << " (" << message << ")";
        }
        std::cerr << "\n";
    }

    int failures() const { return failed_assertions_; }
    int assertions() const { return total_assertions_; }

    void reset() {
        total_assertions_ = 0;
        failed_assertions_ = 0;
    }

    int report() const {
        std::cout << "\n----------------------------------------\n";
        if (failed_assertions_ == 0) {
            std::cout << "All " << total_assertions_ << " checks PASSED!\n";
            return 0;
        } else {
            std::cerr << failed_assertions_ << " of " << total_assertions_ << " checks FAILED!\n";
            return 1;
        }
    }

private:
    int total_assertions_ = 0;
    int failed_assertions_ = 0;
};

} // namespace core
} // namespace minios

#define CHECK(cond) \
    do { \
        if (cond) { \
            ::minios::core::TestRunner::instance().record_pass(); \
        } else { \
            ::minios::core::TestRunner::instance().record_fail(__FILE__, __LINE__, #cond); \
        } \
    } while (false)

#define CHECK_EQ(val1, val2) \
    do { \
        auto _v1 = (val1); \
        auto _v2 = (val2); \
        if (_v1 == _v2) { \
            ::minios::core::TestRunner::instance().record_pass(); \
        } else { \
            std::ostringstream _oss; \
            _oss << "Expected " << _v1 << " == " << _v2; \
            ::minios::core::TestRunner::instance().record_fail(__FILE__, __LINE__, #val1 " == " #val2, _oss.str()); \
        } \
    } while (false)

#define CHECK_NE(val1, val2) \
    do { \
        auto _v1 = (val1); \
        auto _v2 = (val2); \
        if (_v1 != _v2) { \
            ::minios::core::TestRunner::instance().record_pass(); \
        } else { \
            std::ostringstream _oss; \
            _oss << "Expected " << _v1 << " != " << _v2; \
            ::minios::core::TestRunner::instance().record_fail(__FILE__, __LINE__, #val1 " != " #val2, _oss.str()); \
        } \
    } while (false)

#define CHECK_LT(val1, val2) \
    do { \
        auto _v1 = (val1); \
        auto _v2 = (val2); \
        if (_v1 < _v2) { \
            ::minios::core::TestRunner::instance().record_pass(); \
        } else { \
            std::ostringstream _oss; \
            _oss << "Expected " << _v1 << " < " << _v2; \
            ::minios::core::TestRunner::instance().record_fail(__FILE__, __LINE__, #val1 " < " #val2, _oss.str()); \
        } \
    } while (false)

#define CHECK_LE(val1, val2) \
    do { \
        auto _v1 = (val1); \
        auto _v2 = (val2); \
        if (_v1 <= _v2) { \
            ::minios::core::TestRunner::instance().record_pass(); \
        } else { \
            std::ostringstream _oss; \
            _oss << "Expected " << _v1 << " <= " << _v2; \
            ::minios::core::TestRunner::instance().record_fail(__FILE__, __LINE__, #val1 " <= " #val2, _oss.str()); \
        } \
    } while (false)

#define CHECK_GT(val1, val2) \
    do { \
        auto _v1 = (val1); \
        auto _v2 = (val2); \
        if (_v1 > _v2) { \
            ::minios::core::TestRunner::instance().record_pass(); \
        } else { \
            std::ostringstream _oss; \
            _oss << "Expected " << _v1 << " > " << _v2; \
            ::minios::core::TestRunner::instance().record_fail(__FILE__, __LINE__, #val1 " > " #val2, _oss.str()); \
        } \
    } while (false)

#define CHECK_GE(val1, val2) \
    do { \
        auto _v1 = (val1); \
        auto _v2 = (val2); \
        if (_v1 >= _v2) { \
            ::minios::core::TestRunner::instance().record_pass(); \
        } else { \
            std::ostringstream _oss; \
            _oss << "Expected " << _v1 << " >= " << _v2; \
            ::minios::core::TestRunner::instance().record_fail(__FILE__, __LINE__, #val1 " >= " #val2, _oss.str()); \
        } \
    } while (false)
