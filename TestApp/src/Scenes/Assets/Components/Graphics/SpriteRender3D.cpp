#include "Core/EngineLogger.hpp"
#include <Scenes/Assets/Components/Graphics/SpriteRender3D.hpp>
#include <numbers>
#include <Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/CubeMesh.hpp>
#include <Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/SphereMesh.hpp>

// #include "Scene"

using namespace engine3d;
  const int Radius = 1;
  const int SectorCount = 10;
  const int StackCount = 10;

void SpriteRender3D::CreateMesh()
{

}

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
}

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
  
// for(int i = 0; i <= StackCount; ++i)
// {
//     // starting from pi/2 to -pi/2
//     StackAngle = std::numbers::pi / 2 - i * StackStep; 
//     xy = Radius * cosf(StackAngle); // r * cos(u)
//     z = Radius * sinf(StackAngle);  // r * sin(u)

//     // add (SectorCount+1) vertices per stack
//     // first and last vertices have same position 
//     // and normal, but different tex coords
//     for(int j = 0; j <= SectorCount; ++j)
//     {
      
//         SectorAngle = j * SectorStep;           // starting from 0 to 2pi

//         // vertex position (x, y, z)
//         x = xy * cosf(SectorAngle);             // r * cos(u) * cos(v)
//         y = xy * sinf(SectorAngle);             // r * cos(u) * sin(v)
//         vertices.push_back(glm::vec3(x,y,z));

//         // normalized vertex normal (nx, ny, nz)
//         nx = x * LengthInv;
//         ny = y * LengthInv;
//         nz = z * LengthInv;
//         normals.push_back(glm::vec3(nx,ny,nz));

//         // vertex tex coord (s, t) range between [0, 1]
//         s = (float)j / SectorCount;
//         t = (float)i / StackCount;
//         texCoords.push_back(glm::vec2(s,t));
//     }
// }
// for (auto position : vertices)
// {
//   ConsoleLogInfo("vector positions: ({0}, {1}, {2})", position.x, position.y, position.z);
// }
}

void SpriteRender3D::LateUpdate() {}

void SpriteRender3D::PhysicsUpdate() {}