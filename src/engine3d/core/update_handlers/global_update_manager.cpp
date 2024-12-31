#include "thread_utils/thread_utils.hpp"
#include <core/update_handlers/global_update_manager.hpp>
#include <core/application_instance.hpp>
#include <core/engine_logger.hpp>
#include <core/update_handlers/sync_update_manager.hpp>
#include <chrono>
#include <print>
#include <core/event/key_event.hpp>
#include <core/event/input_poll.hpp>
#include <core/engine_logger.hpp>
#include <condition_variable>

namespace engine3d{
    static float s_GlobalDeltaTime = 0.0f;
    Scope<ThreadManager> s_threadManager = nullptr;
    static std::chrono::time_point<std::chrono::high_resolution_clock> s_UpdateTimer;
    Timer s_GlobalTimer;
    Timer s_FrameratePerSecondMaintainTimer;
    static float s_MaxFrameratePerSecond = 0.0f;
    static float s_FrameratePerSecondCounter = 0.0f;
    static constexpr int SECONDS = 1000000;
    float g_DeltaTime = 0.0f;

    // this needs to be called every frame?
    // std::atomic<uint32_t> g_ThreadCounter;
    // std::mutex g_GlobalLock;
    // std::condition_variable g_GlobalConditional; 

    std::vector<std::function<void()>> GlobalUpdateManager::s_ApplicationUpdateSubscribers;

    void GlobalUpdateManager::Initialize(){
        s_GlobalTimer = Timer();
        s_FrameratePerSecondMaintainTimer = Timer();
        s_threadManager = CreateScope<ThreadManager>();

        s_GlobalDeltaTime = 0.0f;
        s_UpdateTimer = s_GlobalTimer.GetCurrentTime();
        s_MaxFrameratePerSecond = 100;
        s_FrameratePerSecondCounter = 1;
        s_ApplicationUpdateSubscribers = std::vector<std::function<void()>>();
        ConsoleLogInfo("F1 to see global time and F2 to see local time");
    }

    void GlobalUpdateManager::GlobalOnTickUpdate(){
        s_FrameratePerSecondMaintainTimer.Reset();

        for(const auto& app_update : s_ApplicationUpdateSubscribers){
            app_update();
        }

        if(s_GlobalTimer.ElapsedSec() >= 1){
            
            if(InputPoll::IsKeyPressed(KeyCode::F1)){
                ConsoleLogInfo("FPS: {1} Delta Time: {0}", s_GlobalDeltaTime, s_FrameratePerSecondCounter);
            }

            s_GlobalTimer.Reset();
            s_FrameratePerSecondCounter = 1;
        }
        else{
            s_FrameratePerSecondCounter++;
        }

        s_GlobalDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(s_GlobalTimer.GetCurrentTime() - s_UpdateTimer).count();
        s_UpdateTimer = s_GlobalTimer.GetCurrentTime();
        g_DeltaTime = s_GlobalDeltaTime / SECONDS;
        
        s_threadManager->OnRun(g_DeltaTime);
        // SyncUpdateManager::RunUpdate(g_DeltaTime);
        WaitForNextFrame();
    }

    void GlobalUpdateManager::IncrementCounter(){
        // g_ThreadCounter |= 1;
    }


    // Maintains a const fps if possible
    void GlobalUpdateManager::WaitForNextFrame(){
        
        while(s_FrameratePerSecondMaintainTimer.ElapsedSec() < 1.0f/s_MaxFrameratePerSecond){
            continue;
        }

        
        // g_ThreadCounter += 1;
        // std::unique_lock<std::mutex> m(g_GlobalLock);
        // g_GlobalConditional.wait(m, [](){});

        // while(g_ThreadCounter < (GetThreadCount()/2) + 1){
        //     continue;
        // }

        // std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1.0f/s_MaxFrameratePerSecond - s_FrameratePerSecondMaintainTimer.ElapsedSec()));
    }

    void GlobalUpdateManager::GlobalCleanup(){
        ConsoleLogWarn("Global Update Manager is cleaning up!");
        s_threadManager->OnStop();
    }
};



/*

[ApplicationManager] x
    -> [Game Object Manager] x
        -> UUID.cpp
-> Scene.cpp


[Time Management] x
    -> Update Managers x

[Scene Management] x
    -> [Components] x
        -> [SPComponents] x
    -> [Scene Objects] x



NEW VERSION

[core]
    -> UUID.cpp

[scene/]
    -> [Components]
        -> [ All Components ]
    -> Scene Renderer
    -> Scene Object

*/