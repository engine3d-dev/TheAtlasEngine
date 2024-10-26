#include "TimeManagement/UpdateManagers/SyncUpdateManager.hpp"
#include <Core/ApplicationManager/Scene.hpp>
#include <vector>

void SceneThread::HandleTask() {
  // an array with a Dynamic size
  std::vector<engine3d::SyncUpdateManager> syncVector;
  engine3d::SyncUpdateManager firstEntry = syncVector.front();
  engine3d::SyncUpdateManager lastEntry = syncVector.back();

  // TODO: this should make an array of sync updates
  // TODO:create a pointer for the start of the array  & end of the array
}