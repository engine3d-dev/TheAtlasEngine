#include <Scenes/Assets/Components/Physics/PhysicsBody3D.hpp>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyID.h>
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
        m_Transform = &m_GameObjectRef->SceneGetComponent<Transform>();
}

void PhysicsBody3D::Update()
{
        m_Transform->m_Position.x = m_interface->GetCenterOfMassPosition(bodyType->m_BodyID).GetX();
        m_Transform->m_Position.y = m_interface->GetCenterOfMassPosition(bodyType->m_BodyID).GetY();
        m_Transform->m_Position.z = m_interface->GetCenterOfMassPosition(bodyType->m_BodyID).GetZ();
        
        //m_interface->ResetSleepTimer(bodyType->m_BodyID);
        //! @note for testing purposes!

        // if(bodyType->m_BodyID.GetIndex())
        // {
        //         printf("BODY_ID %i:(X:%f, Y:%f, Z:%f)\n",
        //                 bodyType->m_BodyID.GetIndex(),
        //                 m_Transform->m_Position.x,
        //                 m_Transform->m_Position.y,
        //                 m_Transform->m_Position.z);
        // }
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