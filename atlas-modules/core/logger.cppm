module;

#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_map>

#include <spdlog/spdlog.h>
#include <memory>

export module core:logger;
export import :common;

static std::string g_current_pattern_for_logs = "Undefined Pattern Specified";

export namespace atlas {
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
        static void initialize_logger_manager(const std::string& pattern = "%^[%T] %n: %v%$") {
            g_current_pattern_for_logs = pattern;

            //! @note Setting up logs for different log stdout's
            //! @note Logs for p_tag is logs specific to the game.
            // s_loggers.insert({ "engine3d", spdlog::stdout_color_mt("atlas") });
            // s_loggers.insert({ "physics", spdlog::stdout_color_mt("physics") });
            // s_loggers.insert({ "vulkan", spdlog::stdout_color_mt("vulkan") });
            // s_loggers.insert(
            // { "assert", spdlog::stdout_color_mt("core assertion") });

            // s_loggers["engine3d"]->set_pattern(pattern);
            // s_loggers["engine3d"]->set_level(spdlog::level::trace);

            // s_loggers["physics"]->set_level(spdlog::level::trace);
            // s_loggers["physics"]->set_pattern(pattern);

            // s_loggers["vulkan"]->set_level(spdlog::level::trace);
            // s_loggers["vulkan"]->set_pattern(pattern);

            // s_loggers["assert"]->set_level(spdlog::level::trace);
            // s_loggers["assert"]->set_pattern(pattern);
        }

        /**
         * @brief sets what the current logger to write to the console with
         */
        static void set_current_logger(const std::string& p_tag = "Undefined g_Tag in console_logger") {
            #ifndef ENABLE_TESTS_ONLY
                //! @note Setting up logs for different log stdout's
                //! @note Logs for p_tag is logs specific to the game
                // s_loggers[p_tag] = spdlog::stdout_color_mt(p_tag);
                // s_loggers[p_tag]->set_level(spdlog::level::trace);
                // s_loggers[p_tag]->set_pattern(g_current_pattern_for_logs);
            #endif
        }

        /**
         * @brief constructs a new spdlog::logger to write to the console
         */
        static void create_new_logger(const std::string& p_tag = "Undefined Tag") {
            #ifndef ENABLE_TESTS_ONLY
                // s_loggers[p_tag] = spdlog::stdout_color_mt(p_tag);
                // s_loggers[p_tag]->set_level(spdlog::level::trace);
                // s_loggers[p_tag]->set_pattern(g_current_pattern_for_logs);
            #endif
        }

        /**
         * @brief retrieves that specific logger if it has been constructed
         *
         * @return ref<spdlog::logger> if found, otherwise return nullptr
         *
         * TODO: Should have this throw an exception rather then returning
         * nullptr
         */
        static std::shared_ptr<spdlog::logger> get(const std::string& p_tag) {
            return s_loggers[p_tag];
        }

    private:
        // Using an unordered_map to specify through a string what logger to
        // retrieve to log messages
        static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> s_loggers;
    };
};

export {
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
}