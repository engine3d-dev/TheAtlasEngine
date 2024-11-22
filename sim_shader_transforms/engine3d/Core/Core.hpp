#pragma once
#include <string>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <cassert>

namespace engine3d{
    
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
    constexpr uint16_t bit(uint16_t x){
        return (1 << x);
    }

    template<typename... T>
    static void Render_Core_Assert(bool x, const T&&... args){
        assert(x);
    }

    constexpr void engine_assert(bool p_Exp, const std::string& p_DebugMsg){
        if(!p_Exp){
            printf("%s", p_DebugMsg.c_str());
            assert(false);
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
        static constexpr auto check(T*)  -> typename std::is_same<
                decltype(std::declval<T>().PushOverlay(std::declval<Args>()...)),
                Ret    // Ensure the return type matches
            >::type;
        template<typename>
        static constexpr std::false_type check(...);
        typedef decltype(check<C>(0)) type;
    public:
        static constexpr bool value = type::value;
    };
    template<typename T>
    inline constexpr bool member_bound_function_check_v = member_bound_function_check<T, void()>::value;



    //! @note Quick implementation similar to std::bind
    //! @note Inefficiency's with std::bind and how it also uses more ram.
    // template<typename T, typename BoundFunc, typename = std::enable_if_t<member_bound_function_check_v<BoundFunc>>>
    // static auto bind(T* instance, BoundFunc&& member_bound_function){
    //     return [instance, member_bound_function](auto&& arg1){
    //         return (instance->*member_bound_function)(std::forward<decltype(arg1)>(arg1));
    //     };
    // }
    // template<typename T, typename BoundFunction, typename = std::enable_if_t<member_bound_function_check_v<BoundFunction>>>
    template<typename T, typename BoundFunction>
    static auto bind(T* instance, BoundFunction&& member_bound_function){
        return [instance, member_bound_function](auto&& params){
            return (instance->*member_bound_function)(std::forward<decltype(params)>(params));
        };
    }

    template<typename T, typename BoundFunc>
    static auto bind2(T* instance, BoundFunc member_bound_function){
        // static_assert(!std::is_member_pointer<decltype(member_bound_function)>::value, "Not a member function of that instance");

        // std::cout << std::boolalpha << is_member_function<T, BoundFunc> << '\n';
        return [instance, member_bound_function](auto&& params){
            return (instance->*member_bound_function)(std::forward<decltype(params)>(params));
        };
    }

    //! @note Some aliases for the smart pointers
    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args){
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args){
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
};