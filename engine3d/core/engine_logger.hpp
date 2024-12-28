#pragma once
#include <memory>
#include <map>
#include <core/core.hpp>
#include <fmt/os.h>
#include <fmt/ostream.h>
#include <spdlog/common.h>
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
        static void InitializeConsoleLogger(const std::string& pattern="%^[%T] %n: %v%$");

        static void SetCurrentApplicationTagLogger(const std::string& p_Tag="Undefined g_Tag in console_logger");

        static Ref<spdlog::logger> GetLogger(const std::string& p_Tag);
    private:
        //! @note std::string is the tag of the log's location.
        static std::unordered_map<std::string, Ref<spdlog::logger>> s_Loggers;
    };
};


//! @note Console Loggers (These are loggers that write specifically to the console, terminal console)
//! @note TODO --- Specify that engine3d will have it's own console terminal that these will be written to.
template<typename... T>
inline void ConsoleLogTrace(spdlog::format_string_t<T...> fmt, T&&... args){
    engine3d::ConsoleEngineLogger::GetLogger("engine3d")->trace(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ConsoleLogWarn(spdlog::format_string_t<T...> fmt, T&&... args){
    engine3d::ConsoleEngineLogger::GetLogger("engine3d")->warn(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ConsoleLogInfo(spdlog::format_string_t<T...> fmt, T&&... args){
    engine3d::ConsoleEngineLogger::GetLogger("engine3d")->info(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ConsoleLogError(spdlog::format_string_t<T...> fmt, T&&... args){
    engine3d::ConsoleEngineLogger::GetLogger("engine3d")->error(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ConsoleLogFatal(spdlog::format_string_t<T...> fmt, T&&... args){
    engine3d::ConsoleEngineLogger::GetLogger("engine3d")->critical(fmt, std::forward<T>(args)...);
}

/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////
template<typename... T>
inline void ConsoleLogTraceWithTag(const std::string& p_Tag, spdlog::format_string_t<T...> fmt, T&&... args){
    engine3d::ConsoleEngineLogger::GetLogger(p_Tag)->trace(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ConsoleLogInfoWithTag(const std::string& p_Tag, spdlog::format_string_t<T...> fmt, T&&... args){
    engine3d::ConsoleEngineLogger::GetLogger(p_Tag)->info(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ConsoleLogWarnWithTag(const std::string& p_Tag, spdlog::format_string_t<T...> fmt, T&&... args){
    engine3d::ConsoleEngineLogger::GetLogger(p_Tag)->warn(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ConsoleLogErrorWithTag(const std::string& p_Tag, spdlog::format_string_t<T...> fmt, T&&... args){
    engine3d::ConsoleEngineLogger::GetLogger(p_Tag)->error(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void ConsoleLogFatalWithTag(const std::string& p_Tag, spdlog::format_string_t<T...> fmt, T&&... args){
    engine3d::ConsoleEngineLogger::GetLogger(p_Tag)->critical(fmt, std::forward<T>(args)...);
}