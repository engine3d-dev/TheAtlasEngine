#pragma once
#include <drivers/renderer_context.hpp>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>
#include <drivers/vulkan-cpp/mesh.hpp>
#include <vector>
#include <map>
#include <string>
#include <vulkan-cpp/uniform_buffer.hpp>
#include <vulkan-cpp/pipeline.hpp>
#include <vulkan-cpp/descriptor_resource.hpp>
#include <vulkan-cpp/shader_resource.hpp>
#include <vulkan-cpp/renderpass.hpp>

namespace atlas::vk {
    /**
     * @brief Something to consider for mesh loading.
     *
     * How will meshes get loaded?
     *
     * There are a few ways for considering for the base approach, which are:
     * TODO: These are things to think about how this may happen because I want
     * to make this lightweight in the sense the data isnt continuously being
     * modified. Taking a looking at minimizing the loading state of vulkan
     * implementation-specific meshes
     *      * While also making the way how mesh components are being added the
     * same as before
     * - Something to avoid is the entities containing the geometry data itself
     * but being able to reference to their respective geometry data that are
     * getting submitted to the GPU
     *
     * * Batching ID's into hash table that contains the actual geometry data
     *  * Using ID to search up the mesh loaded and cached into the map, then
     * reuse that geometry whenever the uniforms are changed
     *  * This way we aren't reloading in the same mesh multiple times, treating
     * the hash table as a slot of the mesh contained within that scene
     *  * Potentially std::map<std::string, std::map<std::string,
     * vk::mesh_metadata>> m_geometries
     *      * Idea is the std::string is the geometries within this scene, the
     * data format is: <scene_str, <mesh_str, mesh_metadata>>
     */
    class vk_renderer : public render_context {
    public:
        vk_renderer(const window_settings& p_settings, uint32_t p_image_size, const std::string& p_tag);

        ~vk_renderer() override = default;

    private:
        void start_frame(const ::vk::command_buffer& p_current,
                         const window_settings& p_settings,
                         const VkRenderPass& p_renderpass,
                         const VkFramebuffer& p_framebuffer,
                         const glm::mat4& p_proj_view) override;
        void background_color(const std::array<float, 4>& p_color) override;

        [[nodiscard]] ::vk::command_buffer active_command() const override { return m_current_command_buffer; }

        void post_frame() override;

    private:
        VkDevice m_device=nullptr;
        vk_physical_driver m_physical;
        glm::mat4 m_proj_view;

        // vk_swapchain m_main_swapchain{};
        window_settings m_window_extent;
        ::vk::command_buffer m_current_command_buffer{};
        VkClearColorValue m_color;

        uint32_t m_image_count = 0;
        ::vk::shader_resource m_shader_group;
        ::vk::pipeline m_main_pipeline;
        ::vk::descriptor_resource m_global_descriptors;
        std::vector<VkDescriptorSetLayout> m_sets_layouts;

        std::map<uint32_t, mesh> m_cached_meshes;
        ::vk::uniform_buffer m_global_uniforms;
        std::map<uint32_t, std::map<std::string, ::vk::descriptor_resource>> m_mesh_descriptors;

        bool m_begin_initialize = true;
        uint32_t m_current_frame = 0;
        glm::mat4 m_model = { 1.f };
    };
};
