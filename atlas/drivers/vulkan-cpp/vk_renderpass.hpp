#pragma once
#include <string>
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <vulkan/vulkan.h>
#include <drivers/vulkan-cpp/vk_driver.hpp>

namespace atlas::vk {

    /**
     * vk_renderpass
     * @brief defines a renderpass operation
     * 
     * Handles
     * 
    */
    class vk_renderpass {
    public:
        vk_renderpass() = default;

        //! @brief Invokes configure when constructing new renderpass
        vk_renderpass(const vk_renderpass_options& p_renderpass_options);

        // TODO: Add this constructor once renderpass_options is implemented to handle VkAttachmentReference
        // vk_renderpass(const renderpass_options& p_renderpass_configuration);

        //! @brief contsructs new renderpass and configures it with the following attachments
        // Used when needing to reconstruct the renderpass state
        // Usable when resizing event occurs and renderpass state of the handler needs to match the swapchain during resizing
        void configure(const vk_renderpass_options& p_renderpass_options);

        //! @brief contsructs new renderpass and configures it with the following attachments 
        // TODO: Implement this function to setup renderpass attachments and handle VkAttachmentReference
        // void configure(const renderpass_options& p_renderpass_options);

        void destroy();

        operator VkRenderPass() const { return m_renderpass_handler; }

        operator VkRenderPass() { return m_renderpass_handler; }

    private:
        vk_driver m_driver{};
        vk_renderpass_options m_renderpass_options{};
        VkRenderPass m_renderpass_handler=nullptr;
    };
};