
#include <Core/ApplicationManager/ThreadPool.hpp>
#include <Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include <mutex>
#include <thread>
#include <condition_variable>

//Testing variables
std::mutex syncLock;
std::condition_variable frameKey;
bool ready = false;

namespace engine3d
{
  ThreadMngr::ThreadMngr()
  {
     syncManager = &SyncUpdateManager::getInstance();
     syncUpdateThread = std::jthread(&ThreadMngr::UpdateSyncFunction, this); 
  }
  void ThreadMngr::ThreadHandler() 
  {
    // this is supposed to serve Scene class a thread
  }

  void ThreadMngr::OnRun(float p_DeltaTime) 
  {
    m_DeltaTime = p_DeltaTime;
    ready = true;
    frameKey.notify_one();
  }

  void ThreadMngr::UpdateSyncFunction()
  {
    while(!m_ThreadStop)
    {
      std::unique_lock<std::mutex> lock(syncLock);

      frameKey.wait(lock, [] {return ready;});
      ready = false;
      syncManager->RunUpdate(m_DeltaTime);
    }
  }

  ThreadMngr::~ThreadMngr()
  {
    syncUpdateThread.join();
  }
};
