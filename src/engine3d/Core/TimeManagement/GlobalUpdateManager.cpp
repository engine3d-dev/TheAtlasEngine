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
    GlobalUpdateManager::GlobalUpdateManager()
    {
        OnSetUp();
    }

    void GlobalUpdateManager::OnSetUp()
    {
        m_GlobalTimer = new Timer();
        m_FPSMaintain = new Timer();

        m_GlobalDeltaTime = 0.0;
        m_UpdateTime = m_GlobalTimer->GetCurrentTime();
        m_MaxFPS = 80;
        m_FPSCounter = 0;

        m_KeyEvent = new InputPoll();

        //! @note syncupdatemanager will not work until the thread manager works.
        ConsoleLogInfo("F1 to see Global time and F2 to see Local time!\n");
        
    }

    void GlobalUpdateManager::GlobalOnTickUpdate()
    {
        m_FPSMaintain->Reset();

        if(m_GlobalTimer->ElapsedSec() >= 1)
        {
            if(m_KeyEvent->IsKeyPressed(KeyCode::F1))
            {
                ConsoleLogInfo("FPS: {1} Delta Time: {0}",
                    m_GlobalDeltaTime, m_FPSCounter);
            }
            m_GlobalTimer->Reset();
            m_FPSCounter = 0;
        }
        else 
        {
            m_FPSCounter++;
        }

        m_GlobalDeltaTime = duration_cast<microseconds>(
                m_GlobalTimer->GetCurrentTime() - m_UpdateTime).count();

        m_UpdateTime = m_GlobalTimer->GetCurrentTime();

        WaitForNextFrame();
    }

    void GlobalUpdateManager::WaitForNextFrame()
    {

        while(m_FPSMaintain->ElapsedSec() < 1.0/m_MaxFPS)
        {
            continue;
        }
    }

     GlobalUpdateManager::~GlobalUpdateManager()
     {
        delete m_GlobalTimer;
        delete m_FPSMaintain;
        delete m_KeyEvent;
     }
}