
#include <engine3d/Core/SceneManagment/Components/GameComponent.hpp>
#include <Scenes/Assets/Components/Physics/PhysicsBody3D.hpp>
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Scenes/Assets/Components/Graphics/Meshes/MeshContainer.hpp>
#include <Core/GraphicDrivers/Mesh.hpp>
class SpriteRender3D : public engine3d::GameComponent 
{
     public:
          SpriteRender3D() = default;
          SpriteRender3D(const engine3d::Mesh& p_MeshBody, engine3d::SceneObject* p_sceneObj);
          SpriteRender3D(const std::string& p_model, engine3d::SceneObject* p_sceneObj);
          void OnIntegrate();   
          void RenderObject();
          std::vector<glm::vec3> vertices;
          std::vector<glm::vec3> normals;
          std::vector<glm::vec2> texCoords;

     private:
          engine3d::Mesh m_Mesh;
          engine3d::Transform m_Transform;  
          glm::vec3* m_TransformPosition; 
};