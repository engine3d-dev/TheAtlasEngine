
#include "ApplicationManager/Scene.hpp"
#include "SceneManagment/SceneObjects/SceneObject.hpp"
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
    m_SyncManager = new SyncUpdateManager();
    m_SyncManager->g_SyncManager = m_SyncManager;
    t_NewScene = new Scene();
    t_NewObject = new SceneObject(t_NewScene);
    t_NewObject->SceneObject::AddComponent<testComp>();
    m_ThreadStop = false;
    syncUpdateThread = std::jthread(&ThreadMngr::UpdateSyncFunction, this);

    if(syncUpdateThread.joinable())
      printf("Thread manager Running!\n");
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
      if(m_ThreadStop)
      {
        printf("Cleaning Thread!\n");
        break;
      }
      ready = false;
      m_SyncManager->RunUpdate(m_DeltaTime);
    }
  }

  ThreadMngr::~ThreadMngr()
  {
    delete t_NewObject;
    delete t_NewScene;
    delete m_SyncManager;
    m_ThreadStop = true;
    ready = true;
    frameKey.notify_all();
  }
};
