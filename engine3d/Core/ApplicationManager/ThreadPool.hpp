#include <Core/ApplicationManager/Scene.hpp>

class SendThread
{
    public:
        void ThreadSender(SceneThreadReceiver &receiver, int taskID);
};