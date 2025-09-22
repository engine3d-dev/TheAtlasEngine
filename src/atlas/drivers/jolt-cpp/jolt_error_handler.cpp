#include <engine_logger.hpp>
#include <Jolt/Jolt.h>
#include <cstdarg>

namespace atlas::physics {

    void trace_impl(const char* p_in_fmt, ...) {
        va_list list;
        va_start(list, p_in_fmt);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), p_in_fmt, list);
        va_end(list);
        console_log_error("{}", buffer);
    }

    bool assert_failed_impl(const char* p_in_expression,
                            const char* p_in_message,
                            const char* p_in_file,
                            unsigned int p_in_line) {

        console_log_error("{}:{}: ({}) {}",
                          p_in_file,
                          p_in_line,
                          p_in_expression,
                          (p_in_message != nullptr ? p_in_message : ""));

        return true;
    };
}