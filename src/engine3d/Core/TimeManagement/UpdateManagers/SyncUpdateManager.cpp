#include "TimeManagement/UpdateManagers/SyncUpdateManager.hpp"
#include "EngineLogger.hpp"
#include "Event/KeyCodes.hpp"
#include "Physics/JoltHandler.hpp"
#include <GLFW/glfw3.h>
#include <chrono>
#include <time.h>

namespace engine3d
{
    SyncUpdateManager::SyncUpdateManager()
    {

        m_LocalTimer = new Timer();
        m_KeyEvent = new InputPoll();
        m_LocalUpdateTime = m_LocalTimer->GetCurrentTime();

        m_SyncLocalDeltaTime = 0.0;
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
        OnPhysicsUpdate();
        const int collisionSteps = 1 + (60*(deltaTime));

        JoltHandler* p_joltHandler = JoltHandler::GetInstance();
        //Updating physics system based on jolt physics
        p_joltHandler->physics_system.Update(
            deltaTime, 
            collisionSteps, 
            p_joltHandler->physics_allocator, 
            p_joltHandler->job_system);
        if(m_RandomFrame <= m_LocalUpdateCounter)
        {
            m_RandomFrame = (rand() % m_MaxVariance) + m_MinFrames;
            m_LocalUpdateCounter = 0;

            OnUpdate();
            OnLateUpdate();

            m_SyncLocalDeltaTime = duration_cast<std::chrono::microseconds>
                (m_LocalTimer->GetCurrentTime() - m_LocalUpdateTime).count();
            m_SyncLocalDeltaTime = m_SyncLocalDeltaTime/1000000;

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
                    m_LocalFPS, m_SyncLocalDeltaTime);
            }
            m_LocalFPS = 0;
        }
        
    }

    void SyncUpdateManager::OnPhysicsUpdate()
    {
        for(auto& l_Subscriber : m_SyncOnTickUpdateSubscribers)
        {
            l_Subscriber();
        }
    }

    void SyncUpdateManager::OnUpdate()
    {
        for(auto& l_Subscriber : m_SyncUpdateSubscribers)
        {
            l_Subscriber();
        }
    }

    void SyncUpdateManager::OnLateUpdate()
    {
        for(auto& l_Subscriber : m_SyncLateUpdateSubscribers)
        {
            l_Subscriber();
        }
    }

    SyncUpdateManager::~SyncUpdateManager()
    {
        delete m_LocalTimer;
        delete m_KeyEvent;
    }
}