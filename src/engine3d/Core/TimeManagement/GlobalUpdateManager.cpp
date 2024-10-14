#include "TimeManagement/UpdateManagers/SyncUpdateManager.hpp"
#include <Core/Event/InputPoll.hpp>
#include <Core/TimeManagement/GlobalUpdateManager.hpp>
#include <chrono>
#include <print>
#include <Core/EngineLogger.hpp>

using namespace std::chrono;

using highResClock = high_resolution_clock;
namespace engine3d
{
    float n = 0;
    static float g_globalDeltaTime = 0;
    GlobalUpdateManager::GlobalUpdateManager()
    {
        OnSetUp();
    }

    void GlobalUpdateManager::OnSetUp()
    {
        m_GlobalTimer = new Timer();
        m_FPSMaintain = new Timer();

        m_GlobalDeltaTime = 0.0;
        m_UpdateTime = m_GlobalTimer->getCurrentTime();
        m_MaxFPS = 80;
        counter = 0;

        m_keyEvent = new InputPoll();
        t_syncUpdate = new SyncUpdateManager();

        ConsoleLogInfo("F1 to see Global time and F2 to see Local time!\n");
        
    }

    void GlobalUpdateManager::globalOnTickUpdate()
    {
        m_FPSMaintain->reset();
        t_syncUpdate->runUpdate(m_GlobalDeltaTime);

        if(m_GlobalTimer->elapsedSec() >= 1)
        {
            if(m_keyEvent->IsKeyPressed(KeyCode::F1))
            {
                ConsoleLogInfo("FPS: {1} Delta Time: {0}",
                    m_GlobalDeltaTime, counter);
            }
            m_GlobalTimer->reset();
            counter = 0;
        }
        else 
        {
            counter++;
        }

        m_GlobalDeltaTime = duration_cast<microseconds>(
                m_GlobalTimer->getCurrentTime() - m_UpdateTime).count();

        m_UpdateTime = m_GlobalTimer->getCurrentTime();

        waitForNextFrame();
    }

    void GlobalUpdateManager::waitForNextFrame()
    {

        while(m_FPSMaintain->elapsedSec() < 1.0/m_MaxFPS)
        {
            continue;
        }
    }


     GlobalUpdateManager::~GlobalUpdateManager()
     {
        delete m_GlobalTimer;
        delete m_FPSMaintain;
        delete m_keyEvent;

        delete t_syncUpdate;
     }

     //paraOnFrameManager
     //paraOnFrameUpdate
     //paraOnCallUpdate
}