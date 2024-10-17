#include <thread>
#include <Core/ApplicationManager/ThreadPool.hpp>

void SendThread::ThreadSender(SceneThreadReceiver &receiver, int taskID)
{
    std::thread workerThread(&SceneThreadReceiver::HandleTask, &receiver, taskID);
    workerThread.detach();
}