// #include <engine3d/Core/EngineLogger.hpp>
#include "Core/SceneManagment/Components/SPComps/Transform.hpp"
#include "Math/Math.hpp"
#include "Physics/JoltHandler.hpp"
#include <Jolt/Physics/EActivation.h>
#include <engine3d/Core/EngineLogger.hpp>
#include <engine3d/Core/Event/InputPoll.hpp>
#include <engine3d/Core/Event/KeyCodes.hpp>
#include <engine3d/Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include <Scenes/Assets/Components/testComp.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#define PUSHFORCE 6000000.0f

using namespace engine3d;
    void testComp::OnIntegrate()
    {
        SyncUpdateManager::GetInstance()->Subscribe
            (this, &testComp::Update);
        SyncUpdateManager::GetInstance()->Subscribe
            (this, &testComp::LateUpdate);
        SyncUpdateManager::GetInstance()->Subscribe
            (this, &testComp::PhysicsUpdate);

        // Need an activation and start funciton
        m_rb = m_GameObjectRef->GetComponent<PhysicsBody3D>().GetBody();
        JoltHandler::GetInstance()->getInterface()->SetGravityFactor(m_rb->m_BodyID,2.0f);
    }

    void testComp::Update() 
    {
        glm::vec3 forward = glm::normalize(
            m_GameObjectRef->GetComponent<Transform>().GetPos<glm::vec3>() -
            m_CameraTransform->GetPos<glm::vec3>()
        );
        glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(globalUp, forward));

        forward.y = 0.0f;
        right.y = 0.0f;
        
        auto player_velocity = VectorConversion::To_Type<glm::vec3>(JoltHandler::GetInstance()->
            getInterface()->GetLinearVelocity(m_rb->m_BodyID));
        float pushForce = 0.0f;
        if(abs(player_velocity.x) + abs(player_velocity.z) > 1)
        {
            pushForce = PUSHFORCE / (abs(player_velocity.x) + abs(player_velocity.z));
        }
        else {
            pushForce = PUSHFORCE;
        }
        ConsoleLogWarn("Forward: {},{},{}", player_velocity.x,forward.y,player_velocity.z);

        if(InputPoll::IsKeyPressed(KeyCode::F5))
        {
            JoltHandler::GetInstance()->
                getInterface()->AddForce(
                    m_rb->m_BodyID,
                    RVec3(0.0f,pushForce,0.0f),
                    EActivation::Activate
                );
        }
        if (InputPoll::IsKeyPressed(KeyCode::Up))
        {
            JoltHandler::GetInstance()->getInterface()->AddForce(
                m_rb->m_BodyID,
                VectorConversion::To_Type<RVec3>(forward * pushForce/2.0f),
                EActivation::Activate
            );
        }
        if (InputPoll::IsKeyPressed(KeyCode::Down))
        {
            JoltHandler::GetInstance()->getInterface()->AddForce(
                m_rb->m_BodyID,
                VectorConversion::To_Type<RVec3>(-forward * PUSHFORCE/20.0f),
                EActivation::Activate
            );
        }
        if (InputPoll::IsKeyPressed(KeyCode::Left))
        {
            JoltHandler::GetInstance()->getInterface()->AddForce(
                m_rb->m_BodyID,
                VectorConversion::To_Type<RVec3>(-right * PUSHFORCE/20.0f), 
                EActivation::Activate
            );
        }
        if (InputPoll::IsKeyPressed(KeyCode::Right))
        {
            JoltHandler::GetInstance()->getInterface()->AddForce(
                m_rb->m_BodyID,
                VectorConversion::To_Type<RVec3>(right * PUSHFORCE/20.0f), 
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