// #include <engine3d/Core/EngineLogger.hpp>
#include <engine3d/Core/EngineLogger.hpp>
#include <engine3d/Core/Event/InputPoll.hpp>
#include <engine3d/Core/Event/KeyCodes.hpp>
#include <engine3d/Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
// #include <Core/../../TestApp/SceneTest/Scenes/Assets/Components/testComp.hpp>
// #include <SceneTes
// #include <SceneTest
// #include <Assets/Components/testComp.hpp>
#include <Scenes/Assets/Components/testComp.hpp>
// #include "Scene"

using namespace engine3d;
    void testComp::OnIntegrate()
    {
        printf("getting here2\n");
        SyncUpdateManager::GetInstance()->Subscribe
            (this, &testComp::Update);
            printf("getting here 3\n");
        SyncUpdateManager::GetInstance()->Subscribe
            (this, &testComp::LateUpdate);
            printf("getting here 4\n");
        SyncUpdateManager::GetInstance()->Subscribe
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

    void testComp::LateUpdate()
    {

    }

    void testComp::PhysicsUpdate()
    {
        
    }