#include <thread>
#include <ThreadPool.hpp>

void SendThread::ThreadSender(SceneThreadReceiver &receiver, int taskID)
{
    std::thread workerThread(&SceneThreadReceiver::HandleTask, &receiver, taskID);
    workerThread.detach();
}