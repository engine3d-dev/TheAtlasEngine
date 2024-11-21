#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Core/SceneManagment/Components/SPComps/Transform.hpp>
#include "EngineLogger.hpp"

namespace engine3d {
    SceneObject::SceneObject(entt::entity handle, Scene *scene)
        : SceneObjectHandler(handle), m_ParentScene(scene) 
        {
            AddComponent<Transform>();
            auto transform_component = SceneGetComponent<Transform>();
            ConsoleLogInfo("(Init) transform_component.m_Position = ({}, {}, {})", transform_component.m_Position.x, transform_component.m_Position.y, transform_component.m_Position.z);
        }

    SceneObject::SceneObject(Scene *scene)
    {
        m_ParentScene = scene;
        SceneObjectHandler = scene->m_SceneRegistry.create();
        ConsoleLogInfo("Entity Registered");
        AddComponent<Transform>();
    }

    glm::mat4 SceneObject::toMat4(){
        auto transform_component = SceneGetComponent<Transform>();

        ConsoleLogInfo("(toMat4) transform_component.m_Position = ({}, {}, {})", transform_component.m_Position.x, transform_component.m_Position.y, transform_component.m_Position.z);

        auto transform = glm::translate(glm::mat4{1.f}, transform_component.m_Position);
        transform = glm::rotate(transform, transform_component.m_AxisRotation.y, {0.f, 1.f, 0.f});
        transform = glm::rotate(transform, transform_component.m_AxisRotation.x, {1.f, 0.f, 0.f});
        transform = glm::rotate(transform, transform_component.m_AxisRotation.z, {0.f, 0.f, 1.f});
        return transform;
    }
}; // namespace Engine3D
