#pragma once
#include <core/utilities/types.hpp>
#include <drivers/vulkan-cpp/vk_driver.hpp>
#include <vulkan-cpp/command_buffer.hpp>
#include <vulkan-cpp/framebuffer.hpp>
#include <vulkan-cpp/renderpass.hpp>
#include <vulkan-cpp/sample_image.hpp>
#include <vulkan-cpp/surface.hpp>
#include <vulkan-cpp/device_present_queue.hpp>

namespace atlas::vk {
    /**
     * @name vk_swapchain
     * @brief High-level abstraction of a vulkan swapchain
     * @brief ref<window> will return the vk_swapchain directly
     * @brief ref<window> returning vk_swapchain will need to be changed
     */
    class vk_swapchain {
    public:
        vk_swapchain() = default;

        /**
         * @brief Constructs a new vulkan swapchain with a window settings to
         * apply to this swapchain
         * @param p_surface passing in the surface handler for swapchain uses
         */
        vk_swapchain(const VkSurfaceKHR& p_surface,
                     const window_settings& p_settings);

        //! @return uint32_t the next available image to present acquired
        uint32_t read_acquired_image();

        //! @return current active command buffer being processed
        [[nodiscard]] ::vk::command_buffer active_command(uint32_t p_frame_index) {
            return m_swapchain_command_buffers[p_frame_index];
        }

        [[nodiscard]] VkFramebuffer active_framebuffer(uint32_t p_frame) const {
            return m_swapchain_framebuffers[p_frame];
        }

        [[nodiscard]] VkRenderPass swapchain_renderpass() const {
            return m_final_renderpass;
        }

        [[nodiscard]] window_settings settings() const {
            return m_window_settings;
        }

        [[nodiscard]] uint32_t image_size() const { return m_image_size; }

        [[nodiscard]] surface_properties data() const {
            return m_surface_properties;
        }

        void destroy();

        void submit(const VkCommandBuffer& p_command);

        operator VkSwapchainKHR() const { return m_swapchain_handler; }

        operator VkSwapchainKHR() { return m_swapchain_handler; }

        void present(const uint32_t& p_current_frame);

    private:
        void invalidate();
        void create();

    private:
        vk_physical_driver m_physical{};
        vk_driver m_driver{};
        VkSurfaceKHR m_current_surface_handler = nullptr;
        VkSwapchainKHR m_swapchain_handler = nullptr;
        VkExtent2D m_swapchain_extent{};
        window_settings m_window_settings{};

        uint32_t m_image_size = 0;

        VkSurfaceKHR m_current_surface = nullptr;
        surface_properties m_surface_properties{};
        std::vector<::vk::command_buffer> m_swapchain_command_buffers{};
        std::vector<::vk::framebuffer> m_swapchain_framebuffers;

        //! @brief setting up images
        std::vector<::vk::sample_image> m_swapchain_images;
        std::vector<::vk::sample_image> m_swapchain_depth_images;

        ::vk::renderpass m_final_renderpass;

        ::vk::device_present_queue m_present_to_queue;
    };

};
