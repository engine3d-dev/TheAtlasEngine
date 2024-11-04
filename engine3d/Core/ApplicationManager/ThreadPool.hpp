#pragma once
#include <Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
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
      bool m_ThreadStop = false;
    private:
      std::jthread syncUpdateThread;
      void UpdateSyncFunction();
      SyncUpdateManager * syncManager = NULL;
      float m_DeltaTime = 0.0f;
  };
};
