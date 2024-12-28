#pragma once
#include <core/core.hpp>
#include <thread_utils/thread.hpp>
#include <map>
#include <vector>

namespace engine3d{
    /**
     * @name Thread Manager
     * @note Might get replaced by task graph since this allows us to synchronously work with different tasks while still being in parallel
     * @note Add more information here
    */
    class ThreadManager{
    public:
        ThreadManager();
        ~ThreadManager();

        void ThreadHandler();
        void OnRun(float p_DeltaTime);
        void OnStop();

        // static uint32_t GetMaxThreads();
    private:
        void UpdateSyncFunction();
        void UpdateParallelFunction();
    private:
        bool m_ThreadStop = false;
        Thread syncUpdateThread;

        //! @note Threads specifically for parallel
        std::vector<Thread*> m_ParallelThreads;
        float m_DeltaTime = 0.0f;
    };
};