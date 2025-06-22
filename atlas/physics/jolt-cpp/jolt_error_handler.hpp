#pragma once
#include <physics/jolt-cpp/jolt-imports.hpp>

/**
 * @brief Handles error handling with in jolt.
 *
 * @note FIXME: Should have a custom definition for how we want to handle and
 * print JPH_ASSERTS().
 */

namespace atlas::physics {

    /**
     * @brief Enforced by JPH to handle assert and give users and ourselves a
     * way to track where those asserts are coming from in Jolt.
     *
     */
    void trace_impl(const char* p_in_fmt, ...);

    /**
     * @brief Allows for custom desciptions and messages when Asserts fail
     *
     * @param p_in_expression The failing arguement
     * @param p_in_message The message provided
     * @param p_in_file The file the assert is in
     * @param p_in_line The line the assert is on
     * @return true
     * @return false
     */
    bool assert_failed_impl(const char* p_in_expression,
                            const char* p_in_message,
                            const char* p_in_file,
                            unsigned int p_in_line);
}