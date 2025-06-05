#pragma once
#include <string>
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <vulkan/vulkan.h>
#include <drivers/vulkan-cpp/vk_driver.hpp>

namespace atlas::vk {

    /**
     * @brief defines high-level properties of the attachment to create the renderpass with
    */
    struct attachment {

    };

    /**
     * @brief renderpass in vulkan contains the attachments, subpass dependencies, and dependencies on reliant of the renderpass in-use.
     * 
    */
    class vk_renderpass {
    public:
        vk_renderpass() = default;

        //! @brief Invokes configure when constructing new renderpass
        vk_renderpass(const vk_renderpass_options& p_renderpass_options);

        //! @brief contsructs new renderpass and configures it with the following attachments
        //! @brief Used when needing to reconstruct the renderpass state
        //! @brief Usable when resizing event occurs and renderpass state of the handler needs to match the swapchain during resizing
        void configure(const vk_renderpass_options& p_renderpass_options);

        void destroy();


        operator VkRenderPass() const { return m_renderpass_handler; }

        operator VkRenderPass() { return m_renderpass_handler; }

    private:
        vk_driver m_driver{};
        vk_renderpass_options m_renderpass_options{};
        VkRenderPass m_renderpass_handler=nullptr;
    };
};