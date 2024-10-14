#include <thread>
#include <Scene.hpp>

class SendThread
{
public:
    void ThreadSender(SceneThreadReceiver &receiver, int taskID);
};