#pragma once
#include <cstdint>
#include <vulkan/vulkan.h>
#include <drivers/vulkan-cpp/vk_types.hpp>

namespace atlas::vk {

    class vk_uniform_buffer {
    public:
        vk_uniform_buffer() = default;

        vk_uniform_buffer(uint32_t p_size_in_bytes);

        //! @return bytes of the uniform data stored with this uniform buffer
        //! handler
        [[nodiscard]] uint32_t size_bytes() const { return m_size_bytes; }

        void update(const void* p_data);

        operator VkBuffer() const { return m_uniform_buffer_data.handler; }

        operator VkBuffer() { return m_uniform_buffer_data.handler; }

        void destroy();

    private:
        VkDevice m_driver = nullptr;
        uint32_t m_size_bytes = 0;
        vk_buffer m_uniform_buffer_data{};
    };
};