
#include "ApplicationManager/Scene.hpp"
#include "SceneManagment/SceneObjects/SceneObject.hpp"
#include <Core/ApplicationManager/ThreadMngr.hpp>
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
    m_SyncManager = SyncUpdateManager::GetInstance();
    t_NewScene = new Scene();
    //! @note NOTE TO MENTION TO AARON: new SceneObject(this);
    t_NewObject = new SceneObject(t_NewScene);
    // t_NewObject->SceneObject::AddComponent<testComp>();
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

  SyncUpdateManager * ThreadMngr::getSyncManager()
  {
    return m_SyncManager;
  }

  ThreadMngr::~ThreadMngr()
  {
    delete t_NewObject;
    delete t_NewScene;
    m_ThreadStop = true;
    ready = true;
    frameKey.notify_all();
  }
};
