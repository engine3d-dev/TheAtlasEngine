#pragma once
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include <Core/ApplicationManager/Scene.hpp>
// #include <Core/../../TestApp/SceneTest/Scenes/Assets/Components/testComp.hpp>
#include <thread>


namespace engine3d
{
  class ThreadMngr 
  {
    public:
      ThreadMngr();
      ~ ThreadMngr();
      void ThreadHandler();
      void OnRun(float p_DeltaTime);
      SyncUpdateManager * getSyncManager();
      bool m_ThreadStop = false;
    private:
      std::jthread syncUpdateThread;
      void UpdateSyncFunction();
      SyncUpdateManager * m_SyncManager = NULL;
      float m_DeltaTime = 0.0f;

      //test stuff
      Scene *t_NewScene = NULL;
      SceneObject* t_NewObject = NULL;

  };
};
