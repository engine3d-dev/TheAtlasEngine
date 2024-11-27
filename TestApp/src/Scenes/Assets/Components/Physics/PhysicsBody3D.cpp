// #include <Scenes/Assets/Components/Physics/PhysicsBody3D.hpp>
#include <Scenes/Assets/Components/Physics/PhysicsBody3D.hpp>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <glm/fwd.hpp>
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
using namespace engine3d;

PhysicsBody3D::PhysicsBody3D(BodyContainer * p_bodyCon)
{
    bodyType = p_bodyCon;
}

void PhysicsBody3D::OnIntegrate()
{
    SyncUpdateManager::GetInstance()->Subscribe
            (this, &PhysicsBody3D::Update);
    SyncUpdateManager::GetInstance()->Subscribe
            (this, &PhysicsBody3D::LateUpdate);
    SyncUpdateManager::GetInstance()->Subscribe
            (this, &PhysicsBody3D::PhysicsUpdate);

    // Probably should be an event or called when activated
    //! @note For now just calling Begin on integrate

    Begin();
}

void PhysicsBody3D::Begin()
{
        m_interface = engine3d::JoltHandler::GetInstance()->getInterface();
        m_Transform = &m_GameObjectRef->GetComponent<Transform>();
        
        auto position = m_Transform->GetPos<glm::vec3>();
        bodyType->CreateBody(
                EActivation::Activate, 
                JPH::RVec3(position.x,position.y,position.z));
}

void PhysicsBody3D::Update()
{       
        //Convert Posiitons
        m_Transform->SetPos<JPH::Vec3>(
                m_interface->GetWorldTransform(bodyType->m_BodyID).GetTranslation()); 
        
        //Convert Rotations
        m_Transform->SetQuat<JPH::Quat>(
                m_interface->GetRotation(bodyType->m_BodyID));

        //Convert Rotations
        m_Transform->SetAxisRot<JPH::Vec3>(
        m_interface->GetRotation(bodyType->m_BodyID).GetEulerAngles());
}

void PhysicsBody3D::ForcedSetScale(float x, float y, float z)
{
        bodyType->CreateScaledType(JPH::Vec3(x,y,z));
        bodyType->m_BodyID = bodyType->CreateBody(
                JPH::EActivation::Activate,
                m_Transform->GetPos<JPH::RVec3>()
                );
        m_Transform->SetScale<glm::vec3>({x,y,z});
}

void PhysicsBody3D::ForcedSetPosition(float x, float y, float z)
{
        m_interface->SetPosition(
                bodyType->m_BodyID,
                RVec3(x,y,z),
                JPH::EActivation::Activate);
        m_Transform->SetPos<glm::vec3>({x,y,z});
}

void PhysicsBody3D::ForcedSetRotation(Quat quaternion)
{
        m_interface->SetRotation(
                bodyType->m_BodyID, 
                quaternion,
                JPH::EActivation::Activate);

        m_Transform->SetQuat<JPH::Quat>(
                quaternion);

        m_Transform->SetAxisRot<JPH::Vec3>(
        m_interface->GetRotation(bodyType->m_BodyID).GetEulerAngles());
}

void PhysicsBody3D::LateUpdate()
{

}

void PhysicsBody3D::PhysicsUpdate()
{

}

BodyContainer* PhysicsBody3D::GetBody()
{
        return bodyType;
}

PhysicsBody3D::~PhysicsBody3D()
{
        delete bodyType;
}