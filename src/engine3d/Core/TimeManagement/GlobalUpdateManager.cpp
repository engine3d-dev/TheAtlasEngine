#include <Core/TimeManagement/GlobalUpdateManager.hpp>
#include <chrono>
#include <print>
#include <Core/EngineLogger.hpp>

using namespace std::chrono;

using highResClock = high_resolution_clock;
namespace engine3d
{

    static float g_globalDeltaTime = 0;
    GlobalUpdateManager::GlobalUpdateManager()
    {
        syncUpdateManger = new SyncUpdateManager();
        frameUpdateManager = new ParallelFrameUpdateManager();
        m_GlobalTimer = new Timer();
        m_GlobalDeltaTime = 0.0;
        m_UpdateTime = m_GlobalTimer->getCurrentTime();
        counter = 0;
    }

    void GlobalUpdateManager::globalOnTickUpdate()
    {

        if(m_GlobalTimer->elapsedSec() >= 1)
        {
            ConsoleLogInfo("FPS: {0}\n", counter);
            m_GlobalTimer->reset();
            counter = 0;
        }
        else 
        {
            counter++;
        }
        

        m_GlobalDeltaTime = duration_cast<microseconds>(m_GlobalTimer->getCurrentTime() - m_UpdateTime).count();
        m_UpdateTime = m_GlobalTimer->getCurrentTime();

        //ConsoleLogInfo("Delta Time: {0}\n", m_GlobalDeltaTime);
    }


     GlobalUpdateManager::~GlobalUpdateManager()
     {
        delete syncUpdateManger;
        delete frameUpdateManager;
        delete m_GlobalTimer;
     }

     //paraOnFrameManager
     //paraOnFrameUpdate
     //paraOnCallUpdate
}