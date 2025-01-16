#include <core/event/input_poll.hpp>
#include <core/engine_logger.hpp>
#include <core/update_handlers/sync_update.hpp>


namespace engine3d{
    static Timer s_LocalTimer = Timer();
    static std::chrono::time_point<std::chrono::high_resolution_clock> s_LocalUpdateTime;
    static float s_SyncLocalDeltaTime = 0.0f;
    static float s_SyncGlobalDeltaTime = 0.0f;
    static int s_MaxVariance = 0;
    static int s_MinFrames = 0;
    static int s_LocalUpdateCounter = 0;
    static int s_LocalFrameratePerSecond = 0;
    static int s_RandomFrame;
    std::deque<std::function<void()>> SyncUpdate::s_SyncLateUpdateSubscribers;
    std::deque<std::function<void()>> SyncUpdate::s_SyncUpdateSubscribers;
    std::deque<std::function<void()>> SyncUpdate::s_SyncOnTickUpdateSubscribers;
    std::deque<std::function<void()>> SyncUpdate::s_SyncRenderSubscribers;
        // std::chrono::time_point<std::chrono::high_resolution_clock> m_LocalUpdateTime;
        
    // int m_MaxVariance;
    // int m_MinFrames;

    // Timer m_LocalTimer;
    // // InputPoll* m_KeyEvent;
    // int m_LocalUpdateCounter;
    // int m_RandomFrame;

    // //! @note inaccurate fps because it relies on window to call.
    // //! @note Jframe may solve issue but yet to be tested.
    // float m_SyncLocalDeltaTime;
    // float m_SyncGlobalDeltaTime;
    // int m_LocalFPS;

    void SyncUpdate::InitializeSyncUpdate()
    {
        ConsoleLogInfo("SyncUpdate::InitializeSyncUpdate Initialized!!");
        s_SyncLateUpdateSubscribers = std::deque<std::function<void()>>();
        s_SyncUpdateSubscribers = std::deque<std::function<void()>>();
        s_SyncOnTickUpdateSubscribers = std::deque<std::function<void()>>();
        s_SyncRenderSubscribers = std::deque<std::function<void()>>();

        // s_LocalTimer = Timer();
        s_LocalTimer = Timer();
        // s_LocalUpdateTime = s_LocalTimer.GetCurrentTime();
        s_LocalUpdateTime = s_LocalTimer.GetCurrentTime();
        s_SyncLocalDeltaTime = 0.0;
        s_MaxVariance = 2;
        s_MinFrames = 0;
        s_LocalUpdateCounter = 0;
        s_LocalFrameratePerSecond = 0;
        srand(std::time(NULL));
        s_RandomFrame = (rand() % s_MaxVariance) + s_MinFrames;
    }

    SyncUpdate::~SyncUpdate()
    {
        // delete s_LocalTimer;
    }

    //! @note this does not work per object this might need to change a little.
    //! Possibly pass gameObjects with the virtual functions.
    //! Possibly seperate active scripts to non active ones in scenes.
    void SyncUpdate::RunUpdate(float deltaTime)
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
        s_SyncGlobalDeltaTime = deltaTime;
        // OnPhysicsUpdate();
        // const int collisionSteps = 1 + (60*(deltaTime));

        // JoltHandler* p_joltHandler = JoltHandler::GetInstance();
        //Updating physics system based on jolt physics
        // p_joltHandler->physics_system.Update(
        //     deltaTime, 
        //     collisionSteps, 
        //     p_joltHandler->physics_allocator, 
        //     p_joltHandler->job_system);

        if(s_RandomFrame <= s_LocalUpdateCounter)
        {
            s_RandomFrame = (rand() % s_MaxVariance) + s_MinFrames;
            s_LocalUpdateCounter = 0;

            OnUpdate();
            OnLateUpdate();

            s_SyncLocalDeltaTime = duration_cast<std::chrono::microseconds>
                (s_LocalTimer.GetCurrentTime() - s_LocalUpdateTime).count();
            s_SyncLocalDeltaTime = s_SyncLocalDeltaTime/1000000;

            s_LocalUpdateTime = s_LocalTimer.GetCurrentTime();
            s_LocalFrameratePerSecond++;
        }
        else 
        {
            s_LocalUpdateCounter++;
        }
        //! @note I don't understand why this works but localtime doesn't.
        //! @note My guess is that it needs to be called every frame/ double buffering
        //! might be an issue.
        // OnSceneRender();
        if(s_LocalTimer.ElapsedSec() >= 1.0)
        {
            s_LocalTimer.Reset();

            //! @note Key event added to allow switch between global and local.
            if(InputPoll::IsKeyPressed(KeyCode::K))
            {
                ConsoleLogInfo("Local FPS: {0}, Local Delta Time: {1}",
                    s_LocalFrameratePerSecond, s_SyncLocalDeltaTime);
            }
            s_LocalFrameratePerSecond = 0;
        }
    }

    void SyncUpdate::OnSceneRender()
    {
        for(auto& l_Subscriber : s_SyncRenderSubscribers)
        {
            l_Subscriber();
        }
    }

    void SyncUpdate::OnPhysicsUpdate()
    {
        for(auto& l_Subscriber : s_SyncOnTickUpdateSubscribers)
        {
            l_Subscriber();
        }
    }

    void SyncUpdate::OnUpdate()
    {
        for(auto& l_Subscriber : s_SyncUpdateSubscribers)
        {
            l_Subscriber();
        }
    }

    void SyncUpdate::OnLateUpdate()
    {
        for(auto& l_Subscriber : s_SyncLateUpdateSubscribers)
        {
            l_Subscriber();
        }
    }

    float SyncUpdate::DeltaTime(){
        return s_SyncLocalDeltaTime;
    }
};