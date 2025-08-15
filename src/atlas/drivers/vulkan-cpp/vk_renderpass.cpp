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

    /*
    vk_renderpass::vk_renderpass(const renderpass_options&
    p_renderpass_configuration) { m_driver = vk_context::driver_context();

            configure(p_renderpass_configuration);
    }
    */

    /*
    void vk_renderpass::configure([[maybe_unused]] const renderpass_options&
    p_renderpass_options) { std::vector<VkAttachmentDescription>
    attachments(p_renderpass_options.color_attachments.size());
            std::vector<VkAttachmentReference>
    color_attachment_references(p_renderpass_options.color_attachments.size());
            std::vector<VkAttachmentReference>
    depth_attachment_references(p_renderpass_options.color_attachments.size());

            for(uint32_t i = 0; i <
    p_renderpass_options.color_attachments.size(); i++) { renderpass_attachment
    attachment = p_renderpass_options.color_attachments[i]; attachments[i] = {
                            .flags = 0,
                            .format = to_vk_format(attachment.format),
                            .samples =
    to_vk_sample_count_bits(attachment.sample_count), .loadOp =
    to_vk_attachment_load(attachment.load), .storeOp =
    to_vk_attachment_store(attachment.store), .stencilLoadOp =
    to_vk_attachment_load(attachment.stencil_load), .stencilStoreOp =
    to_vk_attachment_store(attachment.stencil_store), .initialLayout =
    to_vk_image_layout(attachment.initial), .finalLayout =
    to_vk_image_layout(attachment.finalize)
                    };

                    if(attachment.type == renderpass_type::color) {
                            color_attachment_references[i] = {
                                    .attachment = i,
                                    .layout =
    to_vk_image_layout(attachment.layout)
                            };
                    }
                    else if(attachment.type == renderpass_type::depth) {
                            depth_attachment_references[i] = {
                                    .attachment = i,
                                    .layout =
    to_vk_image_layout(attachment.layout)
                            };
                    }
            }
            VkSubpassDescription subpass_description = {
                    .flags = 0,
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount = 0,
        .pInputAttachments = nullptr,
        .colorAttachmentCount =
    static_cast<uint32_t>(color_attachment_references.size()),
        .pColorAttachments = color_attachment_references.data(),
        .pResolveAttachments = nullptr,
        .pDepthStencilAttachment = depth_attachment_references.data(), // enable
    depth buffering .preserveAttachmentCount = 0, .pPreserveAttachments =
    nullptr
            };


    }
    */

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

        vk_check(res, "vkCreateRenderPass");
    }

    void vk_renderpass::destroy() {
        vkDestroyRenderPass(m_driver, m_renderpass_handler, nullptr);
    }
};
