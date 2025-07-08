#pragma once
#include <cassert>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <core/utilities/memory.hpp>

namespace atlas {

    template<typename... T>
    static void core_assert(bool x, T&&... args) {
        if (!x) {
            spdlog::critical("assertion failed: {}", std::forward<T>(args)...);
        }
    }

    //! @brief alias to atlas::memory::strong_ptr<T>
    template<typename T>
    using strong_ref = memory::strong_ptr<T>;

    //! @return atlas::memory::make_strong_ptr<T> which constructs object of type T
    template<typename T, typename... Args>
    constexpr strong_ref<T> create_strong_ref(std::pmr::polymorphic_allocator<> p_allocator, Args&&... args) {
        return memory::make_strong_ptr<T>(p_allocator, std::forward<Args>(args)...);
    }

    template<typename T>
    using optional_ref = memory::optional_ptr<T>;

    //! @brief alias to memory::optional_ptr<T>
    template<typename T>
    using ref = std::shared_ptr<T>;

    //! @return constructed shared_ptr<T> object
    template<typename T, typename... Args>
    constexpr ref<T> create_ref(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    //! @brief alias to unique_ptr<T>
    template<typename T>
    using scope = std::unique_ptr<T>;

    //! @return make_unique<T> that constructs object
    template<typename T, typename... Args>
    constexpr scope<T> create_scope(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

};