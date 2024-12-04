#pragma once

#include <Scenes/Assets/Components/Physics/PhysicsBody3D.hpp>
#include <Core/SceneManagment/Components/GameComponent.hpp>
#include <glm/fwd.hpp>
#include <engine3d/Core/SceneManagment/Components/SPComps/Transform.hpp>

    class testComp : public engine3d::GameComponent
    {
        public:
            void OnIntegrate();
            void Update();
            void LateUpdate();
            void PhysicsUpdate();

            void SetCameraTransform(engine3d::Transform* p_Transform)
            {
                m_CameraTransform = p_Transform;
            }
        private:
            bool t_Secret = false;
            BodyContainer * m_rb;
            engine3d::Transform* m_CameraTransform;
            engine3d::Transform* m_PlayerTransform;
    };