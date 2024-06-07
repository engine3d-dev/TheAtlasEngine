#include <engine3d/Core/EngineLogger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Engine3D{
    std::shared_ptr<spdlog::logger> EngineLogger::coreLogger;
    std::shared_ptr<spdlog::logger> EngineLogger::clientLogger;

    void EngineLogger::Initialize(){
        spdlog::set_pattern( "%^[%T] %n: %v%$");
        // coreLogger = std::make_shared<spdlog::logger>("GameEngine");
        coreLogger = spdlog::stdout_color_mt("Engine3D");
        coreLogger->set_level(spdlog::level::trace);

        // clientLogger = std::make_shared<spdlog::logger>("APP");
        clientLogger = spdlog::stdout_color_mt("Application");
        clientLogger->set_level(spdlog::level::trace);
    }

};