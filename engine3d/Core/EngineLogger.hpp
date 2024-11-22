#pragma once
#include <memory>
#include <Core/Core.hpp>
#include <fmt/os.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

namespace engine3d{
    /**
     * @name ConsoleEngineLogger
     * @note Engine3D's console logger to write to the console instead of specifying the UI-type of widget to upload logging information to
     * @note TODO --- Adding a way to indicate to the logger to supply a type of ui identifier
     * @note Reason - is if users wanted have a way of knowing to t
    */
    class ConsoleEngineLogger{
    public:
        //! @note Used for initiating this console logger across engine3d supplying the pattern and application to dedicate log messages to
        static void InitializeConsoleLogger(const std::string name, const std::string& pattern="%^[%T] %n: %v%$");

        static Ref<spdlog::logger> GetLoggerInstance();
    private:
        static Ref<spdlog::logger> m_ConsoleLogger;
    };
};


//! @note Console Loggers (These are loggers that write specifically to the console, terminal console)
//! @note TODO --- Specify that engine3d will have it's own console terminal that these will be written to.
template<typename... T>
inline void ConsoleLogTrace(spdlog::format_string_t<T...> fmt, T&&... args){
    engine3d::ConsoleEngineLogger::GetLoggerInstance()->trace(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ConsoleLogWarn(spdlog::format_string_t<T...> fmt, T&&... args){
    engine3d::ConsoleEngineLogger::GetLoggerInstance()->warn(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ConsoleLogInfo(spdlog::format_string_t<T...> fmt, T&&... args){
    engine3d::ConsoleEngineLogger::GetLoggerInstance()->info(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ConsoleLogError(spdlog::format_string_t<T...> fmt, T&&... args){
    engine3d::ConsoleEngineLogger::GetLoggerInstance()->error(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ConsoleLogFatal(spdlog::format_string_t<T...> fmt, T&&... args){
    engine3d::ConsoleEngineLogger::GetLoggerInstance()->critical(fmt, std::forward<T>(args)...);
}