#pragma once
#include<physics/jolt-cpp/jolt-imports.hpp>

#ifdef JPH_ENABLE_ASSERTS
#define JPH_LOG(...) console_log_info(__VA_ARGS__)
#else
#define JPH_LOG(...) (void)0
#endif

namespace atlas::physics {

    void trace_impl(const char* p_in_fmt, ...);

    bool assert_failed_impl(const char* p_in_expression,
                            const char* p_in_message,
                            const char* p_in_file,
                            unsigned int p_in_line);
}