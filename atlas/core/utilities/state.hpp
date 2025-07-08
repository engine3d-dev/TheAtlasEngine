#pragma once
#include <functional>
#include <type_traits>

namespace atlas {

    namespace detail {
        //! @brief invokation calls to the state queues
        // anything in the detail namespace should not be invoked by the
        // application code
        void invoke_on_update();
        void invoke_defer_update();
        void invoke_physics_update();
        void invoke_ui_update();
        void invoke_start();

        void poll_update(const std::function<void()>& p_callback);
        void poll_defer_update(const std::function<void()>& p_callback);
        void poll_physics_update(const std::function<void()>& p_callback);
        void poll_ui_update(const std::function<void()>& p_callback);
        void poll_start(const std::function<void()>& p_callback);

        template<typename T, typename = void>
        struct has_member_fn : std::false_type {};

        template<typename T>
        struct has_member_fn<T, std::void_t<decltype(T::foo)>>
          : std::true_type {};
    };

    template<typename UObject, typename UCallback>
    void register_start(UObject* p_instance, const UCallback& p_callable) {
        static_assert(std::is_member_pointer_v<UCallback>,
                      "Cannot register a function that is not a member "
                      "function of a class object");
        detail::poll_start(
          [p_instance, p_callable]() { (p_instance->*p_callable)(); });
    }

    template<typename UObject, typename UCallback>
    void register_update(UObject* p_instance, const UCallback& p_callable) {
        static_assert(std::is_member_pointer_v<UCallback>,
                      "Cannot register a function that is not a member "
                      "function of a class object");
        detail::poll_update(
          [p_instance, p_callable]() { (p_instance->*p_callable)(); });
    }

    template<typename UObject, typename UCallback>
    void register_physics(UObject* p_instance, const UCallback& p_callable) {
        static_assert(std::is_member_pointer_v<UCallback>,
                      "Cannot register a function that is not a member "
                      "function of a class object");
        detail::poll_physics_update(
          [p_instance, p_callable]() { (p_instance->*p_callable)(); });
    }

    template<typename UObject, typename UCallback>
    void register_deferred(UObject* p_instance, const UCallback& p_callable) {
        static_assert(std::is_member_pointer_v<UCallback>,
                      "Cannot register a function that is not a member "
                      "function of a class object");
        detail::poll_defer_update(
          [p_instance, p_callable]() { (p_instance->*p_callable)(); });
    }

    template<typename UObject, typename UCallback>
    void register_ui(UObject* p_instance, const UCallback& p_callable) {
        static_assert(std::is_member_pointer_v<UCallback>,
                      "Cannot register a function that is not a member "
                      "function of a class object");
        detail::poll_ui_update(
          [p_instance, p_callable]() { (p_instance->*p_callable)(); });
    }

};