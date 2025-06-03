#pragma once
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <drivers/vulkan-cpp/vk_driver.hpp>

namespace atlas::vk {

    /**
     * @brief Handles submissions to the specific queue for presentation
     * @brief This queue is for submitting presentation images through this queue that gets displayed onto the swapchain
     * @brief Used by the vk_swapchain to present currently processed command buffers to the swapchain
    */
    class vk_present_queue {
    public:
        vk_present_queue() = default;
        
        /**
         * @brief Constructs new presentation queue
         * @param p_swapchain presentation queue needs a swapchain of where the images are being rendered to
         * @param p_queue_options specifies options on the types of queue family index, and queue associated index to which queue family handler for submitting work to
        */
        vk_present_queue(const VkSwapchainKHR& p_swapchain, const vk_queue_options& p_queue_options);

        //! @brief Submitting current command buffer with async enabled
        void submit_immediate_async(const VkCommandBuffer& p_command);
        
        //! @brief Submitting current command buffer with async disabled
        void submit_immediate_sync(const VkCommandBuffer& p_command);

        // These are ??? atm because the idea is we submit commands to the presentation queue's then flush them at once
        //! @brief Then again, this probably won't be needed
        // void submit(const VkCommandBuffer& p_command);

        // void execute();

        //! @param p_current_frame current frame of image presentable to the swapchain
        void present_frame(const uint32_t& p_current_frame);

        //! @brief Ensures that operations finished within this queue before processing new frames
        //! @brief Synchronization handling
        void wait_idle();

        //! @return uint32_t next index to the presentable image for the current frame
        uint32_t acquired_frame();

        //! @return bool if the presentation queue handler is valid or not
        [[nodiscard]] bool is_alive() const { return (m_present_queue_handler != nullptr); }

        //! @brief Explicitly invoking cleanup for the presentation queue
        void destroy();

        operator VkQueue() { return m_present_queue_handler; }
        
        operator VkQueue() const { return m_present_queue_handler; }

    private:
        vk_driver m_driver{};
        VkQueue m_present_queue_handler=nullptr;
        VkSwapchainKHR m_swapchain_handler=nullptr;
        VkSemaphore m_render_completed_semaphore=nullptr;
        VkSemaphore m_present_completed_semaphore=nullptr;
    };
};