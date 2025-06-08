#pragma once
#include <cstdint>
#include <vulkan/vulkan.h>
#include <drivers/vulkan-cpp/vk_types.hpp>

namespace atlas::vk {

    class vk_uniform_buffer {
    public:
        vk_uniform_buffer() = default;
        
        vk_uniform_buffer(uint32_t p_size_in_bytes);

        VkBuffer get(uint32_t p_index);

        void update(const void* p_data, size_t p_size_in_bytes);

        operator VkBuffer() const { return m_uniform_buffer_data.handler; }

        operator VkBuffer() { return m_uniform_buffer_data.handler; }

        void destroy();

    private:
        VkDevice m_driver=nullptr;
        vk_buffer m_uniform_buffer_data{};
    };
};