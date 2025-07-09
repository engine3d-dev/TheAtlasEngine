#pragma once
#include <core/core.hpp>
#include <fmt/os.h>
#include <fmt/ostream.h>
#include <map>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

namespace atlas {
    /**
     * @name console_log_manager
     * @note Engine3D's console logger to write to the console instead of
     * specifying the UI-type of widget to upload logging information to
     * @note TODO --- Adding a way to indicate to the logger to supply a type of
     * ui identifier
     * @note Reason - is if users wanted have a way of knowing to t
     */
    class console_log_manager {
    public:
        //! @note Used for initiating this console logger across engine3d
        //! supplying the pattern and application to dedicate log messages to
        static void initialize_logger_manager(
          const std::string& pattern = "%^[%T] %n: %v%$");

        static void set_current_logger(
          const std::string& p_tag = "Undefined g_Tag in console_logger");
        static void create_new_logger(
          const std::string& p_tag = "Undefined Tag");
        static ref<spdlog::logger> get(const std::string& p_tag);

    private:
        //! @note std::string is the tag of the log's location.
        static std::unordered_map<std::string, ref<spdlog::logger>> s_loggers;
    };
};

//! @note Console Loggers (These are loggers that write specifically to the
//! console, terminal console)
//! @note TODO --- Specify that engine3d will have it's own console terminal
//! that these will be written to.
template<typename... T>
inline void
console_log_trace([[maybe_unused]] spdlog::format_string_t<T...> fmt, [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
    atlas::console_log_manager::get("engine3d")
      ->trace(fmt, std::forward<T>(args)...);
#endif
}

template<typename... T>
inline void
console_log_warn([[maybe_unused]] spdlog::format_string_t<T...> fmt, [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY  
    atlas::console_log_manager::get("engine3d")
      ->warn(fmt, std::forward<T>(args)...);
#endif
}

template<typename... T>
inline void
console_log_info([[maybe_unused]] spdlog::format_string_t<T...> fmt, [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY  
    atlas::console_log_manager::get("engine3d")
      ->info(fmt, std::forward<T>(args)...);
#endif
}

template<typename... T>
inline void
console_log_error([[maybe_unused]] spdlog::format_string_t<T...> fmt, [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
    atlas::console_log_manager::get("engine3d")
      ->error(fmt, std::forward<T>(args)...);
#endif
}

template<typename... T>
inline void
console_log_fatal([[maybe_unused]] spdlog::format_string_t<T...> fmt, [[maybe_unused]] T&&... args) {
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