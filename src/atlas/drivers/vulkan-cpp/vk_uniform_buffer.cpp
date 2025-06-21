#include <drivers/vulkan-cpp/vk_uniform_buffer.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <core/engine_logger.hpp>

namespace atlas::vk {

    vk_uniform_buffer::vk_uniform_buffer(uint32_t p_size_in_bytes)
      : m_size_bytes(p_size_in_bytes) {
        m_driver = vk_context::driver_context();

        m_uniform_buffer_data = create_uniform_buffer(p_size_in_bytes);
    }

    void vk_uniform_buffer::update(const void* p_data) {

        void* mapped = nullptr;
        vk_check(vkMapMemory(m_driver,
                             m_uniform_buffer_data.device_memory,
                             0,
                             m_size_bytes,
                             0,
                             &mapped),
                 "vkMapMemory",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);
        memcpy(mapped, p_data, m_size_bytes);
        vkUnmapMemory(m_driver, m_uniform_buffer_data.device_memory);
    }

    void vk_uniform_buffer::destroy() {
        vkFreeMemory(m_driver, m_uniform_buffer_data.device_memory, nullptr);
        vkDestroyBuffer(m_driver, m_uniform_buffer_data.handler, nullptr);
    }
};