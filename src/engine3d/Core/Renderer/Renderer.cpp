#include <Core/Renderer/Renderer.hpp>

namespace engine3d{
    static RendererSettings g_Settings;

    RendererSettings& Renderer::GetSettings(){
        return g_Settings;
    }
};