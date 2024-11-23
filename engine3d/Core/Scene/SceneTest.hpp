#pragma once
#include <Core/internal/Vulkan2Showcase/VulkanModel.hpp>
#include <Core/Core.hpp>
#include <entt/entity/fwd.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #include <Core/ApplicationManager/Scene.hpp>

namespace engine3d{

    //! @note Defines what our scene objects'll be.
    class SceneObjectTutorial{
        using id_t = unsigned int;
    public:
        //! @note Creating a transform component for our scene object
        struct Transform2DComponent{
            glm::vec3 Position{};
            glm::vec3 scale = {1.f, 1.f, 1.f};
            glm::vec3 rotation;

            glm::mat4 mat4(){
                auto transform = glm::translate(glm::mat4{1.f}, Position);
                transform = glm::rotate(transform, rotation.y, {0.f, 1.f, 0.f});
                transform = glm::rotate(transform, rotation.x, {1.f, 0.f, 0.f});
                transform = glm::rotate(transform, rotation.z, {0.f, 0.f, 1.f});

                transform = glm::scale(transform, scale);
                return transform;
            }
        };

        struct RigidBody2dComponent {
            glm::vec2 velocity;
            float mass{1.0f};
        };
        static SceneObjectTutorial Create();

        id_t GetID() const { return m_Id; }

        void SetModel(Ref<vk::VulkanModel>& p_Model){
            m_Model = p_Model;
        }

        void SetRotation(glm::vec3& p_Rotation){
            m_Transform2D.rotation = p_Rotation;
        }

        void SetColor(glm::vec3 p_Color){
            m_Color = p_Color;
        }

        void SetTranslation(Transform2DComponent p_Transformation){
            m_Transform2D = p_Transformation;
        }

        Ref<vk::VulkanModel>& GetModel() { return m_Model; }
        glm::vec3 GetColor() { return m_Color; }
        Transform2DComponent& GetTransform() { return m_Transform2D; }

        Transform2DComponent m_Transform2D;
        RigidBody2dComponent RigidBodyComponent;

    private:
        SceneObjectTutorial(id_t p_Id);

    private:
        Ref<vk::VulkanModel> m_Model;
        glm::vec3 m_Color;

        id_t m_Id;
    };

    // class SceneObjectTutorial2{
    //     using id_t = unsigned int;
    // public:
    //     static SceneObjectTutorial2 Create(Scene* p_Scene);

    //     id_t GetID() const { return m_Id; }

    //     void SetModel(Ref<vk::VulkanModel>& p_Model){
    //         m_Model = p_Model;
    //     }

    //     void SetRotation(glm::vec3& p_Rotation){
    //         m_Transform2D.rotation = p_Rotation;
    //     }

    //     void SetColor(glm::vec3 p_Color){
    //         m_Color = p_Color;
    //     }

    //     void SetTranslation(Transform2DComponent p_Transformation){
    //         m_Transform2D = p_Transformation;
    //     }

    //     Ref<vk::VulkanModel>& GetModel() { return m_Model; }
    //     glm::vec3 GetColor() { return m_Color; }
    //     Transform2DComponent& GetTransform() { return m_Transform2D; }

    //     Transform2DComponent m_Transform2D;
    //     RigidBody2dComponent RigidBodyComponent;




    // private:
    //     SceneObjectTutorial2(id_t p_Id, Scene* p_Scene);

    // private:
    //     Ref<vk::VulkanModel> m_Model;
    //     glm::vec3 m_Color;
    //     Scene* m_Scene;
    //     id_t m_Id;
    // };
};