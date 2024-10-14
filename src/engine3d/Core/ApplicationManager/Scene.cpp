#include <Scene.hpp>
#include <iostream>

void SceneThreadReceiver::HandleTask(int taskID)
{
    std::cout << "ThreadReceiver is handling task ID: " << taskID << std::endl;
}