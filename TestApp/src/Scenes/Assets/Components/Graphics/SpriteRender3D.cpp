#include "Core/EngineLogger.hpp"
#include "Core/GraphicDrivers/Mesh.hpp"
#include <Scenes/Assets/Components/Graphics/SpriteRender3D.hpp>
using namespace engine3d;

  const int Radius = 1;
  const int SectorCount = 10;
  const int StackCount = 10;

SpriteRender3D::SpriteRender3D(const engine3d::Mesh& p_MeshBody, SceneObject* p_sceneObj)
{
  m_Mesh = p_MeshBody;
  p_sceneObj->SetMesh(m_Mesh);
}

SpriteRender3D::SpriteRender3D(const std::string& p_model, SceneObject* p_sceneObj)
{
  m_Mesh = Mesh::LoadModel(p_model);
  p_sceneObj->SetMesh(m_Mesh);
}


void SpriteRender3D::OnIntegrate() 
{
  // Need an activation and start fuction
  m_Transform = m_GameObjectRef->GetComponent<Transform>();
  m_TransformPosition = &m_Transform.m_Position;
}

void SpriteRender3D::RenderObject()
{
  //! @note for special renderer/task based rendering
}
