// #include <engine3d/Core/EngineLogger.hpp>
#include "Core/SceneManagment/Components/GameComponent.hpp"
#include "Physics/JoltHandler.hpp"
#include "Scenes/Assets/Components/Physics/PhysicsBody3D.hpp"
#include <Jolt/Physics/EActivation.h>
#include <engine3d/Core/EngineLogger.hpp>
#include <engine3d/Core/Event/InputPoll.hpp>
#include <engine3d/Core/Event/KeyCodes.hpp>
#include <engine3d/Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include <Scenes/Assets/Components/testComp.hpp>
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>

using namespace engine3d;
    void testComp::OnIntegrate()
    {;
        SyncUpdateManager::GetInstance()->Subscribe
            (this, &testComp::Update);
        SyncUpdateManager::GetInstance()->Subscribe
            (this, &testComp::LateUpdate);
        SyncUpdateManager::GetInstance()->Subscribe
            (this, &testComp::PhysicsUpdate);

        // Need an activation and start funciton
        m_rb = m_GameObjectRef->GetComponent<PhysicsBody3D>().GetBody();

    }

    void testComp::Update() 
    {
        if(InputPoll::IsKeyPressed(KeyCode::F5))
        {
            JoltHandler::GetInstance()->
                getInterface()->AddForce(
                    m_rb->m_BodyID,
                    RVec3(0.0f,100000.0f,0.0f),
                    EActivation::Activate
                );
        }
    }

    void testComp::LateUpdate()
    {

    }

    void testComp::PhysicsUpdate()
    {
        
    }