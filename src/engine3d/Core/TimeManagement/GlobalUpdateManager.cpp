#include <Core/TimeManagement/GlobalUpdateManager.hpp>
#include <print>
namespace engine3d
{
    GlobalUpdateManager::GlobalUpdateManager()
    {
        syncUpdateManger = new SyncUpdateManager();
        frameUpdateManager = new ParallelFrameUpdateManager();
        GlobalDeltaTime = 0.0;
    }

    void GlobalUpdateManager::globalOnTickUpdate()
    {
        const static float& globalDeltaTime = GlobalDeltaTime;


        std::print("Delta Time: {0}\n", GlobalDeltaTime);

    }

     GlobalUpdateManager::~GlobalUpdateManager()
     {
        delete syncUpdateManger;
        delete frameUpdateManager;
     }
}