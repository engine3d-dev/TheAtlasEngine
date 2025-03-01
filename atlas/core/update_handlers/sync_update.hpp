#pragma once
#include <core/engine_logger.hpp>
#include <core/update_handlers/timer.hpp>
#include <functional>
#include <deque>

namespace atlas{
    /**
     * @name Sync Update Manager (SUM)
     * @note Essentially our way of handling differerent state of updates
     * @note Synchronized container to ensure those different updates are synced correctly 
     * 
    */
   
    namespace sync_update{

        //! @note Initializes sync update manager to make sure our manager is able to dispatch our update functions.
        void initialize();

        //! @note Retrieves the local delta time
        float delta_time();

        // Called by thread_manager
        void run_update(float deltaTime);

        inline std::deque<std::function<void()>> s_Update;
        inline std::deque<std::function<void()>> s_LateUpdate;
        inline std::deque<std::function<void()>> s_UIUpdate;
        inline std::deque<std::function<void()>> s_RenderQueue;
        inline std::deque<std::function<void()>> s_PhysicsQueue;

        //! @note Synchronize our update per frame
        template<typename UObject, typename UFunction>
        void sync(UObject* p_instance, const UFunction& p_callable){
            static_assert(std::is_member_pointer_v<UFunction>, "Invalid sync function");
            
            s_Update.push_back([p_instance, p_callable]() {
                (p_instance->*p_callable)();
            });
        }


        template<typename UObject, typename UFunction>
        void sync_physics(UObject* p_instance, const UFunction& p_callable){
            static_assert(std::is_member_pointer_v<UFunction>, "Invalid sync function");
            
            s_PhysicsQueue.push_back([p_instance, p_callable]() {
                (p_instance->*p_callable)();
            });
        }


        //! @note Post update - newly defined late update
        template<typename UObject, typename UCallable>
        inline void post(UObject* p_instance, const UCallable& p_callable) {
            static_assert(std::is_member_pointer_v<UCallable>, "Invalid post function");

            s_LateUpdate.push_back([p_instance, p_callable]() {
                (p_instance->*p_callable)();
            });
        }

        //! @note Submitting our draw calls to be rendered
        //! TODO: Should move this away from the user
        template<typename UObject, typename UCallable>
        inline void submit(UObject* p_instance, const UCallable& p_callable) {
            static_assert(std::is_member_pointer_v<UCallable>, "Invalid submit function");
            s_RenderQueue.push_back([p_instance, p_callable]() {
                (p_instance->*p_callable)();
            });
        }

        //! @note Attach our UI Updates
        template<typename UObject, typename UCallable>
        void attach(UObject* p_instance, const UCallable& p_callable){
            // UI returns a boolean because this can be used to indicate whether specific callable functions are activated
            //! @note NOTE: This is just for testing...
            static_assert(std::is_member_pointer_v<UCallable>, "Invalid attach function");
            
            s_UIUpdate.push_back([p_instance, p_callable](){
                (p_instance->*p_callable)();
            });
        }


        void on_update();

        void on_physics_update();

        void on_ui_update();

        void on_scene_render();

        void on_late_update();
  
    };

    //! @note These are public functions that users can utilize
    //! @note If users are using anything within the sync_update namespace. They shouldn't
    template<typename UObject, typename UCallable>
    void sync(UObject* p_instance, const UCallable& p_callable){
        sync_update::sync(p_instance, p_callable);
    }

    template<typename UObject, typename UCallable>
    void sync_physics(UObject* p_instance, const UCallable& p_callable){
        sync_update::sync_physics(p_instance, p_callable);
    }

    template<typename UObject, typename UCallable>
    void post(UObject* p_instance, const UCallable& p_callable){
        sync_update::post(p_instance, p_callable);
    }

    template<typename UObject, typename UCallable>
    void submit(UObject* p_instance, const UCallable& p_callable){
        sync_update::submit(p_instance, p_callable);
    }


    template<typename UObject, typename UCallable>
    void attach(UObject* p_instance, const UCallable& p_callable){
        sync_update::attach(p_instance, p_callable);
    }



};