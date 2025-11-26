#pragma once
#include <vulkan-cpp/texture.hpp>

namespace atlas::vk {
    struct vertex_input {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normals;
        glm::vec2 uv;

        bool operator==(const vertex_input& other) const {
            return position == other.position and color == other.color and
                   uv == other.uv and normals == other.normals;
        }
    };

    //! @brief Going to remove this
    //! @brief This is being used by descriptor sets and vk_renderer
    //! TODO: Modify vk_descriptor_set and vk_renderer and add uniforms.hpp
    //! inside renderer/ (dir) for used across vk_renderer and the renderer
    struct camera_ubo {
        glm::mat4 projection{ 1.f };
        glm::mat4 view{ 1.f };
    };

    //! @brief Just for testing purposes for sending this struct over to the
    //! shader
    struct global_ubo {
        glm::mat4 mvp = { 1.f };
    };

    /**
     * @brief material is going to define properties about how a scene object
     * itself gets rendered
     *
     */
    struct geometry_uniform {
        glm::mat4 model{ 1.f };
        glm::vec4 color{ 1.f };
        // std::vector<std::string> texture_paths;
    };
    
    /**
     * @brief Represents a vulkan specific mateiral to pass in two textures and
     * offset for handling shininess
     */
    struct material_uniform {
        float shininess = 1.f;
        ::vk::texture diffuse;
        ::vk::texture specular;
    };
};