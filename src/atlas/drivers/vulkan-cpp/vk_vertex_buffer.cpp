#include <drivers/vulkan-cpp/vk_vertex_buffer.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <drivers/vulkan-cpp/utilties/utils.hpp>
#include <array>

namespace atlas::vk {

    vk_vertex_buffer::vk_vertex_buffer(
      const std::span<vertex_input>& p_vertices) {
        m_driver = vk_context::driver_context();
        m_vertices_count = p_vertices.size();
        m_vertices_byte_size_count = p_vertices.size_bytes();

        /*
        1.) Creating staging buffer
            * VK_BUFFER_USAGE_STORAGE_BUFFER_BIT sets becaused required for
        buffers represented for storing vertices
        */
        vk_buffer_info staging_info = {
            .device_size = (uint32_t)m_vertices_byte_size_count,
            .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
                     VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            .memory_property_flag = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                    VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
        };

        vk_buffer staging_buffer = create_buffer(staging_info);

        // 2.) Maps the staging buffer via through vkMap/vkUnmap
        write(staging_buffer, p_vertices);

        // 3.) Now creating our actual vertex buffer handler
        vk_buffer_info vertex_info = {
            .device_size = m_vertices_byte_size_count,
            .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                     VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            .memory_property_flag = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        };

        m_vertex_handler = create_buffer(vertex_info);

        // 4. Copying vertices from staging buffer to the actual vertex buffer
        // handlers
        copy(staging_buffer, m_vertex_handler, m_vertices_byte_size_count);

        // 5.) Cleanup staging buffer
        vkFreeMemory(m_driver, staging_buffer.device_memory, nullptr);
        vkDestroyBuffer(m_driver, staging_buffer.handler, nullptr);
    }

    void vk_vertex_buffer::bind(const VkCommandBuffer& p_current) {
        std::array<VkBuffer, 1> handlers = { m_vertex_handler.handler };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(p_current, 0, 1, handlers.data(), offsets);
    }

    void vk_vertex_buffer::draw(const VkCommandBuffer& p_current) {
        vkCmdDraw(p_current, m_vertices_count, 1, 0, 0);
    }

    void vk_vertex_buffer::destroy() {
        free_buffer(m_driver, m_vertex_handler);
    }
};