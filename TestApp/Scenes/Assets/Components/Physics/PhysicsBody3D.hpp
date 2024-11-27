
#pragma once

#include "Core/SceneManagment/Components/GameComponent.hpp"
#include <Scenes/Assets/Components/Bodies/BodyContainer.hpp>
#include <Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include <Core/SceneManagment/Components/SPComps/Transform.hpp>
#include <Jolt/Math/Quat.h>

#include <Jolt/Physics/Body/BodyInterface.h>
// This is a test version of this class as there is to much
class PhysicsBody3D: public engine3d::GameComponent
{
    public:
        PhysicsBody3D(BodyContainer * p_bodyCon);
        ~PhysicsBody3D();
        void OnIntegrate();
        void Update();
        void LateUpdate();
        void PhysicsUpdate();
        void Begin();
        BodyContainer* GetBody();

        void ForcedSetScale(float x, float y, float z);
        void ForcedSetPosition(float x, float y, float z);
        void ForcedSetRotation(Quat quaternion);

    private:
        engine3d::Transform* m_Transform;
        PhysicsBody3D() = default;
        BodyContainer * bodyType;
        JPH::BodyInterface* m_interface;
        bool once = false;
};
