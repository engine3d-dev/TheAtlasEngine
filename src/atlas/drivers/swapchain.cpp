#include <core/application.hpp>
#include <drivers/swapchain.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <drivers/vulkan/vulkan_swapchain.hpp>

namespace atlas {
    ref<swapchain> swapchain::initialize(VkSurfaceKHR p_surface) {
        switch (application::current_api()) {
            case API::VULKAN:
                return create_ref<vk::vk_swapchain>(
                  vk::vk_context::get_current_selected_physical_driver(),
                  vk::vk_context::get_current_driver(),
                  p_surface);
            default:
                break;
        }
        return nullptr;
    }

    void swapchain::on_resize(uint32_t p_width, uint32_t p_height) {
        recreate_swapchain(p_width, p_height);
    }

    uint32_t swapchain::get_images_size() const {
        return images_size();
    }

    bool swapchain::is_resized() {
        return swapchain_resized();
    }

    void swapchain::set_resize_status(bool p_is_resize_signaled) {
        resize_status(p_is_resize_signaled);
    }

    VkFormat& swapchain::get_swapchain_format() {
        return read_swapchain_format();
    }

    VkFramebuffer swapchain::get_framebuffer(uint32_t p_index) {
        return read_framebuffer(p_index);
    }

    VkRenderPass swapchain::get_renderpass() {
        return read_swapchain_renderpass();
    }

    VkImageView swapchain::get_image_view(uint32_t p_index) {
        return read_image_view(p_index);
    }

    VkSwapchainKHR swapchain::get_vk_swapchain() {
        return vk_swapchain_handler();
    }

    VkExtent2D swapchain::get_extent() {
        return read_swapchain_extent();
    }

    void swapchain::submit_command_buffer(VkCommandBuffer* p_command_buffers) {
        return submit_and_write_command_buffer(p_command_buffers);
    }

    //! @note Proceeds to getting next frame.
    uint32_t swapchain::acquire_next_image() {
        return read_acquire_next_frame();
    }

    //! @note m_CurrentFrameIdx
    uint32_t swapchain::get_current_per_tick() {
        return current_frame_per_tick();
    }

    bool swapchain::is_rebuild() {
        return vk::vk_swapchain::is_resized();
    }

    void swapchain::reset_build() {
        vk::vk_swapchain::resize_reset();
    }
};