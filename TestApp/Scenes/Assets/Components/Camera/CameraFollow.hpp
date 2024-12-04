#pragma once
#include <engine3d/Core/SceneManagment/Components/GameComponent.hpp>
#include <engine3d/Core/SceneManagment/Components/SPComps/Transform.hpp>

class CameraFollow : public engine3d::GameComponent
{
    public:
        CameraFollow()=delete;
        CameraFollow(engine3d::Transform &p_transform);

        void OnIntegrate();
        void PhysicsUpdate();
        void LateUpdate();
    private:
        engine3d::Transform* m_PlayerTransform;
        engine3d::Transform* m_Transform;
        glm::vec3 playerVelocity;
};