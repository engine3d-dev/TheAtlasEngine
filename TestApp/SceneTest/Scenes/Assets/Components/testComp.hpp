#pragma once

#include <Core/SceneManagment/Components/GameComponent.hpp>
namespace engine3d
{
    class testComp : public GameComponent
    {
        public:
            void OnIntegrate();
            void Update();
            void LateUpdate(){};
            void PhysicsUpdate(){};
        private:
            bool t_Secret = false;
    };
};