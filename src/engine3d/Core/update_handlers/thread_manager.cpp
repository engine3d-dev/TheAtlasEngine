#include <core/engine_logger.hpp>
#include <update_handlers/global_update_manager.hpp>
#include <update_handlers/sync_update_manager.hpp>
#include <core/update_handlers/thread_manager.hpp>
#include <condition_variable>
#include <fmt/format.h>
#include <thread_utils/thread_utils.hpp>
#include <barrier>

namespace engine3d{
    std::mutex g_SyncLock;
    std::mutex g_ParallelLock;
    //! @note conditional variable specifically for SyncUpdateManager's thread.
    //! @note Name's or use may change later for thread manager. Since TaskGraph/Signal trees WILL replace this.
    std::condition_variable g_SyncFrame;
    std::condition_variable g_ParallelFrame;
    
    static bool g_Ready = false;
    static bool g_Ready2 = false;

    std::atomic<uint32_t> g_ParallelCounter;


    ThreadManager::ThreadManager() : syncUpdateThread("Sync Update Thread"){
        m_ThreadStop = false;
        syncUpdateThread.Dispatch(&ThreadManager::UpdateSyncFunction, this);
        //! @note Allocating the threads needed for the parallel frame to run at

        if(syncUpdateThread.Joinable()){
            ConsoleLogInfo("Thread Manager is running!");
        }

        // auto core_amount_half = GetThreadCount() / 2;
        // ConsoleLogTrace("Amount of Threads Specified: {}", core_amount_half);

        // for(uint32_t i = 0; i < core_amount_half; i++){
        //     Thread t = Thread(fmt::format("Thread {}", i));
        //     t.Dispatch(&ThreadManager::UpdateParallelFunction, this);
        //     m_ParallelThreads.push_back(&t);
        // }

    }

    ThreadManager::~ThreadManager(){
        m_ThreadStop = true;
        g_Ready = true;
        g_Ready2 = true;
        g_SyncFrame.notify_all();
        g_ParallelFrame.notify_all();
    }

    void ThreadManager::ThreadHandler(){
    }

    void ThreadManager::OnRun(float p_DeltaTime){
        m_DeltaTime = p_DeltaTime;
        g_Ready = true;
        g_Ready2 = true;
        g_SyncFrame.notify_one();
        g_ParallelFrame.notify_all();
    }

    void ThreadManager::UpdateSyncFunction(){
        while(!m_ThreadStop){
            std::unique_lock<std::mutex> m(g_SyncLock);
            
            GlobalUpdateManager::IncrementCounter();

            g_SyncFrame.wait(m, []{return g_Ready; });

            if(m_ThreadStop){
                ConsoleLogInfo("Cleaning up Thread");
                break;
            }

            g_Ready = false;
            SyncUpdateManager::RunUpdate(m_DeltaTime);
        }
    }

    void ThreadManager::UpdateParallelFunction(){
        //! @note 1.) Get all threads to wait on g_SyncFrame
        /*

            Except they dont wait on all g_SyncFrame (atomic)

            1.) Barrier
            1.1 stop all threads moving further (until hit barrier).
                1.1.1

            1.2 Then sync with global update using g_Parallel.wait (g_ParallelFrame)
            - using second barrier.
        */
        ConsoleLogInfo("Are you running?");
        while(!m_ThreadStop){
            // 1.)
            std::barrier sync_point(m_ParallelThreads.size(), [&]() noexcept{
                ConsoleLogInfo("Inside Barrier");
            });

            GlobalUpdateManager::IncrementCounter();

            g_ParallelCounter += 1;
            std::unique_lock<std::mutex> m(g_ParallelLock);

            g_ParallelFrame.wait(m, []{ return g_Ready2; });
            
            std::barrier sync_point2(m_ParallelThreads.size(), [&]() noexcept{
                ConsoleLogInfo("Inside Barrier #2");
            });

            if(m_ThreadStop){
                ConsoleLogInfo("Cleaning up Thread Parallel");
                break;
            }

            g_Ready2 = false;
            SyncUpdateManager::RunUpdate(m_DeltaTime);
        }

        //! @note 2.)
    }

    void ThreadManager::OnStop(){
        m_ThreadStop = true;
        g_Ready = true;
        g_Ready2 = true;
        g_SyncFrame.notify_all();
        g_ParallelFrame.notify_all();
    }
};