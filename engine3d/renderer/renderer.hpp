#pragma once
#include <vulkan/vulkan.hpp>

namespace engine3d{
    /**
     * @name Renderer
     * @note Is our actual public API for handling a lot of our renderer.
     * @note The way we have this setup is the renderer will have a renderer context.
     * @note Renderer context and what that means is defined by its implementation.
     * @note Meaning if we eventually handle multiple graphics API's that may be performant on specific platforms like DirectX.
     *      The renderer context's public API's wont change but the operations behind it's implementation would be different by the API being used and set
     * @note TODO: Some thing I will consider because currently at runtime these graphic API's can be set, this is something that should be set at compile-time
    */
    class Renderer{
    public:
        static void Initialize();

        //! @note using begin/end semantics for specifying that when we've reached the end of the frame is when we flush and render our contexts that is proviedd.
        static void Begin();
        static void End();
    };
};