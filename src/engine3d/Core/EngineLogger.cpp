#include <engine3d/Core/EngineLogger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace engine3d{

    //! @note Console logger
    Ref<spdlog::logger> ConsoleEngineLogger::m_ConsoleLogger;

    void ConsoleEngineLogger::InitializeConsoleLogger(const std::string name, const std::string& pattern){
        spdlog::set_pattern(pattern);
        m_ConsoleLogger = spdlog::stdout_color_mt(name);
        m_ConsoleLogger->set_level(spdlog::level::trace);
    }

    Ref<spdlog::logger> ConsoleEngineLogger::GetLoggerInstance(){
        if(m_ConsoleLogger == nullptr){
            spdlog::log(spdlog::level::critical, "ConsoleEngineLogger was initialized! (Should not hit this statement at all)\n");
            std::terminate();
        }
        
        return m_ConsoleLogger;
    }
};