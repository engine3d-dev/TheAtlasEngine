#pragma once
#include <cstdint>
#include <vulkan/vulkan.h>
#include <drivers/vulkan-cpp/vk_types.hpp>

namespace atlas::vk {

    /**
     * @brief Vulkan Command Buffers
     *
     * Use this to create a command buffer according to vulkan specifications
     *
     * vulkan command buffer is how vulkan submits commands specifically
     * directly or indirectly to the GPU via `vkQueueSubmit`
     *
     * Command buffers in vulkan only know about the queue family they are
     * associated with, their primary flag, and allocation operation associated
     * with.
     */
    class vk_command_buffer {
    public:
        vk_command_buffer() = default;

        //! @brief Constructs new command buffer
        vk_command_buffer(const command_buffer_settings& p_settings);

        //! @brief Indication when for this command buffer to start recording (writing) commands to this command buffer
        void begin(const VkCommandBufferUsageFlags& p_usage_flags);

        //! @brief Indication when the command buffer to stop recording (write) commands to this buffer.
        void end();

        //! @brief Enable to check for command buffers if they are valid
        [[nodiscard]] bool is_valid() const { return (m_command_buffer_handler != nullptr); }

        //! @brief API for explicitly invoking cleanup on the command buffer
        void destroy();

        operator VkCommandBuffer() const {
            if (m_begin_end_count != 2) {
            }
            return m_command_buffer_handler;
        }
        operator VkCommandBuffer() { return m_command_buffer_handler; }

    private:
        VkDevice m_driver = nullptr;
        VkCommandBuffer m_command_buffer_handler = nullptr;
        VkCommandPool m_command_pool = nullptr;
        uint32_t m_begin_end_count = 0;
    };
};