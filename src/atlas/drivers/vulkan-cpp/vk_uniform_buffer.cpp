#include <drivers/vulkan-cpp/vk_uniform_buffer.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <drivers/vulkan-cpp/helper_functions.hpp>

namespace atlas::vk {
    vk_buffer create_uniform_buffer(uint32_t p_size) {
        vk_buffer_info uniform_info = {
            .device_size = p_size,
            .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            .memory_property_flag = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        };
        vk_buffer buffer_data = create_buffer(uniform_info);

        return buffer_data;
    }

    vk_uniform_buffer::vk_uniform_buffer(uint32_t p_size_in_bytes) {
        m_driver = vk_context::driver_context();

        m_uniform_buffer_data = create_uniform_buffer(p_size_in_bytes);
    }

    VkBuffer vk_uniform_buffer::get([[maybe_unused]] uint32_t p_index) {
        return m_uniform_buffer_data.handler;
    }

    void vk_uniform_buffer::update(const void* p_data, size_t p_size_in_bytes) {
        void* mapped = nullptr;
        vk_check(vkMapMemory(m_driver,
                             m_uniform_buffer_data.device_memory,
                             0,
                             p_size_in_bytes,
                             0,
                             &mapped),
                 "vkMapMemory",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);
        memcpy(mapped, p_data, p_size_in_bytes);
        vkUnmapMemory(m_driver, m_uniform_buffer_data.device_memory);
    }

    void vk_uniform_buffer::destroy() {
        vkFreeMemory(m_driver, m_uniform_buffer_data.device_memory, nullptr);
        vkDestroyBuffer(m_driver, m_uniform_buffer_data.handler, nullptr);
    }
};