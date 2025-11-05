#pragma once
#include <core/core.hpp>
#include <fmt/os.h>
#include <fmt/ostream.h>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

namespace atlas {
    /**
     * @brief logger for logging messages to stdout on the console
     *
     * Manages the logger instances and patterns for setting up with the loggers
     * when writing logs
     */
    class console_log_manager {
    public:
        /**
         * @brief initializes the console_log_manager
         *
         * TODO: Revisit the logger and do some refactoring because the way this
         * works should be changed, as I'd prob do this differently now.
         */
        static void initialize_logger_manager(
          const std::string& pattern = "%^[%T] %n: %v%$");

        /**
         * @brief sets what the current logger to write to the console with
         */
        static void set_current_logger(
          const std::string& p_tag = "Undefined g_Tag in console_logger");

        /**
         * @brief constructs a new spdlog::logger to write to the console
         */
        static void create_new_logger(
          const std::string& p_tag = "Undefined Tag");

        /**
         * @brief retrieves that specific logger if it has been constructed
         *
         * @return ref<spdlog::logger> if found, otherwise return nullptr
         *
         * TODO: Should have this throw an exception rather then returning
         * nullptr
         */
        static ref<spdlog::logger> get(const std::string& p_tag);

    private:
        // Using an unordered_map to specify through a string what logger to
        // retrieve to log messages
        static std::unordered_map<std::string, ref<spdlog::logger>> s_loggers;
    };
};

//! @note Console Loggers (These are loggers that write specifically to the
//! console, terminal console)
//! @note TODO --- Specify that engine3d will have it's own console terminal
//! that these will be written to.
template<typename... T>
inline void
console_log_trace([[maybe_unused]] spdlog::format_string_t<T...> fmt,
                  [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
    atlas::console_log_manager::get("engine3d")
      ->trace(fmt, std::forward<T>(args)...);
#endif
}

template<typename... T>
inline void
console_log_warn([[maybe_unused]] spdlog::format_string_t<T...> fmt,
                 [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
    atlas::console_log_manager::get("engine3d")
      ->warn(fmt, std::forward<T>(args)...);
#endif
}

template<typename... T>
inline void
console_log_info([[maybe_unused]] spdlog::format_string_t<T...> fmt,
                 [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
    atlas::console_log_manager::get("engine3d")
      ->info(fmt, std::forward<T>(args)...);
#endif
}

template<typename... T>
inline void
console_log_error([[maybe_unused]] spdlog::format_string_t<T...> fmt,
                  [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
    atlas::console_log_manager::get("engine3d")
      ->error(fmt, std::forward<T>(args)...);
#endif
}

template<typename... T>
inline void
console_log_fatal([[maybe_unused]] spdlog::format_string_t<T...> fmt,
                  [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
    atlas::console_log_manager::get("engine3d")
      ->critical(fmt, std::forward<T>(args)...);
#endif
}

/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////
template<typename... T>
inline void
console_log_trace_tagged([[maybe_unused]] const std::string& p_tag,
                         [[maybe_unused]] spdlog::format_string_t<T...> fmt,
                         [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
    atlas::console_log_manager::get(p_tag)->trace(fmt,
                                                  std::forward<T>(args)...);
#endif
}

template<typename... T>
inline void
console_log_info_tagged([[maybe_unused]] const std::string& p_tag,
                        [[maybe_unused]] spdlog::format_string_t<T...> fmt,
                        [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
    atlas::console_log_manager::get(p_tag)->info(fmt, std::forward<T>(args)...);
#endif
}

template<typename... T>
inline void
console_log_warn_tagged([[maybe_unused]] const std::string& p_tag,
                        [[maybe_unused]] spdlog::format_string_t<T...> fmt,
                        [[maybe_unused]] T&&... args) {
    atlas::console_log_manager::get(p_tag)->warn(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void
console_log_error_tagged([[maybe_unused]] const std::string& p_tag,
                         [[maybe_unused]] spdlog::format_string_t<T...> fmt,
                         [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
    atlas::console_log_manager::get(p_tag)->error(fmt,
                                                  std::forward<T>(args)...);
#endif
}

template<typename... T>
inline void
console_log_fatal_tagged([[maybe_unused]] const std::string& p_tag,
                         [[maybe_unused]] spdlog::format_string_t<T...> fmt,
                         [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
    atlas::console_log_manager::get(p_tag)->critical(fmt,
                                                     std::forward<T>(args)...);
#endif
}