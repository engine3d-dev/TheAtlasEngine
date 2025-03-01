#pragma once
#include <initializer_list>
#include <span>
#include <string>
#include <vector>
#include <core/core.hpp>

namespace atlas{
    /*
    Each render pass may have a framebuffer corresponding to it

    This allows us to specify specific post-processing effects


    // Here are information the render pass needs to know that are required for the driver to handle our images
    - attachment reference
        - Used to inform 
    - attachment description
    - subpass description
    - subpass dependency

    // Here are parameters for the user on what they may want to only modify

    - types of attachments per render pass
    - width, height per render pass
    - specify usable attachments per render pass (use-case post-processing)

    Usage: This is how we would be passing the data to our abstraction layer for specifying multiple different render passes
    RenderPassSpecifications main_spec = {
        .width = 1,
        .height = 1,
        .attachments = {
            RED,
            RGBA8,
            RGBA16,
            DEPTH24_STENCIL8
        }
    };

    renderpass main_pass = renderpass(main_spec);


    // Now lets say we wanted to apply lighting
    // There can be a render pass for that too
    renderpass lighting_pass = renderpass({
        .width = 1,
        .height = 1,
        .attachments = {
            RGBA8,
            RGBA16,
            DEPTH24_STENCIL8,
        }
    });

    // In this context, lets say we want to apply shadow-mapping using a depth property
    renderpass lighting_pass = renderpass({
        .width = 1,
        .height = 1,
        .attachments = {
            RGBA8,
            RGBA16,
            DEPTH24_STENCIL8,
        }
    });
    */

    /*

        // using depth buffer as a depth-stencil attachment
        VkAttachmentReference (depth attachment)
            attachment = 1
            layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL

            layout is VkImageLayout
                - which is an enumeration thats used for specifying how image (a texture) should be accessed by GPU
                - Defining pixel data in memory and what operations are allowed on it, such as reading from color attachment in the render-pass
                    or sampling it in a shader

        
        VkAttachmentDescription (color_attachment) Handles the Following:
            format = get_swapchain_format()
            samples = VK_SAMPLE_COUNT_1_BIT
            loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR
            store_op = VK_ATTACHMENT_STORE_OP_STORE
            stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE
            stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE
            initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
            finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        


    
    */

    //! @note Used to specify what attachment type for per render pass
    //! @note Usage: Format that is defaulted to throughout the vulkan abstraction
    //! @note Meaning not just used for attachment formats, also image formats
    enum class ImageFormat{
        RED, RGBA8, RGBA16,
        DEPTH24_STENCIL8,
        DEFAULT = DEPTH24_STENCIL8
    };

    //! @note Enum for indicating state of what access shaders have
    //! @note Usage: as a bitfield
    enum class ShaderAccessOperation{
        DONT_CARE,
        LOAD,
        STORE,
        DEFAULT = DONT_CARE
    };

    //! @note Used for specifying the type of image layout for getting accessed
    /*
    
    Image Layout

    COLOR_ATTACHMENT
        - Only be used as a color or resolve attachment in VkFramebuffer/Framebuffer
    
    DEPTH_STENCIL_ATTACHMENT_READ_WRITE
        - Layout for bothj depth/stencil aspects of a depth/stencil format image allowing read/write access as depth/stencil attachment
        - Equivalent to, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL and VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL.
    
    DEPTH_STENCIL_READ
        - Specifies layout both depth/stencil to only have read-only access or shaders as sampled image, combined imagef/sampler, or input attachment
        - Equivalent to VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL and VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL
    
    TRANSFER_SRC
        - MUST only be used as src image of transfer command (for vulkan check definitions of VK_PIPELINE_STAGE_TRANSFER_BIT)
    
    TRANSFER_DST
        - MUST only be used as destination image of transfer command.
        - Layout is invalid only for image subresources of images created with VK_IMAGE_USAGE_TRANSFER_DST_BIT (TRANSAFER_DST_BIT)

    PREINITIALIZED
        - Specifies image's memory defined layout and can be populated by data, but that it has not been initialized by driver yet
        - Image memory cannot be transitioned into this layout.
        - Layout can be used as `initialLayout` in vulkan's context of VkImageCreateInfo
    
    STENCIL_READ_WRITE
        - Specifies read/write access to stencil aspect as stencil attachment
        - Read-only depth attachment
        - Equivalent to VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL and VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL

    DEPTH_READ_WRITE
        - Specifies layout for depth/stencil format images allowing read/write access to depth aspect as depth attachment
        - Read only access to stencil aspect as stencil attachment
        - Equivalent to VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL and VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL

    */
    enum class ImageLayout{
        UNDEFINED,
        IMAGE_OPTIMAL,
        COLOR_ATTACHMENT,                       // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL 
        DEPTH_STENCIL_ATTACHMENT_READ_WRITE,    // VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL 
        DEPTH_STENCIL_READ,                     // VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
        TRANSFER_SRC,                           // VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
        TRANSFER_DST,                           // VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        PREINITIALIZED,                         // VK_IMAGE_LAYOUT_PREINITIALIZED
        STENCIL_READ_WRITE_DEPTH_READ_ONLY,     // VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL
        DEPTH_READ_WRITE_STENCIL_READ_ONLY,     // VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL
    };


    enum class SubpassContents : int{
        INLINE = 0,
        SECONDARY_COMMAND_BUFFERS = 1,
        INLINE_AND_SECONDARY_COMMAND_BUFFERS_KHR = 2,
        INLINE_AND_COMMAND_BUFFERS_EXT = 3
    };


    enum class PipelineStageFlags{
        COLOR_ATTACHMENT,
    };

    //! @note To specify our what attachments our render passes will take
    /*
    
    "Color Buffer": {
        .width = 10,
        .height = 10,
        .attachments = {
            RGBA_U32 // equivalent to R8G8B8_UNORM
        }
    };
    
    */
    struct renderpass_properties{
        uint32_t width;
        uint32_t height;
        
        std::span<ImageFormat> attachments;
        std::span<ShaderAccessOperation> operations;
    };

    /**
        @name Render Pass
        @note Responsible for configuring pipelines output to the framebuffer
        @note Graphics Pipeline needs to know this on creation in order to compile frag shader properly
        @note In vulkan VkRenderPass is needed to be specified in VkFramebufferCreateInfo
        @note This is because 


        renderpass main_rp = renderpass::create();

    */
    class renderpass{
    public:
        // renderpass();

        // graphics-agnostic implementation of a render pass creation
        static ref<renderpass> create(const renderpass_properties& properties);

        //! @note When a render pass should start per frame
        void Begin() { return BeginRenderPass(); }

        //! @note When a render pass should end at the end per frame
        void End() { return EndRenderPass();}

    private:
        //! @note Implementation-detail on when renderpasses begin/end that is graphic-api agnostic
        virtual void BeginRenderPass() = 0;
        virtual void EndRenderPass() = 0;
    };
};