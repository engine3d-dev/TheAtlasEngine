#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Core/SceneManagment/Components/SPComps/Transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <numbers>

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
        GetComponent<Transform>().m_Position = p_Position;
    }

    void SceneObject::SetRotation(const glm::vec3& p_Rotation){
        GetComponent<Transform>().m_AxisRotation = p_Rotation;
    }

    glm::vec3 SceneObject::GetRotation(){
        return GetComponent<Transform>().m_AxisRotation;
    }

    glm::mat4 SceneObject::toMat4(){
        auto transform_component = GetComponent<Transform>();

        auto transform = glm::translate(glm::mat4{1.f}, transform_component.m_Position);

        glm::quat quaterion{
            transform_component.m_QuaterionRot.w,
            transform_component.m_QuaterionRot.x,
            transform_component.m_QuaterionRot.y,
            transform_component.m_QuaterionRot.z,
        };

        transform *= glm::mat4_cast(quaterion);
        transform = glm::scale(transform, transform_component.m_Scale);
        return transform;
    }
}; // namespace Engine3D
