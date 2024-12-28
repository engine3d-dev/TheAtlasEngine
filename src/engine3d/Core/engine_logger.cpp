#include <core/engine_logger.hpp>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace engine3d{

    std::string g_CurrentPatternForLogs = "Undefined Pattern Specified";
    std::unordered_map<std::string, Ref<spdlog::logger>> ConsoleEngineLogger::s_Loggers;

    void ConsoleEngineLogger::InitializeConsoleLogger(const std::string& pattern){
        // spdlog::set_pattern(pattern);
        g_CurrentPatternForLogs = pattern;
        // m_ConsoleLogger = spdlog::stdout_color_mt(name);
        // m_ConsoleLogger->set_level(spdlog::level::trace);
        
        //! @note Setting up logs for different log stdout's
        //! @note Logs for p_Tag is logs specific to the game.
        s_Loggers.insert({"engine3d", spdlog::stdout_color_mt("engine3d")});
        s_Loggers.insert({"core", spdlog::stdout_color_mt("core")});
        s_Loggers.insert({"physics", spdlog::stdout_color_mt("physics")});
        s_Loggers.insert({"vulkan", spdlog::stdout_color_mt("vulkan")});
        
        s_Loggers["engine3d"]->set_pattern(pattern);
        s_Loggers["engine3d"]->set_level(spdlog::level::trace);

        s_Loggers["core"]->set_pattern(pattern);
        s_Loggers["core"]->set_level(spdlog::level::trace);

        s_Loggers["physics"]->set_level(spdlog::level::trace);
        s_Loggers["physics"]->set_pattern(pattern);

        s_Loggers["vulkan"]->set_level(spdlog::level::trace);
        s_Loggers["vulkan"]->set_pattern(pattern);
    }

    void ConsoleEngineLogger::SetCurrentApplicationTagLogger(const std::string& p_Tag){
        //! @note Setting up logs for different log stdout's
        //! @note Logs for p_Tag is logs specific to the game
        s_Loggers[p_Tag] = spdlog::stdout_color_mt(p_Tag);
        s_Loggers[p_Tag]->set_level(spdlog::level::trace);
        s_Loggers[p_Tag]->set_pattern(g_CurrentPatternForLogs);
    }

    Ref<spdlog::logger> ConsoleEngineLogger::GetLogger(const std::string& p_Tag){
        return s_Loggers[p_Tag];
    }
};