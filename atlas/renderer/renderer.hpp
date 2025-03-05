#pragma once
#include <core/core.hpp>
#include <core/scene/scene.hpp>
#include <vulkan/vulkan.hpp>

namespace atlas {
    /**
     * @name Renderer
     * @note Is our actual public API for handling a lot of our renderer.
     * @note The way we have this setup is the renderer will have a renderer
     * context.
     * @note Renderer context and what that means is defined by its
     * implementation.
     * @note Meaning if we eventually handle multiple graphics API's that may be
     * performant on specific platforms like DirectX. The renderer context's
     * public API's wont change but the operations behind it's implementation
     * would be different by the API being used and set
     * @note TODO: Some thing I will consider because currently at runtime these
     * graphic API's can be set, this is something that should be set at
     * compile-time
     */
    class scene_object;
    class renderer {
    public:
        //! @note Used to initialize
        static void initialize();

        static uint32_t get_current_frame();

        /**
        @note Begin()
            - Indicates to the renderer the preparation before the next frame's
        called

        @note End()
            - At the end of frame the renderer will offload the tasks onto the
        GPU

        TODO: Implement Batch Rendering (Minimal Optimizaiton)
            - Batch Rendering is a technique on rendering in batches
            - Meaning when we call `Render*` functions, we are not calling draw
        commands per function call of `Render*`
            - Batch Rendering is how we render in batches. Meaning when we call
        Render*, we submit to via those Render* functions. Then when we reached
        the end of the frame via End() function.
            - Then End() is reached before we indicate the scene is rendered, we
        render all our modified objects
        */
        static void begin();
        static void end();
    };
};