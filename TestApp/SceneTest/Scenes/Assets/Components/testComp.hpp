#pragma once

#include "Scenes/Assets/Components/Physics/PhysicsBody3D.hpp"
#include <Core/SceneManagment/Components/GameComponent.hpp>

    class testComp : public engine3d::GameComponent
    {
        public:
            void OnIntegrate();
            void Update();
            void LateUpdate();
            void PhysicsUpdate();
        private:
            bool t_Secret = false;
            BodyContainer * m_rb;
    };