#include <Scenes/Assets/Components/Graphics/SpriteRender3D.hpp>
#include <Scenes/Assets/Components/Graphics/Meshes/MeshContainer.hpp>
#include <Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/CubeMesh.hpp>
#include <Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/SphereMesh.hpp>

// #include "Scene"

using namespace engine3d;
  const int Radius = 1;
  const int SectorCount = 10;
  const int StackCount = 10;

SpriteRender3D(MeshContainer* meshBody) : m_MeshContainter(meshBody){}
void SpriteRender3D::CreateMesh()
{
  // TODO: change from hardcoded values later
  MeshContainer myMeshContainer(int 1, int 10, int 10);
  myMeshContainer.SphereBlueprint();
};

void SpriteRender3D::OnIntegrate() 
{
  
  SyncUpdateManager::GetInstance()->Subscribe(
    this, &SpriteRender3D::Update);
  SyncUpdateManager::GetInstance()->Subscribe
  (this, &SpriteRender3D::LateUpdate);
  SyncUpdateManager::GetInstance()->Subscribe(
    this, &SpriteRender3D::PhysicsUpdate);

  // Need an activation and start fuction
  m_Transform = m_GameObjectRef->SceneGetComponent<Transform>();
  m_TransformPosition = &m_Transform.m_Position;
};

void SpriteRender3D::Update()
{
//   std::vector<glm::vec3>().swap(normals);
//   std::vector<glm::vec3>().swap(vertices);
//   std::vector<glm::vec2>().swap(texCoords);
//   float x, y, z, xy; //vertex position
//   float nx, ny, nz, LengthInv = 1.0f / Radius; //vertex normal
//   float s, t;        //vertex texCoord
//   float SectorStep = 2 * std::numbers::pi / SectorCount;
//   float StackStep = std::numbers::pi / StackCount;
//   float SectorAngle, StackAngle;

};

void SpriteRender3D::LateUpdate() {};

void SpriteRender3D::PhysicsUpdate() {};