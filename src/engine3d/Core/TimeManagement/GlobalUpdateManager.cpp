#include <Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
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

        n=0;

        m_keyEvent = new InputPoll();

        ConsoleLogInfo("F1 to see Global time and F2 to see Local time!\n");
        
    }

    void GlobalUpdateManager::globalOnTickUpdate()
    {
        m_FPSMaintain->reset();

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
        
        //! @note for showcase purposes
        // for(int i = 0; i < 1400; i++)
        // {
        //     for(int j = 0; j < 10000; j++){
        //         n++;
        //     }
        // }

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
     }

     //paraOnFrameManager
     //paraOnFrameUpdate
     //paraOnCallUpdate
}