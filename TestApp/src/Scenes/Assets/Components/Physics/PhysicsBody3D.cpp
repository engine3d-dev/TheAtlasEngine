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
}

void PhysicsBody3D::Update()
{
        //Convert Posiitons
        m_Transform->m_Position.x = m_interface->
                GetCenterOfMassPosition(bodyType->m_BodyID).GetX();
        m_Transform->m_Position.y = m_interface->
                GetCenterOfMassPosition(bodyType->m_BodyID).GetY();
        m_Transform->m_Position.z = m_interface->
                GetCenterOfMassPosition(bodyType->m_BodyID).GetZ();
        
        //Convert Rotations
        m_Transform->m_QuaterionRot.x = m_interface->
                GetRotation(bodyType->m_BodyID).GetX();
        m_Transform->m_QuaterionRot.y = m_interface->
                GetRotation(bodyType->m_BodyID).GetY();
        m_Transform->m_QuaterionRot.z = m_interface->
                GetRotation(bodyType->m_BodyID).GetZ();
        m_Transform->m_QuaterionRot.w = m_interface->
                GetRotation(bodyType->m_BodyID).GetW();

        //Convert Rotations
        m_Transform->m_AxisRotation.x = m_interface->
                GetRotation(bodyType->m_BodyID).GetEulerAngles().GetX();
        m_Transform->m_AxisRotation.y = m_interface->
                GetRotation(bodyType->m_BodyID).GetEulerAngles().GetY();
        m_Transform->m_AxisRotation.z = m_interface->
                GetRotation(bodyType->m_BodyID).GetEulerAngles().GetZ();
}

void PhysicsBody3D::SetScale(float x, float y, float z)
{
        m_Transform->m_Scale = glm::vec3(x,y,z);
        m_interface->GetShape(bodyType->m_BodyID)->ScaleShape(RVec3(x,y,z));
}

void PhysicsBody3D::SetPosition(float x, float y, float z)
{
        m_Transform->m_Position = glm::vec3(x,y,z);
        m_interface->SetPosition(
                bodyType->m_BodyID,
                RVec3(x,y,z),
                JPH::EActivation::Activate);
}

void PhysicsBody3D::SetRotation(Quat quaternion)
{
        m_interface->SetRotation(
                bodyType->m_BodyID, 
                quaternion,
                JPH::EActivation::Activate);
                
        m_Transform->m_AxisRotation.x = m_interface->GetRotation(
                bodyType->m_BodyID).GetEulerAngles().GetX();
        m_Transform->m_AxisRotation.y = m_interface->GetRotation(
                bodyType->m_BodyID).GetEulerAngles().GetY();
        m_Transform->m_AxisRotation.z = m_interface->GetRotation(
                bodyType->m_BodyID).GetEulerAngles().GetZ();
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