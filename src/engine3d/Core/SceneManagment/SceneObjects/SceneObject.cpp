#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>

namespace Engine3D
{
	SceneObject::SceneObject(entt::entity handle, Scene* scene) : SceneObjectHandler(handle), m_ParentScene(scene) {}
};