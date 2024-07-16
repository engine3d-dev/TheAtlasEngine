#pragma once
#include <memory>
#include <engine3d/Core/Core.h>
// #include <spdlog/spdlog.h>
// #include <spdlog/fmt/ostr.h>
#include "fmt/os.h"
#include "fmt/ostream.h"
#include "spdlog/spdlog.h"

namespace engine3d{
    class ENGINE_API EngineLogger{
    public:
        EngineLogger();
        ~EngineLogger();

        static void Initialize();

        // If either CoreLogger or client is nullptr
        // Then we should automatically terminate the app.
        // This is because if we continue, we will segfault due to the CoreLogger/ClientLogger not being initialized
        static bool isLoggerInit() {
            return (coreLogger == nullptr || clientLogger == nullptr);
        }

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger(){
            if(EngineLogger::isLoggerInit()){
                // spdlog::fatal("EngineLogger::Init() needs to be called!\nEngineLogger::Init() returns a nullptr");
                spdlog::log(spdlog::level::critical, "EngineLogger::Init() needs to be called!\n");
                std::terminate();
            }
            return coreLogger;
        }

        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
            if(EngineLogger::isLoggerInit()){
                // Logger::fatal("EngineLogger::Init() needs to be called!\n");
                spdlog::log(spdlog::level::critical, "EngineLogger::Init() needs to be called!\n");
                std::terminate();
            }
            
            return clientLogger;
        }

    private:
        // static std::shared_ptr<Logger::Log> CoreLogger;
        // static std::shared_ptr<Logger::Log> ClientLogger;
        static std::shared_ptr<spdlog::logger> coreLogger;
        static std::shared_ptr<spdlog::logger> clientLogger;
    };
};


// ------------ Core logs ------------

template<typename... T>
inline void CoreLogTrace(spdlog::format_string_t<T...> fmt, T&&... args) {
    engine3d::EngineLogger::GetCoreLogger()->trace(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void CoreLogInfo(spdlog::format_string_t<T...> fmt, T&&... args) {
    engine3d::EngineLogger::GetCoreLogger()->info(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void CoreLogWarn(spdlog::format_string_t<T...> fmt, T &&...args) {
    engine3d::EngineLogger::GetCoreLogger()->warn(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void CoreLogError(spdlog::format_string_t<T...> fmt, T &&...args) {
    engine3d::EngineLogger::GetCoreLogger()->error(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void CoreLogFatal(spdlog::format_string_t<T...> fmt, T &&...args) {
    // engine3d::EngineLogger::GetCoreLogger()->fatal(fmt, std::forward<T>(args)...);
    engine3d::EngineLogger::GetCoreLogger()->critical(fmt, std::forward<T>(args)...);
}

// ------------ Client logs ------------
template<typename... T>
inline void ClientLogTrace(spdlog::format_string_t<T...> fmt, T &&...args) {
    engine3d::EngineLogger::GetClientLogger()->trace(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ClientLogInfo(spdlog::format_string_t<T...> fmt, T &&...args) {
    engine3d::EngineLogger::GetClientLogger()->info(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ClientLogWarn(spdlog::format_string_t<T...> fmt, T &&...args) {
    engine3d::EngineLogger::GetClientLogger()->warn(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ClientLogError(spdlog::format_string_t<T...> fmt, T &&...args) {
    engine3d::EngineLogger::GetClientLogger()->error(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ClientLogFatal(spdlog::format_string_t<T...> fmt, T &&...args) {
    engine3d::EngineLogger::GetClientLogger()->critical(fmt, std::forward<T>(args)...);
}