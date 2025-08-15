#include <drivers/vulkan-cpp/vk_present_queue.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <core/engine_logger.hpp>
#include <iostream>

namespace atlas::vk {

    vk_present_queue::vk_present_queue(
      const VkSwapchainKHR& p_swapchain_handler,
      const vk_queue_options& p_queue_options)
      : m_swapchain_handler(p_swapchain_handler) {

        m_driver = vk_context::driver_context();

        m_present_queue_handler = m_driver.get_queue(p_queue_options);

        m_render_completed_semaphore = create_semaphore(m_driver);
        m_present_completed_semaphore = create_semaphore(m_driver);
    }

    void vk_present_queue::submit_immediate_async(
      const VkCommandBuffer& p_command) const {
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
        vk_check(res, "vkQueueSubmit");
    }

    void vk_present_queue::submit_immediate_sync(
      const VkCommandBuffer& p_command) const {
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
        vk_check(res, "vkQueueSubmit");
    }

    VkResult vk_present_queue::present_frame(const uint32_t& p_current_frame) {
        VkPresentInfoKHR present_info = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &m_render_completed_semaphore,
            .swapchainCount = 1,
            .pSwapchains = &m_swapchain_handler,
            .pImageIndices = &p_current_frame,
        };

        VkResult res =
          vkQueuePresentKHR(m_present_queue_handler, &present_info);
        vk_check(res, "vkQueuePresentKHR");

        return res;
    }

    // TODO: Way to return possible out of date status from function
    //-- This is because swapchain resizing needs to happen ASAP to prevent
    // potentially invalid
    //  operations
    uint32_t vk_present_queue::acquired_frame() {
        uint32_t image_acquired;
        VkResult acquired_next_image_result =
          vkAcquireNextImageKHR(m_driver,
                                m_swapchain_handler,
                                std::numeric_limits<uint64_t>::max(),
                                m_present_completed_semaphore,
                                nullptr,
                                &image_acquired);

        if (acquired_next_image_result == VK_ERROR_OUT_OF_DATE_KHR) {
            console_log_trace("acquired next image out of date!!!");
            m_resize_requested = true;
        }

        vk_check(acquired_next_image_result, "vkAcquireNextImageKHR");

        return image_acquired;
    }

    void vk_present_queue::wait_idle() {
        vkQueueWaitIdle(m_present_queue_handler);
    }

    void vk_present_queue::destroy() {
        // Ensure that the driver before destroying the semaphore that they
        // finish execution beforehand
        vkDeviceWaitIdle(m_driver);
        vkDestroySemaphore(m_driver, m_present_completed_semaphore, nullptr);
        vkDestroySemaphore(m_driver, m_render_completed_semaphore, nullptr);
    }
};
