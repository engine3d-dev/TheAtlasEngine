
#include <Core/SceneManagment/Components/GameComponent.hpp>
#include <Scenes/Assets/Components/Physics/PhysicsBody3D.hpp>
#include <Scenes/Assets/Components/Graphics/Meshes/MeshContainer.hpp>

class SpriteRender3D : public engine3d::GameComponent 
{
     public:
          SpriteRender3D(MeshContainer* meshBody);
          void OnIntegrate();  
          void Update();
          void LateUpdate();
          void PhysicsUpdate();
          std::vector<glm::vec3> vertices;
          std::vector<glm::vec3> normals;
          std::vector<glm::vec2> texCoords;

     private:
          MeshContainer m_MeshContainer;
          engine3d::Transform m_Transform;  
          glm::vec3* m_TransformPosition; 
};