#include <drivers/vulkan-cpp/vk_renderpass.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <core/engine_logger.hpp>

namespace atlas::vk {

    vk_renderpass::vk_renderpass(
      const vk_renderpass_options& p_renderpass_options)
      : m_renderpass_options(p_renderpass_options) {
        m_driver = vk_context::driver_context();

        configure(m_renderpass_options);
    }

    void vk_renderpass::configure(
      const vk_renderpass_options& p_renderpass_options) {
        // We reconfigure this swapchain to use the following set renderpass
        // option Set this to false to use the options this renderpass
        // constructed with Set to true to configure with the following new
        // renderpass options
        if (p_renderpass_options.cache) {
            m_renderpass_options = p_renderpass_options;
            console_log_fatal("renderpass cache set to true!!!");
        }

        VkRenderPassCreateInfo renderpass_ci = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .attachmentCount =
              static_cast<uint32_t>(m_renderpass_options.attachments.size()),
            .pAttachments = m_renderpass_options.attachments.data(),
            .subpassCount = static_cast<uint32_t>(
              m_renderpass_options.subpass_descriptions.size()),
            .pSubpasses = m_renderpass_options.subpass_descriptions.data(),
            .dependencyCount =
              static_cast<uint32_t>(m_renderpass_options.dependencies.size()),
            .pDependencies = m_renderpass_options.dependencies.data()
        };

        VkResult res = vkCreateRenderPass(
          m_driver, &renderpass_ci, nullptr, &m_renderpass_handler);
        
        vk_check(res, "vkCreateRenderPass", __FILE__, __LINE__, __FUNCTION__);


    }

    void vk_renderpass::destroy() {
        vkDestroyRenderPass(m_driver, m_renderpass_handler, nullptr);
    }
};