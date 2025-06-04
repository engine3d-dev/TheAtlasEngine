#pragma once
#include <string>
#include <drivers/renderer_context.hpp>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>
#include <drivers/vulkan-cpp/vk_command_buffer.hpp>

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
        vk_renderer(const std::string& p_tag);
        vk_renderer(const vk_swapchain& p_swapchain_handler, const std::string& p_tag);
        
        ~vk_renderer() override;

    private:
        void start_frame(const vk_command_buffer& p_current, const VkFramebuffer& p_current_fb, const VkRenderPass& p_current_renderpass) override;
        void post_frame() override;
        void background_color(const std::array<float, 4>& p_color) override;

    private:
        vk_driver m_driver{};
        vk_swapchain m_swapchain_handler;
        vk_command_buffer m_current_command_buffer{};
        VkClearColorValue m_color;
    };
};