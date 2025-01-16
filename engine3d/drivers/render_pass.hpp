#pragma once

namespace engine3d{
    /*
    Each render pass may have a framebuffer corresponding to it

    This allows us to specify specific post-processing effects

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

    RenderPass main_pass = RenderPass(main_spec);


    // Now lets say we wanted to apply lighting
    // There can be a render pass for that too
    RenderPass lighting_pass = RenderPass({
        .width = 1,
        .height = 1,
        .attachments = {
            RGBA8,
            RGBA16,
            DEPTH24_STENCIL8,
        }
    });

    // In this context, lets say we want to apply shadow-mapping using a depth property
    RenderPass lighting_pass = RenderPass({
        .width = 1,
        .height = 1,
        .attachments = {
            RGBA8,
            RGBA16,
            DEPTH24_STENCIL8,
        }
    });


    */
    class RenderPass{
    public:
    };
};