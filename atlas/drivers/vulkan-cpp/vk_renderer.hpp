#pragma once
#include <string>
#include <drivers/renderer_context.hpp>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>
#include <drivers/vulkan-cpp/vk_command_buffer.hpp>
#include <drivers/vulkan-cpp/vk_shader_group.hpp>
#include <drivers/vulkan-cpp/vk_pipeline.hpp>
#include <drivers/vulkan-cpp/vk_uniform_buffer.hpp>
#include <vector>
#include <drivers/vulkan-cpp/vk_texture.hpp>
#include <drivers/vulkan-cpp/mesh.hpp>
#include <drivers/vulkan-cpp/vk_uniform_buffer.hpp>
#include <vector>
#include <map>
#include <core/scene/components.hpp>

namespace atlas::vk {
    /**
     * @brief Something to consider for mesh loading.
     * 
     * How will meshes get loaded?
     * 
     * There are a few ways for considering for the base approach, which are:
     * TODO: These are things to think about how this may happen because I want to make this lightweight in the sense the data isnt continuously being modified. Taking a looking at minimizing the loading state of vulkan implementation-specific meshes
     *      * While also making the way how mesh components are being added the same as before
     * - Something to avoid is the entities containing the geometry data itself but being able to reference to their respective geometry data that are getting submitted to the GPU
     * 
     * * Batching ID's into hash table that contains the actual geometry data
     *  * Using ID to search up the mesh loaded and cached into the map, then reuse that geometry whenever the uniforms are changed
     *  * This way we aren't reloading in the same mesh multiple times, treating the hash table as a slot of the mesh contained within that scene
     *  * Potentially std::map<std::string, std::map<std::string, vk::mesh_metadata>> m_geometries
     *      * Idea is the std::string is the geometries within this scene, the data format is: <scene_str, <mesh_str, mesh_metadata>>
    */
    class vk_renderer : public render_context {
    public:
        vk_renderer(const vk_swapchain& p_swapchain, const std::string& p_tag);
        
        ~vk_renderer() override = default;

    private:
        void start_frame(const vk_command_buffer& p_current, const vk_swapchain& p_swapchain_handler) override;
        void post_frame() override;
        void background_color(const std::array<float, 4>& p_color) override;

    private:
        vk_swapchain m_main_swapchain{};
        vk_command_buffer m_current_command_buffer{};
        VkClearColorValue m_color;

        vk_shader_group m_shader_group;
        vk_pipeline m_main_pipeline{};
        uint32_t m_image_count=0;

        // descriptor set and global uniforms for global camera data
        std::vector<vk_uniform_buffer> m_global_uniforms{};
        descriptor_set m_global_descriptor{};

        std::vector<VkDescriptorSetLayout> m_geometry_descriptor_layout;

        // --------------------------------------------
        // Mesh 0 - Viking Room + Viking Room Texture
        // --------------------------------------------
        // mesh m_mesh0;
        // descriptor_set m_descriptor_set1{}; // viking room mesh
        // vk_uniform_buffer m_mesh0_material_ubo{}; // uniform for viking room mesh

        // --------------------------------------------
        // Mesh 1 - Cube + wood texture
        // --------------------------------------------
        // mesh m_mesh1;
        // descriptor_set m_descriptor_set2{}; // cube wood
        // vk_uniform_buffer m_mesh1_material_ubo{}; // material uniform for cube

        // This is for caching any loaded mesh and only modifying this mesh if that entity is there.
        // It is for this vk_renderer to manage
        // std::string = the name of the entity
        // mesh = corresponding to the entity that is being loaded
        std::map<std::string, mesh> m_cached_meshes;


        std::map<std::string, descriptor_set_layout> m_descriptor_table;

        // std::string = entity name
        // descriptor_set for now will represent the material descriptor set
        std::map<std::string, descriptor_set> m_geometry_descriptor;
        descriptor_set_layout m_material_descriptor_layout;



        bool m_begin_initialize=true;
        uint32_t m_current_frame=0;
        camera m_camera;
        glm::mat4 m_model={1.f};
    };
};