#include <Scenes/Assets/Components/Graphics/SpriteRender3D.hpp>
// #include "Scene"

using namespace engine3d;
  const int Radius = 1;
  const int SectorCount = 10;
  const int StackCount = 10;

SpriteRender3D::SpriteRender3D(MeshContainer* meshBody) : m_MeshContainer(meshBody){}

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
  //! @note needs to be sent to the vulkanmodal
  m_MeshContainer->GetVertices(); 
};

void SpriteRender3D::LateUpdate() {};

void SpriteRender3D::PhysicsUpdate() {};