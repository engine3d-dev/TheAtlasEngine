#include <Core/EngineLogger.hpp>
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
            spdlog::log(spdlog::level::critical, "ConsoleEngineLogger WAS NOT initialized! (Should not hit this statement at all)\n");
            spdlog::log(spdlog::level::critical, "Founded at {0} and on line {1}", __FILE__, __LINE__);
            std::terminate();
        }
        
        return m_ConsoleLogger;
    }
};