#include "ApplicationInstance.hpp"
// #include "Physics/JoltHandler.hpp"
#include <Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include <Core/Event/InputPoll.hpp>
#include <Core/TimeManagement/GlobalUpdateManager.hpp>
#include <chrono>
#include <print>
#include <Core/EngineLogger.hpp>
#include <GLFW/glfw3.h>

using namespace JPH;
using namespace JPH::literals;

using namespace std::chrono;

#define TOSECONDS  1000000

using highResClock = high_resolution_clock;
namespace engine3d
{
    // std::vector<std::function<void()>> GlobalUpdateManager::m_ApplicationUpdateSubscribers;

    GlobalUpdateManager::GlobalUpdateManager()
    {
        OnSetUp();
    }

    void GlobalUpdateManager::OnSetUp()
    {
        m_GlobalTimer = new Timer();
        m_FPSMaintain = new Timer();
        m_threadManager = new ThreadMngr();

        m_GlobalDeltaTime = 0.0;
        m_UpdateTime = m_GlobalTimer->GetCurrentTime();
        m_MaxFPS = 90;
        m_FPSCounter = 1;

        //! @note syncupdatemanager will not work until the thread manager works.
        ConsoleLogInfo("F1 to see Global time and F2 to see Local time!\n");
        
    }

    void GlobalUpdateManager::GlobalOnTickUpdate()
    {
        m_FPSMaintain->Reset();
        for(const auto& appUpdate : m_ApplicationUpdateSubscribers){
            appUpdate();
        }

        if(m_GlobalTimer->ElapsedSec() >= 1)
        {
            if(InputPoll::IsKeyPressed(KeyCode::F1))
            {
                ConsoleLogInfo("FPS: {1} Delta Time: {0}",
                    m_GlobalDeltaTime, m_FPSCounter);
            }
            m_GlobalTimer->Reset();
            m_FPSCounter = 1;
        }
        else 
        {
            m_FPSCounter++;
        }

        if(InputPoll::IsKeyPressed(KeyCode::F12))
        {
            glfwDestroyWindow(ApplicationInstance::GetWindow().GetNativeWindow()); 
        }

        m_GlobalDeltaTime = duration_cast<microseconds>(
                m_GlobalTimer->GetCurrentTime() - m_UpdateTime).count();

        m_UpdateTime = m_GlobalTimer->GetCurrentTime();

        m_threadManager->OnRun(m_GlobalDeltaTime / TOSECONDS);

        WaitForNextFrame();
    }

    void GlobalUpdateManager::WaitForNextFrame()
    {
        // if(m_FPSMaintain->ElapsedSec() < 1.0/m_MaxFPS){
        //     std::this_thread::sleep_until(std::chrono::time_point<highResClock>());
        // }

        //! @note We'll continue this in a bit.... finish later.
        //! @note Technically should be notify by Sync update manager.
        //! @note Example: SyncUpdateManager::NotifyAll();
        while(m_FPSMaintain->ElapsedSec() < 1.0/m_MaxFPS)
        {
            continue;
        }
    }

    GlobalUpdateManager::~GlobalUpdateManager()
    {
        delete m_GlobalTimer;
        delete m_FPSMaintain;
        delete m_threadManager;
    }
}
