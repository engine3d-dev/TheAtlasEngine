#include <Core/ApplicationManager/Scene.hpp>

void SceneThreadReceiver::HandleTask(int taskID)
{
    std::cout << "ThreadReceiver is handling task ID: " << taskID << std::endl;
}