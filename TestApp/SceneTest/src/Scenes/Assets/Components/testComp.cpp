#include "EngineLogger.hpp"
#include "Event/InputPoll.hpp"
#include "Event/KeyCodes.hpp"
#include "TimeManagement/UpdateManagers/SyncUpdateManager.hpp"
#include <Core/../../TestApp/SceneTest/Scenes/Assets/Components/testComp.hpp>
namespace engine3d
{
    void testComp::OnIntegrate()
    {
        SyncUpdateManager::g_SyncManager->Subscribe
            (this, &testComp::Update);
        SyncUpdateManager::g_SyncManager->Subscribe
            (this, &testComp::LateUpdate);
        SyncUpdateManager::g_SyncManager->Subscribe
            (this, &testComp::PhysicsUpdate);
        
        printf("Registered Object!");
    }

    void testComp::Update() 
    {
        if(InputPoll::IsKeyPressed(KeyCode::F5) && !t_Secret)
        {
            ConsoleLogInfo("Secret Key has been pressed!\n");
            t_Secret = true;
        }
        else if(!InputPoll::IsKeyPressed(KeyCode::F5) && t_Secret)
        {
            ConsoleLogInfo("Secret Key has been released!\n");
            t_Secret = false;
        }
    }
};