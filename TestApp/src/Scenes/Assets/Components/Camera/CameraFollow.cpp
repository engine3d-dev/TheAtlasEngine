#include "Core/EngineLogger.hpp"
#include "Core/TimeManagement/GlobalUpdateManager.hpp"
#include <Core/SceneManagment/Components/SPComps/Camera.hpp>
#include <Core/SceneManagment/Components/SPComps/Transform.hpp>
#include <Physics/Interfaces/BPLayerInterfaceHandler.hpp>
#include <Jolt/Math/Vec3.h>
#include <Scenes/Assets/Components/testComp.hpp>
#include <cstddef>
#include <cstdio>
#include <engine3d/Physics/JoltHandler.hpp>
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Scenes/Assets/Components/Physics/PhysicsBody3D.hpp>
#include <Scenes/Assets/Components/Camera/CameraFollow.hpp>
#include <engine3d/Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include <Jolt/Math/Quat.h>
#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Math/Math.hpp>
#include <Math/Interpolation.hpp>
#include <numbers>
#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

const float RADIUS = 20.0f;
const float HEIGHT = 10.0f;
const float MINDIST = 1.58f;
float distanceSpeed = 0.0f;
bool hasLooked = false;

namespace ToPhysics
{
    glm::quat LookAt(const glm::vec3& position, const glm::vec3& target) 
    {
        glm::vec3 up = {0.0f, 1.0f, 0.0f};

        glm::vec3 forward = glm::normalize(target - position);

        glm::vec3 right = glm::normalize(glm::cross(up, forward));
        glm::vec3 adjustedUp = glm::cross(forward, right);

        float w = sqrt(1.0f + right.x + adjustedUp.y + forward.z) * 0.5f;
        float invW4 = 1.0f / (4.0f * w);
        float x = (adjustedUp.z - forward.y) * invW4;
        float y = (forward.x - right.z) * invW4;
        float z = (right.y - adjustedUp.x) * invW4;

        return glm::normalize(glm::quat(w, x, y, z));
    }

    double AngleNormalize(double angle)
    {
        angle = std::fmod(angle, std::numbers::pi * 2);
        if(angle < 0)
        {
            angle += (2 * std::numbers::pi);
        }
        return  angle;
    }
}

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

    if(glm::length(player_velocity) > .00001f)
    {
        playerVelocity = player_velocity;
        playerVelocity.y = 0;
        playerVelocity = glm::normalize(playerVelocity) * -RADIUS;
    }
    distanceSpeed = glm::length(abs(m_PlayerTransform->m_Position-m_Transform->m_Position))/10.0f;
    distanceSpeed = glm::clamp(distanceSpeed, .50f, 2.0f);
    glm::vec3 finalRotPosition = glm::vec3{
        playerVelocity.x + m_PlayerTransform->m_Position.x, 
        m_PlayerTransform->m_Position.y + HEIGHT, 
        playerVelocity.z + m_PlayerTransform->m_Position.z};

    if (glm::length(finalRotPosition - m_Transform->m_Position) > 0.001f) 
    {
        glm::vec3 newPos = engine3d::Interpolation::LinearInterpolate(
                m_Transform->m_Position, 
                finalRotPosition, 
                nullptr,
                engine3d::SyncUpdateManager::GetInstance()->m_SyncGlobalDeltaTime * distanceSpeed);
        
        m_Transform->SetPos<glm::vec3>(newPos);
        glm::quat lookAt = ToPhysics::LookAt(
            m_Transform->m_Position, 
            m_PlayerTransform->m_Position);

        m_Transform->SetQuat(
            lookAt
        );

        m_Transform->SetAxisRot<>(glm::vec3{
            ToPhysics::AngleNormalize(m_Transform->GetQuat<JPH::Quat>().GetEulerAngles().GetX()),
            ToPhysics::AngleNormalize(m_Transform->GetQuat<JPH::Quat>().GetEulerAngles().GetY()),
            ToPhysics::AngleNormalize(m_Transform->GetQuat<JPH::Quat>().GetEulerAngles().GetZ())}
        );
        auto axisRot = m_Transform->GetAxisRot<glm::vec3>();


        m_GameObjectRef->GetComponent<engine3d::Camera>().SetViewXYZ(
        m_Transform->m_Position, 
        {
            m_Transform->GetQuat<glm::vec4>().w,
            m_Transform->GetQuat<glm::vec4>().x,
            m_Transform->GetQuat<glm::vec4>().y,
            m_Transform->GetQuat<glm::vec4>().z,
        });
    }
}

void CameraFollow::PhysicsUpdate()
{  
}