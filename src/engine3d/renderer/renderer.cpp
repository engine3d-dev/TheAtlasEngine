#include "engine_logger.hpp"
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

    void Renderer::RenderSceneObjects(std::map<std::string, Ref<SceneNode>>& p_SceneObjects){
        return g_RendererBackend->RenderSceneObjects(p_SceneObjects);
    }
};