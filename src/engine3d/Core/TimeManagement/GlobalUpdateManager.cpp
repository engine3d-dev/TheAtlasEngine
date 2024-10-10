#include <Core/TimeManagement/GlobalUpdateManager.hpp>
namespace engine3d
{
    GlobalUpdateManager::GlobalUpdateManager()
    {
        syncUpdateManger = new SyncUpdateManager();
        frameUpdateManager = new ParallelFrameUpdateManager();
    }

    void GlobalUpdateManager::globalOnTickUpdate()
    {
        const static float& globalDeltaTime = GlobalDeltaTime;

        

    }

     GlobalUpdateManager::~GlobalUpdateManager()
     {
        delete syncUpdateManger;
        delete frameUpdateManager;
     }
}