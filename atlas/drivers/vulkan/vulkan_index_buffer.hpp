#pragma once
#include <drivers/index_buffer.hpp>

namespace atlas::vk {
    class vk_index_buffer : public index_buffer {
    public:
        vk_index_buffer(const std::vector<uint32_t>& p_indices);
        ~vk_index_buffer() override = default;

    private:
        void bind_to_index_buffer(
          const VkCommandBuffer& p_command_buffer) override;
        void render_index_buffer(
          const VkCommandBuffer& p_command_buffer) override;
        [[nodiscard]] bool contains_indices() const override;

    private:
        VkBuffer m_index_buffer_handler;
        VkDeviceMemory m_index_device_buffer_memory;
        uint32_t m_indices_count;
        bool m_has_indices = false;
    };
};