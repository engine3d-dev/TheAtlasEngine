#pragma once
#include <cstdint>
#include <vulkan/vulkan.h>

namespace atlas::vk {

    /**
     * @param Primary
     * submission: immediately to vulkan queue for executing on device
     * execution: vkQueueSubmit primarily submits primary command buffers
     *
     * @param Seconary
     * submission: Cannot be submitted directly to vulkan queue
     * execution: Indirect execution by being called from primary command
     * buffers using `vkCmdExecuteCommands`
     * 
     * When executed within render-pass instance, they inherit
     * renderpass/subpass state from primary command buffer
     * 
     * Can be allocated and recorded in parallel, which can leverage
     * devices that support multi-core processing
     * 
     * Have independent lifetimes that can be managed independently of
     * primary command buffers, allowing for more flexible resource management
     */
    enum command_buffer_levels : uint8_t {
        Primary = 0,
        Secondary = 1,
        MaxEnum = 2
    };

    /**
     * @brief settings for specifying command buffers to construct
     * 
     * @param Levels are for specifying the kind of command buffer being
     * constructed
     * 
     * queue_index specifies the queue family associated with this
     * command buffer
     * 
     * pool_flag in the context of the command buffers are used to
     * control memory allocation behavior within the command buffer pool to
     * allocate
     *
     * @param VK_COMMAND_POOL_CREATE_TRANSIENT_BIT Indicates command buffers
     * allocated from pool will be short-lived, meaning they'll reset or freed
     * quickly
     * @param VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT Allows individual
     * command buffers allocated from the pool to be reset to their initial
     * state using `vkResetCommandBuffer`
     * @param VK_COMMAND_POOL_CREATE_PROTECTED_BIT Designates command buffers
     * allocated from the pool as protective command buffers, implying they are
     * stored in protected memory and access to them is restricted to authorized
     * operations
     */
    struct command_buffer_settings {
        command_buffer_settings(uint32_t p_queue_family,
                                const command_buffer_levels& p_levels,
                                const VkCommandPoolCreateFlagBits& p_pool_flags)
          : levels(p_levels)
          , queue_index(p_queue_family)
          , pool_flag(p_pool_flags) {}

        command_buffer_levels levels;
        uint32_t queue_index = -1;
        VkCommandPoolCreateFlagBits pool_flag;
    };

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
        [[nodiscard]] bool is_valid() const { return false; }

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