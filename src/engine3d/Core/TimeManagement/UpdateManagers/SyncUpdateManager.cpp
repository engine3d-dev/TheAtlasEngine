#include "TimeManagement/UpdateManagers/SyncUpdateManager.hpp"
#include "EngineLogger.hpp"
#include "Event/KeyCodes.hpp"
#include <GLFW/glfw3.h>
#include <time.h>

namespace engine3d
{
    SyncUpdateManager::SyncUpdateManager()
    {

        m_LocalTimer = new Timer();
        m_KeyEvent = new InputPoll();
        m_LocalUpdateTime = m_LocalTimer->GetCurrentTime();

        m_LocalDeltaTime = 0.0;
        m_MaxVariance = 2;
        m_MinFrames = 0;
        m_LocalUpdateCounter = 0;
        m_LocalFPS = 0;
        srand(std::time(NULL));
        m_RandomFrame = (rand() % m_MaxVariance) + m_MinFrames;
    }

    //! @note this does not work per object this might need to change a little.
    //! Possibly pass gameObjects with the virtual functions.
    //! Possibly seperate active scripts to non active ones in scenes.
    void SyncUpdateManager::RunUpdate(float deltaTime)
    {
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
        OnPhysicsUpdate(deltaTime);
        if(m_RandomFrame <= m_LocalUpdateCounter)
        {
            m_RandomFrame = (rand() % m_MaxVariance) + m_MinFrames;
            m_LocalUpdateCounter = 0;

            OnUpdate(m_LocalDeltaTime);
            OnLateUpdate(m_LocalDeltaTime);

            m_LocalDeltaTime = duration_cast<std::chrono::microseconds>
                (m_LocalTimer->GetCurrentTime() - m_LocalUpdateTime).count();

            m_LocalUpdateTime = m_LocalTimer->GetCurrentTime();
            m_LocalFPS++;
        }
        else 
        {
            m_LocalUpdateCounter++;
        }

        if(m_LocalTimer->ElapsedSec() >= 1.0)
        {
            m_LocalTimer->Reset();

            //! @note Key event added to allow switch between global and local.
            if(m_KeyEvent->IsKeyPressed(KeyCode::F2))
            {
                ConsoleLogInfo("Local FPS: {0}, Local Delta Time: {1}",
                    m_LocalFPS, m_LocalDeltaTime);
            }
            m_LocalFPS = 0;
        }
        
    }

    void SyncUpdateManager::OnPhysicsUpdate(float p_DeltaTime)
    {
        for(auto& l_Subscriber : m_SyncOnTickUpdateSubscribers)
        {
            l_Subscriber(p_DeltaTime);
        }
    }

    void SyncUpdateManager::OnUpdate(float p_DeltaTime)
    {
        for(auto& l_Subscriber : m_SyncUpdateSubscribers)
        {
            l_Subscriber(p_DeltaTime);
        }
    }

    void SyncUpdateManager::OnLateUpdate(float p_DeltaTime)
    {
        for(auto& l_Subscriber : m_SyncLateUpdateSubscribers)
        {
            l_Subscriber(p_DeltaTime);
        }
    }

    SyncUpdateManager::~SyncUpdateManager()
    {
        delete m_LocalTimer;
        delete m_KeyEvent;
    }
}