#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Core/SceneManagment/Components/SPComps/Transform.hpp>
#include "EngineLogger.hpp"

namespace engine3d {
    SceneObject::SceneObject(entt::entity handle, Scene *scene)
        : SceneObjectHandler(handle), m_ParentScene(scene) 
        {
            AddComponent<Transform>();
        }

    SceneObject::SceneObject(Scene *scene)
    {
        m_ParentScene = scene;
        SceneObjectHandler = scene->m_SceneRegistry.create();
        ConsoleLogInfo("Entity Registered");
        AddComponent<Transform>();
    }
}; // namespace Engine3D
