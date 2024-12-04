#include "Core/EngineLogger.hpp"
#include "Core/SceneManagment/Components/SPComps/Camera.hpp"
#include "Core/SceneManagment/Components/SPComps/Transform.hpp"
#include "Physics/Interfaces/BPLayerInterfaceHandler.hpp"
#include <Scenes/Assets/Components/testComp.hpp>
#include <cstddef>
#include <cstdio>
#include <engine3d/Physics/JoltHandler.hpp>
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Scenes/Assets/Components/Physics/PhysicsBody3D.hpp>
#include <Scenes/Assets/Components/Camera/CameraFollow.hpp>
#include <engine3d/Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include <Jolt/Math/Quat.h>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Math/Math.hpp>
#include <Math/Interpolation.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

const float RADIUS = 20.0f;
const float HEIGHT = 10.0f;
float distanceSpeed = 0.0f;

CameraFollow::CameraFollow(engine3d::Transform &p_transform)
{
    m_PlayerTransform = &p_transform;
}

void CameraFollow::OnIntegrate()
{
    engine3d::SyncUpdateManager::GetInstance()->Subscribe
            (this, &CameraFollow::PhysicsUpdate);
    engine3d::SyncUpdateManager::GetInstance()->Subscribe
            (this, &CameraFollow::LateUpdate);

    // Probably should be an event or called when activated
    //! @note For now just calling Begin on integrate
    m_Transform = &m_GameObjectRef->GetComponent<engine3d::Transform>();
    auto* player = m_PlayerTransform->GetParent();
    player->GetComponent<testComp>().SetCameraTransform(m_Transform);
}

void CameraFollow::LateUpdate()
{

    engine3d::SceneObject* player = m_PlayerTransform->GetParent();
    PhysicsBody3D* rb = &player->GetComponent<PhysicsBody3D>();
    auto l_interface = engine3d::JoltHandler::GetInstance()->getInterface();
    const glm::vec3 player_velocity = VectorConversion::To_Type<glm::vec3>(
        l_interface->GetLinearVelocity(rb->GetBody()->m_BodyID));

    //glm::quat playerRotation = m_PlayerTransform->GetQuat<glm::quat>();
    //glm::vec3 forward = glm::normalize(glm::rotate(playerRotation, glm::vec3(0.0f, 0.0f, -1.0f)));
    //glm::vec3 CameraPos = forward * RADIUS;
    //CameraPos[1] = 0.0f;

    if(glm::length(player_velocity) > .00001f)
    {
        playerVelocity = glm::normalize(player_velocity) * -RADIUS;
    }
    distanceSpeed = glm::length(abs(m_PlayerTransform->m_Position-m_Transform->m_Position))/10.0f;

    ConsoleLogWarn("speedmult: {}", distanceSpeed);
        glm::vec3 finalRotPosition = glm::vec3{
            playerVelocity.x + m_PlayerTransform->m_Position.x, 
            m_PlayerTransform->m_Position.y + HEIGHT, 
            playerVelocity.z + m_PlayerTransform->m_Position.z}; 
        m_Transform->SetPos<glm::vec3>({
            engine3d::Interpolation::LinearInterpolate(
                m_Transform->m_Position, 
                finalRotPosition, 
                nullptr,
                engine3d::SyncUpdateManager::GetInstance()->m_SyncLocalDeltaTime * distanceSpeed)
        });


    m_GameObjectRef->GetComponent<engine3d::Camera>().SetViewTarget(
        m_Transform->m_Position,  
        m_PlayerTransform->m_Position);
    
}

void CameraFollow::PhysicsUpdate()
{
    
}