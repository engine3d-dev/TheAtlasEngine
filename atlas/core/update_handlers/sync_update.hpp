#pragma once
#include <core/engine_logger.hpp>
#include <deque>
#include <functional>

namespace atlas {
    /**
     * @name Sync Update Manager (SUM)
     * @note Essentially our way of handling differerent state of updates
     * @note Synchronized container to ensure those different updates are synced
     * correctly
     *
     */

    namespace sync_update {

        //! @note Initializes sync update manager to make sure our manager is
        //! able to dispatch our update functions.
        void initialize();

        //! @note Retrieves the local delta time
        float delta_time();

        // Called by thread_manager
        // void run_update(float deltaTime);

        inline std::deque<std::function<void()>> s_update;
        inline std::deque<std::function<void()>> s_late_update;
        inline std::deque<std::function<void()>> s_ui_update;
        inline std::deque<std::function<void()>> s_physica_update;

        //! @note Synchronize our update per frame
        template<typename UObject, typename UFunction>
        void sync(UObject* p_instance, const UFunction& p_callable) {
            static_assert(std::is_member_pointer_v<UFunction>,
                          "Invalid sync function");

            s_update.push_back(
              [p_instance, p_callable]() { (p_instance->*p_callable)(); });
        }

        template<typename UObject, typename UFunction>
        void sync_physics(UObject* p_instance, const UFunction& p_callable) {
            static_assert(std::is_member_pointer_v<UFunction>,
                          "Invalid sync function");

            s_physica_update.push_back(
              [p_instance, p_callable]() { (p_instance->*p_callable)(); });
        }

        //! @note Post update - newly defined late update
        template<typename UObject, typename UCallable>
        inline void post(UObject* p_instance, const UCallable& p_callable) {
            static_assert(std::is_member_pointer_v<UCallable>,
                          "Invalid post function");

            s_late_update.push_back(
              [p_instance, p_callable]() { (p_instance->*p_callable)(); });
        }

        //! @note Attach our UI Updates
        template<typename UObject, typename UCallable>
        void attach(UObject* p_instance, const UCallable& p_callable) {
            // UI returns a boolean because this can be used to indicate whether
            // specific callable functions are activated
            //! @note NOTE: This is just for testing...
            static_assert(std::is_member_pointer_v<UCallable>,
                          "Invalid attach function");

            s_ui_update.push_back(
              [p_instance, p_callable]() { (p_instance->*p_callable)(); });
        }

        void on_update();

        void on_physics_update();

        void on_ui_update();

        void on_late_update();

    };

    //! @note These are public functions that users can utilize
    //! @note If users are using anything within the sync_update namespace. They
    //! shouldn't
    template<typename UObject, typename UCallable>
    void sync(UObject* p_instance, const UCallable& p_callable) {
        sync_update::sync(p_instance, p_callable);
    }

    template<typename UObject, typename UCallable>
    void sync_physics(UObject* p_instance, const UCallable& p_callable) {
        sync_update::sync_physics(p_instance, p_callable);
    }

    template<typename UObject, typename UCallable>
    void post(UObject* p_instance, const UCallable& p_callable) {
        sync_update::post(p_instance, p_callable);
    }

    template<typename UObject, typename UCallable>
    void attach(UObject* p_instance, const UCallable& p_callable) {
        sync_update::attach(p_instance, p_callable);
    }

};