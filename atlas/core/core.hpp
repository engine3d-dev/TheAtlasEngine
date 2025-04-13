#pragma once
#include <cassert>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

namespace atlas {
#ifdef ENGINE_PLATFORM_WINDOWS
#ifdef ENGINE_BUILD_DLL
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#else // If we are on either Linux/Mac OS, since they use __attributes__
#define ENGINE_API __attribute__((visibility("default")))
#endif

#ifdef ATLAS_DEBUG_BUILD
#define ATLAS_DEBUG_BUILD
#endif

    template<typename... T>
    static void core_assert(bool x, T&&... args) {
        // const char* filename = __FILE__;
        // auto line = __LINE__;
        if (!x) {
            // spdlog::critical("assertion: {}", fmt, std::forward<T>(args)...,
            // filename, line);
            spdlog::critical("assertion failed: {}", std::forward<T>(args)...);
        }
    }

    //! @note Some aliases for the smart pointers
    template<typename T>
    using ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    constexpr ref<T> create_ref(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using scope = std::unique_ptr<T>;

    template<typename T, typename... Args>
    constexpr scope<T> create_scope(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using weak_ptr = std::weak_ptr<T>;

    template<typename T, typename... Args>
    constexpr weak_ptr<T> create_weak_ptr(Args&&... args) {
        return std::weak_ptr<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename U>
    weak_ptr<T> to_weak_ptr(const U& p_value) {
        return CreateWeakPtr<T>(p_value);
    }
};