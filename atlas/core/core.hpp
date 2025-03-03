#pragma once
#include <cassert>
#include <cstdint>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <type_traits>

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

    //! @note Helper function to initialize our bitfields
    constexpr uint16_t bit(uint16_t x) {
        return (1 << x);
    }

    // template<typename... T>
    // static void Render_Core_Assert(bool x, const T&&... args){
    //     // assert(x);
    //     if(x){
    //         console_log_fatal("")
    //         assert(x);
    //     }
    // }

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

    template<typename, typename T>
    struct member_bound_function_check {
        static_assert(
          std::integral_constant<T, false>::value,
          "Second template parameter needs to be of function type.");
    };

    // Specialization that does the actual detection

    template<typename C, typename Ret, typename... Args>
    struct member_bound_function_check<C, Ret(Args...)> {
    private:
        template<typename T>
        static constexpr auto check(T*) -> typename std::is_same<
          decltype(std::declval<T>().PushOverlay(std::declval<Args>()...)),
          Ret // Ensure the return type matches
          >::type;
        template<typename>
        static constexpr std::false_type check(...);
        typedef decltype(check<C>(0)) type;

    public:
        static constexpr bool value = type::value;
    };
    template<typename T>
    inline constexpr bool member_bound_function_check_v =
      member_bound_function_check<T, void()>::value;

    //! @note Quick implementation similar to std::bind
    //! @note Inefficiency's with std::bind and how it also uses more ram.
    // template<typename T, typename BoundFunc, typename =
    // std::enable_if_t<member_bound_function_check_v<BoundFunc>>> static auto
    // bind(T* instance, BoundFunc&& member_bound_function){
    //     return [instance, member_bound_function](auto&& arg1){
    //         return
    //         (instance->*member_bound_function)(std::forward<decltype(arg1)>(arg1));
    //     };
    // }
    // template<typename T, typename BoundFunction, typename =
    // std::enable_if_t<member_bound_function_check_v<BoundFunction>>>
    template<typename T, typename BoundFunction>
    static auto bind(T* instance, BoundFunction&& member_bound_function) {
        return [instance, member_bound_function](auto&& params) {
            return (instance->*member_bound_function)(
              std::forward<decltype(params)>(params));
        };
    }

    template<typename T, typename BoundFunc>
    static auto bind2(T* instance, BoundFunc member_bound_function) {
        // static_assert(!std::is_member_pointer<decltype(member_bound_function)>::value,
        // "Not a member function of that instance");

        // std::cout << std::boolalpha << is_member_function<T, BoundFunc> <<
        // '\n';
        return [instance, member_bound_function](auto&& params) {
            return (instance->*member_bound_function)(
              std::forward<decltype(params)>(params));
        };
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