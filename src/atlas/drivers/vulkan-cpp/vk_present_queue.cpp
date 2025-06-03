#include <drivers/vulkan-cpp/vk_present_queue.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <core/engine_logger.hpp>

namespace atlas::vk {

    vk_present_queue::vk_present_queue(
      const VkSwapchainKHR& p_swapchain_handler,
      const vk_queue_options& p_queue_options)
      : m_swapchain_handler(p_swapchain_handler) {

        console_log_info(
          "vk_present_queue::vk_present_queue begin initialization!!!");
        m_driver = vk_context::driver_context();

        m_present_queue_handler = m_driver.get_queue(p_queue_options);

        m_render_completed_semaphore = create_semaphore(m_driver);
        m_present_completed_semaphore = create_semaphore(m_driver);

        console_log_info(
          "vk_present_queue::vk_present_queue end initialization!!!");
    }

    void vk_present_queue::submit_immediate_async(
      const VkCommandBuffer& p_command) {
        VkPipelineStageFlags wait_flags =
          VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &m_present_completed_semaphore,
            .pWaitDstStageMask = &wait_flags,
            .commandBufferCount = 1,
            .pCommandBuffers = &p_command,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &m_render_completed_semaphore,
        };

        VkResult res =
          vkQueueSubmit(m_present_queue_handler, 1, &submit_info, nullptr);
        vk_check(res, "vkQueueSubmit", __FILE__, __LINE__, __FUNCTION__);
    }

    void vk_present_queue::submit_immediate_sync(
      const VkCommandBuffer& p_command) {
        VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = &p_command,
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr,
        };

        VkResult res =
          vkQueueSubmit(m_present_queue_handler, 1, &submit_info, nullptr);
        vk_check(res, "vkQueueSubmit", __FILE__, __LINE__, __FUNCTION__);
    }

    void vk_present_queue::present_frame(const uint32_t& p_current_frame) {
        VkPresentInfoKHR present_info = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &m_render_completed_semaphore,
            .swapchainCount = 1,
            .pSwapchains = &m_swapchain_handler,
            .pImageIndices = &p_current_frame,
        };

        vk_check(vkQueuePresentKHR(m_present_queue_handler, &present_info),
                 "vkQueuePresentKHR",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);
    }

    uint32_t vk_present_queue::acquired_frame() {
        uint32_t image_acquired;
        VkResult acquired_next_image_result =
          vkAcquireNextImageKHR(m_driver,
                                m_swapchain_handler,
                                std::numeric_limits<uint64_t>::max(),
                                m_present_completed_semaphore,
                                nullptr,
                                &image_acquired);

        vk_check(acquired_next_image_result,
                 "vkAcquireNextImageKHR",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);
        return image_acquired;
    }

    void vk_present_queue::wait_idle() {
        vkQueueWaitIdle(m_present_queue_handler);
    }

    void vk_present_queue::destroy() {
        // Ensure that the driver before destroying the semaphore that they finish execution beforehand
        vkDeviceWaitIdle(m_driver);
        vkDestroySemaphore(m_driver, m_present_completed_semaphore, nullptr);
        vkDestroySemaphore(m_driver, m_render_completed_semaphore, nullptr);
    }
};