// #include "Core/EngineLogger.hpp"
// #include <TestApp/SceneTest/Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/SphereMesh.hpp>
// #include <numbers>
#include <Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/SphereMesh.hpp>
using namespace engine3d;

void SphereMesh::CreateSphere(){
    m_Transform = m_GameObjectRef->SceneGetComponent<Transform>();
    m_TransformPosition = &m_Transform.m_Position;
}