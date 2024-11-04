#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include "EngineLogger.hpp"

namespace engine3d {
    SceneObject::SceneObject(entt::entity handle, Scene *scene)
        : SceneObjectHandler(handle), m_ParentScene(scene) {}

    SceneObject::SceneObject(Scene *scene)
    {
        m_ParentScene = scene;
        SceneObjectHandler = scene->m_SceneRegistry.create();
        ConsoleLogInfo("Entity Registered");
    }
}; // namespace Engine3D
