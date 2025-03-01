#include <core/engine_logger.hpp>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace atlas{

    std::string g_CurrentPatternForLogs = "Undefined Pattern Specified";
    std::unordered_map<std::string, ref<spdlog::logger>> console_log_manager::s_loggers;

    void console_log_manager::initialize_logger_manager(const std::string& pattern){
        // spdlog::set_pattern(pattern);
        g_CurrentPatternForLogs = pattern;
        // m_ConsoleLogger = spdlog::stdout_color_mt(name);
        // m_ConsoleLogger->set_level(spdlog::level::trace);
        
        //! @note Setting up logs for different log stdout's
        //! @note Logs for p_tag is logs specific to the game.
        s_loggers.insert({"engine3d", spdlog::stdout_color_mt("engine3d")});
        s_loggers.insert({"physics", spdlog::stdout_color_mt("physics")});
        s_loggers.insert({"vulkan", spdlog::stdout_color_mt("vulkan")});
        s_loggers.insert({"assert", spdlog::stdout_color_mt("core assertion")});
        
        s_loggers["engine3d"]->set_pattern(pattern);
        s_loggers["engine3d"]->set_level(spdlog::level::trace);

        s_loggers["physics"]->set_level(spdlog::level::trace);
        s_loggers["physics"]->set_pattern(pattern);

        s_loggers["vulkan"]->set_level(spdlog::level::trace);
        s_loggers["vulkan"]->set_pattern(pattern);

        s_loggers["assert"]->set_level(spdlog::level::trace);
        s_loggers["assert"]->set_pattern(pattern);
    }

    void console_log_manager::create_new_logger(const std::string& p_tag){
        s_loggers[p_tag] = spdlog::stdout_color_mt(p_tag);
        s_loggers[p_tag]->set_level(spdlog::level::trace);
        s_loggers[p_tag]->set_pattern(g_CurrentPatternForLogs);
    }

    void console_log_manager::set_current_logger(const std::string& p_tag){
        //! @note Setting up logs for different log stdout's
        //! @note Logs for p_tag is logs specific to the game
        s_loggers[p_tag] = spdlog::stdout_color_mt(p_tag);
        s_loggers[p_tag]->set_level(spdlog::level::trace);
        s_loggers[p_tag]->set_pattern(g_CurrentPatternForLogs);
    }

    ref<spdlog::logger> console_log_manager::get(const std::string& p_tag){
        return s_loggers[p_tag];
    }
};