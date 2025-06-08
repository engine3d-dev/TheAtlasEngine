#include <drivers/vulkan-cpp/vk_graphics_queue.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <drivers/vulkan-cpp/helper_functions.hpp>

namespace atlas::vk {

    vk_graphics_queue::vk_graphics_queue(const vk_queue_options& p_queue_options) {
        //! TODO: Check if we are getting the compute/transfer/graphics queue through physical driver and then pass the result of the queue family index into the get_queue that returns VkQueue
        m_physical = vk_context::physical_driver();
        m_driver = vk_context::driver_context();
        
        m_graphics_queue_handler = m_driver.get_queue(p_queue_options);
        m_render_completed_semaphore = create_semaphore(m_driver);
        m_present_completed_semaphore = create_semaphore(m_driver);

    }

    void vk_graphics_queue::immediate_submit_sync(const VkCommandBuffer& p_command_buffer) {
        VkPipelineStageFlags wait_flags =
          VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &m_present_completed_semaphore,
            .pWaitDstStageMask = &wait_flags,
            .commandBufferCount = 1,
            .pCommandBuffers = &p_command_buffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &m_render_completed_semaphore,
        };

        VkResult res =
          vkQueueSubmit(m_graphics_queue_handler, 1, &submit_info, nullptr);
        vk_check(res, "vkQueueSubmit", __FILE__, __LINE__, __FUNCTION__);
    }

    void vk_graphics_queue::immediate_submit_async(const VkCommandBuffer& p_command_buffer) {
        VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = &p_command_buffer,
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr,
        };

        VkResult res =
          vkQueueSubmit(m_graphics_queue_handler, 1, &submit_info, nullptr);
        vk_check(res, "vkQueueSubmit", __FILE__, __LINE__, __FUNCTION__);
    }

    void vk_graphics_queue::wait_idle() {
        vkQueueWaitIdle(m_graphics_queue_handler);
    }

    void vk_graphics_queue::destroy() {
        // Ensure that the driver before destroying the semaphore that they finish execution beforehand
        vkDeviceWaitIdle(m_driver);
        vkDestroySemaphore(m_driver, m_present_completed_semaphore, nullptr);
        vkDestroySemaphore(m_driver, m_render_completed_semaphore, nullptr);
    }

};