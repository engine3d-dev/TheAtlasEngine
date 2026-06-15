module;

#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_map>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>
#include <print>

export module atlas.core.utilities:logger;
import :common;

export namespace atlas {
    /**
     * @brief logger for logging messages to stdout on the console
     *
     * Manages the logger instances and patterns for setting up with the loggers
     * when writing logs
     */
    class console_log_manager {
    public:
        console_log_manager(
          const std::string& p_pattern = "%^[%n] [%T]: %v%$") {
            std::println("Constructing console_log_manager!");

            //! @note Setting up logs for different log stdout's
            //! @note Logs for p_tag is logs specific to the game.
            m_loggers.insert({ "atlas", spdlog::stdout_color_mt("atlas") });
            m_loggers.insert({ "physics", spdlog::stdout_color_mt("physics") });
            m_loggers.insert({ "vulkan", spdlog::stdout_color_mt("vulkan") });
            m_loggers.insert(
              { "assert", spdlog::stdout_color_mt("core assertion") });

            m_loggers["atlas"]->set_pattern(p_pattern);
            m_loggers["atlas"]->set_level(spdlog::level::trace);

            m_loggers["physics"]->set_level(spdlog::level::trace);
            m_loggers["physics"]->set_pattern(p_pattern);

            m_loggers["vulkan"]->set_level(spdlog::level::trace);
            m_loggers["vulkan"]->set_pattern(p_pattern);

            m_loggers["assert"]->set_level(spdlog::level::trace);
            m_loggers["assert"]->set_pattern(p_pattern);
            s_instance = this;
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
            return s_instance->m_loggers[p_tag];
        }

    private:
        static console_log_manager* s_instance;
        // Using an unordered_map to specify through a string what logger to
        // retrieve to log messages
        std::unordered_map<std::string, std::shared_ptr<spdlog::logger>>
          m_loggers;
    };

    // std::unordered_map<std::string, ref<spdlog::logger>>
    // console_log_manager::s_loggers;
    console_log_manager* console_log_manager::s_instance = nullptr;
};

export {
    //! @note Console Loggers (These are loggers that write specifically to the
    //! console, terminal console)
    template<typename... T>
    inline void console_log_trace(
      [[maybe_unused]] spdlog::format_string_t<T...> fmt,
      [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
        atlas::console_log_manager::get("atlas")->trace(
          fmt, std::forward<T>(args)...);
#endif
    }

    template<typename... T>
    inline void console_log_warn(
      [[maybe_unused]] spdlog::format_string_t<T...> fmt,
      [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
        atlas::console_log_manager::get("atlas")->warn(
          fmt, std::forward<T>(args)...);
#endif
    }

    template<typename... T>
    inline void console_log_info(
      [[maybe_unused]] spdlog::format_string_t<T...> fmt,
      [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
        atlas::console_log_manager::get("atlas")->info(
          fmt, std::forward<T>(args)...);
#endif
    }

    template<typename... T>
    inline void console_log_error(
      [[maybe_unused]] spdlog::format_string_t<T...> fmt,
      [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
        atlas::console_log_manager::get("atlas")->error(
          fmt, std::forward<T>(args)...);
#endif
    }

    template<typename... T>
    inline void console_log_fatal(
      [[maybe_unused]] spdlog::format_string_t<T...> fmt,
      [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
        atlas::console_log_manager::get("atlas")->critical(
          fmt, std::forward<T>(args)...);
#endif
    }

    template<typename... T>
    inline void console_log_trace_tagged(
      [[maybe_unused]] const std::string& p_tag,
      [[maybe_unused]] spdlog::format_string_t<T...> fmt,
      [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
        atlas::console_log_manager::get(p_tag)->trace(fmt,
                                                      std::forward<T>(args)...);
#endif
    }

    template<typename... T>
    inline void console_log_info_tagged(
      [[maybe_unused]] const std::string& p_tag,
      [[maybe_unused]] spdlog::format_string_t<T...> fmt,
      [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
        atlas::console_log_manager::get(p_tag)->info(fmt,
                                                     std::forward<T>(args)...);
#endif
    }

    template<typename... T>
    inline void console_log_warn_tagged(
      [[maybe_unused]] const std::string& p_tag,
      [[maybe_unused]] spdlog::format_string_t<T...> fmt,
      [[maybe_unused]] T&&... args) {
        atlas::console_log_manager::get(p_tag)->warn(fmt,
                                                     std::forward<T>(args)...);
    }

    template<typename... T>
    inline void console_log_error_tagged(
      [[maybe_unused]] const std::string& p_tag,
      [[maybe_unused]] spdlog::format_string_t<T...> fmt,
      [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
        atlas::console_log_manager::get(p_tag)->error(fmt,
                                                      std::forward<T>(args)...);
#endif
    }

    template<typename... T>
    inline void console_log_fatal_tagged(
      [[maybe_unused]] const std::string& p_tag,
      [[maybe_unused]] spdlog::format_string_t<T...> fmt,
      [[maybe_unused]] T&&... args) {
#ifndef ENABLE_TESTS_ONLY
        atlas::console_log_manager::get(p_tag)->critical(
          fmt, std::forward<T>(args)...);
#endif
    }
}