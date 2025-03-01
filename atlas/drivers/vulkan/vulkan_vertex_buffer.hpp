#pragma once
#include <drivers/vertex_buffer.hpp>
#include <vulkan/vulkan_core.h>

namespace atlas::vk{
    class vk_vertex_buffer : public vertex_buffer{
    public:
        vk_vertex_buffer(const std::vector<vertex>& p_vertices);
        virtual ~vk_vertex_buffer(){}

    protected:

        void bind_vertex_buffer(const VkCommandBuffer& p_command_buffer) override;

        void render_vertex_buffer(const VkCommandBuffer& p_command_buffer) override;

    private:
        VkBuffer m_vertex_buffer_handler;
        VkDeviceMemory m_vertex_buffer_device_memory;
        uint32_t m_count; // count of vertices
    };
};