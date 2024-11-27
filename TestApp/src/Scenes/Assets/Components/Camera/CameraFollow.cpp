#include "Core/EngineLogger.hpp"
#include "Core/SceneManagment/Components/SPComps/Camera.hpp"
#include <cstdio>
#include <engine3d/Physics/JoltHandler.hpp>
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Scenes/Assets/Components/Physics/PhysicsBody3D.hpp>
#include <Scenes/Assets/Components/Camera/CameraFollow.hpp>
#include <engine3d/Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include <Jolt/Math/Quat.h>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

const float RADIUS = 20.0f;
const float HEIGHT = 10.0f;


// namespace ToPhysics
// {
//     glm::vec3 FollowObject(const glm::vec3& targetPosition, const glm::vec3& velocity, float radius) {
//     // Avoid division by zero by handling near-zero velocity components
//     glm::vec3 adjustedVelocity = velocity;
//     if (glm::length(velocity) < 0.001f) {
//         adjustedVelocity = glm::vec3(0.0f, 0.0f, -1.0f); // Default fallback direction
//     }

//     // Calculate the direction opposite to the velocity
//     glm::vec3 normVel = -glm::normalize(glm::vec3(adjustedVelocity.x, 0.0f, adjustedVelocity.z));

//     // Scale by the desired follow radius
//     glm::vec3 followOffset = normVel * radius;

//     // Calculate the camera's new position behind the target
//     return targetPosition + followOffset;
// }
// };

CameraFollow::CameraFollow(engine3d::Transform &p_transform)
{
    m_PlayerTransform = &p_transform;
}

void CameraFollow::OnIntegrate()
{
    engine3d::SyncUpdateManager::GetInstance()->Subscribe
            (this, &CameraFollow::LateUpdate);

    // Probably should be an event or called when activated
    //! @note For now just calling Begin on integrate
    m_Transform = &m_GameObjectRef->GetComponent<engine3d::Transform>();;
}

void CameraFollow::LateUpdate()
{

    engine3d::SceneObject* player = m_PlayerTransform->GetParent();
    PhysicsBody3D* rb = &player->GetComponent<PhysicsBody3D>();
    auto l_interface = engine3d::JoltHandler::GetInstance()->getInterface();
    auto player_velocity = l_interface->GetLinearVelocity(rb->GetBody()->m_BodyID);
    // glm::vec3 newPos = ToPhysics::FollowObject(
    //     m_PlayerTransform->GetPos<glm::vec3>(), 
    //     glm::vec3{player_velocity.GetZ(),player_velocity.GetY(),player_velocity.GetZ()}, 
    //     RADIUS);

    m_Transform->SetPos<glm::vec3>({
        m_PlayerTransform->m_Position.x,
        m_PlayerTransform->m_Position.y + HEIGHT,
        m_PlayerTransform->m_Position.z - RADIUS
    });

    m_GameObjectRef->GetComponent<engine3d::Camera>().SetViewTarget(
        {
            m_Transform->m_Position.x,
            m_Transform->m_Position.y,
            m_Transform->m_Position.z}, 
        m_PlayerTransform->m_Position);
    
}