#pragma once

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
    };