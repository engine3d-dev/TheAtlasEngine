
#include <Core/ApplicationManager/ThreadPool.hpp>

void ThreadMngr::ThreadHandler() {
  // this is supposed to serve Scene class a thread
}

void ThreadMngr::OnRun(float deltaTime) {
  // go into scene & grab the array of sync update
  // create the worker thread & send that thread sceneMngr
  // create the end pointers for the array & call
  // SyncUpdateManager::runUpdate(float deltaTime);
}