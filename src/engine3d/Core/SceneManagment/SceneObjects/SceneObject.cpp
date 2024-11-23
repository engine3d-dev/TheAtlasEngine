#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Core/SceneManagment/Components/SPComps/Transform.hpp>

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
        AddComponent<Transform>();
    }

    void SceneObject::SetPosition(const glm::vec3& p_Position){
        SceneGetComponent<Transform>().m_Position = p_Position;
    }

    void SceneObject::SetRotation(const glm::vec3& p_Rotation){
        SceneGetComponent<Transform>().m_AxisRotation = p_Rotation;
    }

    glm::vec3 SceneObject::GetRotation(){
        return SceneGetComponent<Transform>().m_AxisRotation;
    }

    glm::mat4 SceneObject::toMat4(){
        auto transform_component = SceneGetComponent<Transform>();

        auto transform = glm::translate(glm::mat4{1.f}, transform_component.m_Position);
        transform[0][0] *= transform_component.m_Scale.x;
        transform[1][1] *= transform_component.m_Scale.y;
        transform[2][2] *= transform_component.m_Scale.z;

        transform = glm::rotate(transform, transform_component.m_AxisRotation.y, {0.f, 1.f, 0.f});
        transform = glm::rotate(transform, transform_component.m_AxisRotation.x, {1.f, 0.f, 0.f});
        transform = glm::rotate(transform, transform_component.m_AxisRotation.z, {0.f, 0.f, 1.f});
        return transform;
    }
}; // namespace Engine3D
