#include <drivers/vulkan/vulkan_renderpass.hpp>
#include <vulkan/vulkan_core.h>

namespace atlas::vk {

    static VkFormat to_vulkan_format(ImageFormat Format) {
        switch (Format) {
            case ImageFormat::RGBA16:
                return VK_FORMAT_R8G8B8A8_SINT;
            default:
                break;
        }

        return VK_FORMAT_UNDEFINED;
    }

    //! @note Used to convert from our enum of ImageLayout into vulkan or an
    //! api-specific equivalent of the supported image layouts
    static VkImageLayout to_image_layout(ImageLayout Layout) {
        VkImageLayout layout_value = VK_IMAGE_LAYOUT_UNDEFINED;

        switch (Layout) {
            case ImageLayout::IMAGE_OPTIMAL:
                return VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
            default:
                break;
        }

        return layout_value;
    }

    /*
        Usage:
            - Helper function for bitwise-OR for knowing what our image layout
       is going to be
            - Also used to
    */
    static VkImageLayout RetrievedImageLayout(VkImageLayout Layout) {
        int new_layout = VK_IMAGE_LAYOUT_UNDEFINED;
        new_layout |= VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;

        if (Layout & VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL) {
            // Layout |= VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
        }

        return (VkImageLayout)new_layout;
    }

    VulkanRenderPass::VulkanRenderPass(
      const renderpass_properties& p_Properties) {
        auto attachments = p_Properties.attachments;

        //! @note If we have attachments specified for this render-pass
        //! @note If there are not render pass attachments, then we assign
        //! defaulted attachments for basic-uses
        if (!attachments.empty()) {
        }

        VkAttachmentReference depth_attachment = {
            .attachment = 0, .layout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL
        };
    }

    void VulkanRenderPass::BeginRenderPass() {}

    void VulkanRenderPass::EndRenderPass() {}
};