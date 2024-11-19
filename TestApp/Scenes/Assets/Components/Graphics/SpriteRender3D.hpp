#include "Scenes/Assets/Components/Physics/PhysicsBody3D.hpp"
#include <Core/SceneManagment/Components/GameComponent.hpp>

class SpriteRender3D : public engine3d::GameComponent 
{
     public:
          SpriteRender3D(engine3d::Transform){};
          void OnIntegrate();  
          void Update();
          void LateUpdate();
          void PhysicsUpdate();
          void CreateMesh();
          std::vector<glm::vec3> vertices;
          std::vector<glm::vec3> normals;
          std::vector<glm::vec2> texCoords;
     private:
          engine3d::Transform m_Transform;  
          glm::vec3* m_TransformPosition; 
};