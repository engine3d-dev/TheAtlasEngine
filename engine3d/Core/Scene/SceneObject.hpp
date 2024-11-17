#pragma once
#include <Core/internal/Vulkan2Showcase/VulkanModel.hpp>
#include <Core/Core.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace engine3d{
    //! @note Creating a transform component for our scene object
    struct Transform2DComponent{
        glm::vec2 Translation{};
        glm::vec2 scale = {1.f, 1.f};
        float rotation;

        glm::mat2 mat2() {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotation_mat = {{c, s}, {-s, c}};

            glm::mat2 scale_mat{{scale.x, .0f}, {.0f, scale.y}};
            // return glm::mat2{1.f};
            // return scale_mat;
            return rotation_mat * scale_mat;
        }
    };

    //! @note Defines what our scene objects'll be.
    class SceneObject{
        using id_t = unsigned int;
    public:
        static SceneObject Create();

        id_t GetID() const { return m_Id; }

        void SetModel(Ref<vk::VulkanModel>& p_Model){
            m_Model = p_Model;
        }

        void SetRotation(float& p_Rotation){
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

    private:
        SceneObject(id_t p_Id);

    private:
        Ref<vk::VulkanModel> m_Model;
        glm::vec3 m_Color;

        id_t m_Id;
    };
};