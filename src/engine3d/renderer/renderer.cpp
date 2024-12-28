#include <renderer/renderer.hpp>
#include <drivers/renderer_backend.hpp>

namespace engine3d{
    //! @note Only this renderer should have ownership of this renderer context
    //! @note Renderer context is  a graphics-agnostic renderer that provides an API to interact with our agnostic graphcis API's such as Vulkan, DirectX (if we plan to), etc.
    Scope<RendererContext> g_RendererBackend = nullptr;

    void Renderer::Initialize(){
        g_RendererBackend = RendererContext::Initialize("Renderer");
    }

    void Renderer::Begin(){
        return g_RendererBackend->Begin();
    }

    void Renderer::End(){
        return g_RendererBackend->End();
    }
};