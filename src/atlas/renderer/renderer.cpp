#include <drivers/renderer_backend.hpp>
#include <renderer/renderer.hpp>

namespace atlas {
    //! @note Only this renderer should have ownership of this renderer context
    //! @note Renderer context is  a graphics-agnostic renderer that provides an
    //! API to interact with our agnostic graphcis API's such as Vulkan, DirectX
    //! (if we plan to), etc.
    scope<render_context> g_RendererBackend = nullptr;

    void renderer::initialize() {
        g_RendererBackend = render_context::initialize("Renderer");
    }

    void renderer::begin() {
        return g_RendererBackend->begin();
    }

    void renderer::end() {
        return g_RendererBackend->end();
    }

    uint32_t renderer::get_current_frame() {
        return render_context::get_current_frame();
    }
};