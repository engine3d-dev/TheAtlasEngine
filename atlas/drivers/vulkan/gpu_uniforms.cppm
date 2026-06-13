module;

#include <span>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

export module atlas.drivers.vulkan:gpu_uniforms;

import vk;


export namespace atlas {
    struct gpu_mesh_data {
        vk::vertex_buffer vertex;
        vk::index_buffer index;
        uint32_t index_count=0;
        uint32_t instance=1;
        uint32_t first_index=0;
        uint32_t vertex_offset=0;
        uint32_t first_instance=0;
        bool has_indices_buffer=false;
        uint32_t vertices_size = 0;
        uint32_t indices_size=0;
    };

    struct push_constant_data {
        uint64_t scene_address=0;
        uint64_t model_mat_array_address=0;
        uint32_t model_idx=0;
        uint32_t diffuse_idx=0;
        uint32_t specular_idx=0;
        uint64_t point_light_address;
    };

    struct scene_uniforms {
        glm::mat4 view=glm::mat4(1.f);
        glm::mat4 proj=glm::mat4(1.f);
        glm::vec4 camera_pos=glm::vec4(1.f);
    };

    struct objects_uniform {
        std::span<glm::mat4> model_matrices;
    };

    struct gpu_material {
        uint64_t diffuse_idx=0;
        uint64_t specular_idx=0;
    };

    struct gpu_point_light {
        glm::vec4 position; // this is provided by the transform
        glm::vec4 color = { 1.f, 1.f, 1.f, 1.f };
        float attenuation = 1.f;
        float constant = 1.f;
        float linear = 1.f;
        float quadratic = 1.f;

        glm::vec4 ambient = glm::vec4(1.f);
        glm::vec4 diffuse = glm::vec4(1.f);
        glm::vec4 specular = glm::vec4(1.f);
    };

     struct light_scene_ubo {
        alignas(16) uint32_t num_lights=0;
        alignas(16) std::array<gpu_point_light, 1000> point_lights{};
    };


    /**
     * 
     * @brief slot is the index into the texture array to retrieve this specific texture data
     * 
     * vk::texture is the texture data to configure the GPU-visible image resource
    */
    struct gpu_image {
        uint64_t slot=0;
        vk::texture texture_data;
    };
};