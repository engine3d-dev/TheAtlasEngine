#include <Scenes/Assets/Components/Physics/PhysicsBody3D.hpp>

#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Math/Vec3.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Math/Quat.h>
#include <Jolt/Physics/EActivation.h>
#include <glm/fwd.hpp>
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
        // m_Transform = &m_GameObjectRef->SceneGetComponent<Transform>();
        // m_Transform = m_GameObjectRef
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

        // std::print("Rotation: (X: {0}, Y: {1}, Z: {2})\n",
        //         m_Transform->m_AxisRotation.x, 
        //         m_Transform->m_AxisRotation.y, 
        //         m_Transform->m_AxisRotation.z);
        
}

void PhysicsBody3D::SetScale(float x, float y, float z)
{
        m_interface->GetShape(bodyType->m_BodyID)->ScaleShape(RVec3(x,y,z));
}

void PhysicsBody3D::SetPosition(float x, float y, float z)
{
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