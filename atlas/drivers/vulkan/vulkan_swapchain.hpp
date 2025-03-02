#pragma once
#include <drivers/swapchain.hpp>
#include <drivers/vulkan/vulkan_driver.hpp>
#include <vulkan/vulkan.hpp>


namespace atlas::vk {
    /**
     * @name Vulkan swapchain
     * @note Defines the vulkan-backend implementation of a swapchain.
     */
    class vk_swapchain : public swapchain {
    public:
        virtual ~vk_swapchain() {}
        //! @note TODO: Change this into RendererConfig
        static constexpr uint32_t MaxFramesInFlight = 2;

        vk_swapchain() = default;
        vk_swapchain(vk_physical_driver p_physical,
                     vk_driver p_driver,
                     VkSurfaceKHR p_surface);

        static void resize_reset();
        static bool is_resized();

    private:
        void on_create(uint32_t p_width, uint32_t p_height);

    private:
        //! @note Graphic swapchain virtual implementation functions
        void resize_status(bool p_is_resized_signaled) override;

        bool swapchain_resized() override;
        void recreate_swapchain(uint32_t p_width, uint32_t p_height) override;
        VkSwapchainKHR vk_swapchain_handler() override;
        VkRenderPass read_swapchain_renderpass() override;
        VkFormat& read_swapchain_format() override;

        uint32_t images_size() const override;
        VkFramebuffer read_framebuffer(uint32_t idx) override;
        VkImageView read_image_view(uint32_t index) override;
        VkExtent2D read_swapchain_extent() override;
        void submit_and_write_command_buffer(
          VkCommandBuffer* p_command_buffers) override;
        uint32_t read_acquire_next_frame() override;
        uint32_t current_frame_per_tick() override;

    private:
        //! @note Helper functions or various operations.
        VkPresentModeKHR select_compatible_present_mode(
          const VkPresentModeKHR& p_request_mode,
          const std::vector<VkPresentModeKHR>& p_modes);
        VkExtent2D select_valid_extent(
          const VkSurfaceCapabilitiesKHR& p_surface_capabilities);

        //! @note Search Memory Type
        uint32_t select_memory_type(
          VkPhysicalDeviceMemoryProperties p_memory_properties,
          uint32_t p_type_filter,
          VkMemoryPropertyFlags p_property_flag);

        /**
         * @note Selecting specific type of format.
         * @param VkPhysicalDevice is used to extract the depth format and check
         * if it is available on our current GPU.
         */
        VkFormat select_depth_format(const VkPhysicalDevice& p_physical);

        //! @note Helper function to get the format types we need.
        VkFormat select_supported_format(VkPhysicalDevice p_physical,
                                         const std::vector<VkFormat>& p_formats,
                                         VkImageTiling p_tiling,
                                         VkFormatFeatureFlags p_feature_flags);

    private:
        int m_width = -1;
        int m_height = -1;
        bool m_is_resized_requested = false;
        VkQueue m_presentation_queue = nullptr;
        VkSurfaceKHR m_current_surface = nullptr;

        struct SwapchainImage {
            VkImage Image;
            VkImageView ImageView;
        };

        struct SwapchainDepthImage {
            VkImage Image;
            VkImageView ImageView;
            VkDeviceMemory DeviceMemory;
        };

        //! @note Images that are either in-use for images or depth images.
        std::vector<SwapchainImage> m_swapchain_images;
        std::vector<SwapchainDepthImage> m_swapchain_depth_images;
        std::vector<VkFramebuffer> m_swapchain_framebuffers;

        VkSwapchainKHR m_swapchain = nullptr;
        VkSurfaceFormatKHR m_surface_format;
        VkPresentModeKHR m_presentation_mode;
        uint32_t m_presentation_index = -1;
        VkExtent2D m_swapchain_extent;

        //! @note Setting up Command Buffers
        // VkCommandBuffer m_CommandBufferForSwapchain;
        std::vector<VkCommandBuffer>
          m_swapchain_command_buffers; // command buffers per swapchain
        VkCommandPool m_swapchain_command_pool;

        VkRenderPass m_swapchain_renderpass;

        //! @note Semaphores to signal when frames finished or current frame
        //! being rendered has been completed.
        std::vector<VkSemaphore>
          m_semaphores_images_available; // semaphores for when images are
                                         // available
        std::vector<VkSemaphore>
          m_semaphores_render_completed; // semaphores when working on the
                                         // current frame has been completed

        std::vector<VkFence>
          m_swapchain_fences; // fences for when frames in flight
        std::vector<VkFence>
          m_swapchain_in_flight_fences; // images fences for when we currently
                                        // have images currently in flight

        vk_physical_driver m_physical;
        vk_driver m_driver;

        //! @note Current frame that we are at in the application from the start
        //! of the app.
        size_t m_current_frame_index = 0;

        //! @note Acts as our image index.
        uint32_t m_current_image_index =
          0; // contains the index of the image we are currently working through
    };
};