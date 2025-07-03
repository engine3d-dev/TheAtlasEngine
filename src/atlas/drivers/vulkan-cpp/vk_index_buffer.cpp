#include <drivers/vulkan-cpp/vk_index_buffer.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <drivers/vulkan-cpp/utilties/utils.hpp>

namespace atlas::vk {
    vk_index_buffer::vk_index_buffer(const std::span<uint32_t>& p_indices) {
        m_driver = vk_context::driver_context();
        m_indices_count = static_cast<uint32_t>(p_indices.size());

        vk_buffer_info index_buffer_info = {
            .device_size = (uint32_t)p_indices.size_bytes(),
            .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            .memory_property_flag = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                    VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
        };

        m_index_buffer_handler = create_buffer(index_buffer_info);

        write(m_index_buffer_handler, p_indices);
    }

    void vk_index_buffer::bind(const VkCommandBuffer& p_current) {
        vkCmdBindIndexBuffer(
          p_current, m_index_buffer_handler.handler, 0, VK_INDEX_TYPE_UINT32);
    }

    void vk_index_buffer::draw(const VkCommandBuffer& p_current) {
        vkCmdDrawIndexed(p_current, m_indices_count, 1, 0, 0, 0);
    }

    void vk_index_buffer::destroy() {
        if (m_index_buffer_handler.device_memory != nullptr) {
            vkFreeMemory(
              m_driver, m_index_buffer_handler.device_memory, nullptr);
        }

        if (m_index_buffer_handler.handler != nullptr) {
            vkDestroyBuffer(m_driver, m_index_buffer_handler.handler, nullptr);
        }
    }
};