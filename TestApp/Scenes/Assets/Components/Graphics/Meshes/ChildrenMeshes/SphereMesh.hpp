#include "Core/SceneManagment/Components/GameComponent.hpp"
#include "Scenes/Assets/Components/Physics/PhysicsBody3D.hpp"

class SphereMesh : public engine3d::GameComponent
{   
    public:
        void CreateSphere();

    private:
        engine3d::Transform m_Transform;  
        glm::vec3* m_TransformPosition; 
};