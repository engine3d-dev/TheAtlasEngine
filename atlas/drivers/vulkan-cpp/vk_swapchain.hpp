#pragma once
#include <core/utilities/types.hpp>
#include <drivers/vulkan-cpp/vk_driver.hpp>
#include <drivers/vulkan-cpp/vk_command_buffer.hpp>
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <drivers/vulkan-cpp/vk_present_queue.hpp>
#include <drivers/vulkan-cpp/vk_renderpass.hpp>

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
         * @brief Constructs a new vulkan swapchain with a window settings to apply to this swapchain
         * @param p_surface passing in the surface handler for swapchain uses
        */
        vk_swapchain(const VkSurfaceKHR& p_surface, const window_settings& p_settings);
 
        //! @brief Constructs new swapchain with specified window-integration configuration settings
        void configure(const window_settings& p_settings);

        //! @brief Used for when swapchain is resized then only reconfigured properties to get resizability
        // NOTE: Should remove this. Leaving this here is because currently deciding what might be an approach for setting up swapchain
        // void reconfigure(const window_settings& p_settings);

        //! @return uint32_t the next available image to present acquired
        uint32_t read_acquired_image();

        //! @return current active command buffer being processed
        [[nodiscard]] vk_command_buffer active_command_buffer(uint32_t p_frame) const {
            return m_swapchain_command_buffers[p_frame];
        }

        [[nodiscard]] VkFramebuffer active_framebuffer(uint32_t p_frame) const {
            return m_swapchain_framebuffers[p_frame];
        }

        [[nodiscard]] VkRenderPass swapchain_renderpass() const { return m_swapchain_main_renderpass; }

        [[nodiscard]] window_settings settings() const { return m_window_settings; }

        [[nodiscard]] uint32_t image_size() const { return m_image_size; }

        [[nodiscard]] surface_properties data() const { return m_surface_properties; }

        void destroy();

        void submit(const VkCommandBuffer& p_command);

        operator VkSwapchainKHR() const { return m_swapchain_handler; }

        operator VkSwapchainKHR() { return m_swapchain_handler; }

        void present(const uint32_t& p_current_frame);

    private:
        void recreate();
        void on_create();

    private:
        vk_physical_driver m_physical{};
        vk_driver m_driver{};
        VkSurfaceKHR m_current_surface_handler=nullptr;
        VkSwapchainKHR m_swapchain_handler=nullptr;
        VkExtent2D m_swapchain_extent{};
        window_settings m_window_settings{};
        
        uint32_t m_image_size = 0;

        //! @brief Render Pass Specifications
        VkRenderPass m_color_renderpass=nullptr;

        VkSurfaceKHR m_current_surface=nullptr;
        surface_properties m_surface_properties{};
        std::vector<vk_command_buffer> m_swapchain_command_buffers{};
        std::vector<VkFramebuffer> m_swapchain_framebuffers{};

        //! @brief setting up images
        std::vector<vk_image_handle> m_swapchain_images{};
        std::vector<vk_image> m_swapchain_depth_images{};
    
        //! @brief Main swapchain renderpass
        //! @brief color renderpass
        // VkRenderPass m_swapchain_renderpass=nullptr;
        vk_renderpass m_swapchain_main_renderpass{};

        vk_present_queue m_present_to_queue{};
    };

};