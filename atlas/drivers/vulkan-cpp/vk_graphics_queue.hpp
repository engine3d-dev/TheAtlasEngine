#pragma once
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <drivers/vulkan-cpp/vk_driver.hpp>

namespace atlas::vk {
    /**
     * @brief graphics queue mainly used for submitting to the graphics family
     * specific queue
     *
     * Graphic Queues are used for submitting commands via rendering commands
     * directly or indirectly to the GPU
     *
     * Single graphics queue can only perform compute and transfer operations as
     * well
     *
     * Primarily used for rendeirng logic commands processing
     */
    class vk_graphics_queue {
    public:
        vk_graphics_queue() = default;

        vk_graphics_queue(const vk_queue_options& p_queue_options);

        void immediate_submit_sync(const VkCommandBuffer& p_command_buffer);

        void immediate_submit_async(const VkCommandBuffer& p_command_buffer);

        void wait_idle();

        void destroy();

        operator VkQueue() { return m_graphics_queue_handler; }

        operator VkQueue() const { return m_graphics_queue_handler; }

    private:
        vk_physical_driver m_physical{};
        vk_driver m_driver{};
        VkQueue m_graphics_queue_handler = nullptr;

        VkSemaphore m_render_completed_semaphore = nullptr;
        VkSemaphore m_present_completed_semaphore = nullptr;
    };
};