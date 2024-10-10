#include <Core/TimeManagement/GlobalUpdateManager.hpp>
namespace engine3d
{
    GlobalUpdateManager::GlobalUpdateManager()
    {
        syncUpdateManger = new SyncUpdateManager();
        frameUpdateManager = new ParallelFrameUpdateManager();
    }


     GlobalUpdateManager::~GlobalUpdateManager()
     {
        
     }
    void globalOnTickUpdate();
}