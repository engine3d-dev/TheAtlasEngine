#pragma once
#include <span>
#include <vulkan/vulkan.h>
#include <drivers/vulkan-cpp/vk_types.hpp>

namespace atlas::vk {
    class vk_index_buffer {
    public:
        vk_index_buffer() = default;
        vk_index_buffer(const std::span<uint32_t>& p_indices);

        void bind(const VkCommandBuffer& p_current);

        void draw(const VkCommandBuffer& p_current);

        void destroy();

        [[nodiscard]] size_t size() const { return m_indices_count; }

    private:
        VkDevice m_driver = nullptr;
        uint32_t m_indices_count = 0;
        vk_buffer m_index_buffer_handler{};
    };
};