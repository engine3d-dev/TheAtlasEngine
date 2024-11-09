// #include <engine3d/Core/EngineLogger.hpp>
#include "Core/SceneManagment/SceneObjects/SceneObject.hpp"
#include <engine3d/Core/EngineLogger.hpp>
#include <engine3d/Core/Event/InputPoll.hpp>
#include <engine3d/Core/Event/KeyCodes.hpp>
#include <engine3d/Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include <Scenes/Assets/Components/testComp.hpp>
// #include "Scene"

using namespace engine3d;
    void testComp::OnIntegrate()
    {;
        SyncUpdateManager::GetInstance()->Subscribe
            (this, &testComp::Update);
        SyncUpdateManager::GetInstance()->Subscribe
            (this, &testComp::LateUpdate);
        SyncUpdateManager::GetInstance()->Subscribe
            (this, &testComp::PhysicsUpdate);

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