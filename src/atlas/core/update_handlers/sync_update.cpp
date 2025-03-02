#include <core/engine_logger.hpp>
#include <core/event/event.hpp>
#include <core/update_handlers/sync_update.hpp>

namespace atlas::sync_update {
    static timer s_Localtimer = timer();
    static std::chrono::time_point<std::chrono::high_resolution_clock>
      s_LocalUpdateTime;
    static float s_SyncLocalDeltaTime = 0.0f;
    static float s_SyncGlobalDeltaTime = 0.0f;
    static int s_MaxVariance = 0;
    static int s_MinFrames = 0;
    static int s_LocalUpdateCounter = 0;
    static int s_LocalFrameratePerSecond = 0;
    static int s_RandomFrame;

    void initialize() {
        console_log_info("InitializeSyncUpdate Initialized!!");
        s_Localtimer = timer();
        s_LocalUpdateTime = s_Localtimer.current_time();
        s_SyncLocalDeltaTime = 0.0;
        s_MaxVariance = 2;
        s_MinFrames = 0;
        s_LocalUpdateCounter = 0;
        s_LocalFrameratePerSecond = 0;
        srand(std::time(NULL));
        s_RandomFrame = (rand() % s_MaxVariance) + s_MinFrames;
    }

    //! @note this does not work per object this might need to change a little.
    //! Possibly pass gameObjects with the virtual functions.
    //! Possibly seperate active scripts to non active ones in scenes.
    void run_update(float deltaTime) {
        //! @note unsafe!!!!
        /**
         * @note Render is not quite placed in the right position here
         * @param randomFrame Used both to determine update and
         * used to allow render to optimise gpu sending if needed
         *
         * @note I used random in this case becuase it is hard for a
         * human to catch wether the render is correct or not.
         * Benchmark later.
         */
        s_SyncGlobalDeltaTime = deltaTime;
        // on_physics_update();
        // const int collisionSteps = 1 + (60*(deltaTime));

        // JoltHandler* p_joltHandler = JoltHandler::GetInstance();
        // Updating physics system based on jolt physics
        // p_joltHandler->physics_system.Update(
        //     deltaTime,
        //     collisionSteps,
        //     p_joltHandler->physics_allocator,
        //     p_joltHandler->job_system);

        if (s_RandomFrame <= s_LocalUpdateCounter) {
            s_RandomFrame = (rand() % s_MaxVariance) + s_MinFrames;
            s_LocalUpdateCounter = 0;

            // on_update();
            // on_updateSub2();
            // on_late_update();

            s_SyncLocalDeltaTime =
              duration_cast<std::chrono::microseconds>(
                s_Localtimer.current_time() - s_LocalUpdateTime)
                .count();
            s_SyncLocalDeltaTime = s_SyncLocalDeltaTime / 1000000;

            s_LocalUpdateTime = s_Localtimer.current_time();
            s_LocalFrameratePerSecond++;
        }
        else {
            s_LocalUpdateCounter++;
        }
        //! @note I don't understand why this works but localtime doesn't.
        //! @note My guess is that it needs to be called every frame/ double
        //! buffering might be an issue.
        // on_scene_render();
        if (s_Localtimer.seconds() >= 1.0) {
            s_Localtimer.reset();

            //! @note Key event added to allow switch between global and local.
            if (event::is_key_pressed(KeyCode::K)) {
                console_log_info("Local FPS: {0}, Local Delta Time: {1}",
                                 s_LocalFrameratePerSecond,
                                 s_SyncLocalDeltaTime);
            }
            s_LocalFrameratePerSecond = 0;
        }
    }

    // void on_physics_update()
    // {
    //     // for(auto& l_Subscriber : s_SyncOnTickUpdateSubscribers)
    //     // {
    //     //     l_Subscriber();
    //     // }
    // }

    void on_update() {

        // console_log_trace("on_update Called for
        // std::deque<std::function<void()>>!!!");
        for (auto& l_Subscriber : s_Update) {
            l_Subscriber();
        }
    }

    void on_ui_update() {

        for (auto& uiUpdate : s_UIUpdate) {
            uiUpdate();
        }
    }

    void on_late_update() {
        for (auto& l_Subscriber : s_LateUpdate) {
            l_Subscriber();
        }
    }

    void on_scene_render() {
        for (auto& l_Subscriber : s_RenderQueue) {
            l_Subscriber();
        }
    }

    void on_physics_update() {
        for (auto& l_Subscriber : s_PhysicsQueue) {
            l_Subscriber();
        }
    }

    float delta_time() {
        return s_SyncLocalDeltaTime;
    }

};