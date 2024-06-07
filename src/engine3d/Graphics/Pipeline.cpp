#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Graphics/Pipeline.h>

namespace Engine3D{
    //! @note Setting our current API defaulted to OpenGL
    static API g_CurrentAPI = API::OPENGL;
    static PipelineSettings g_Properties;

    void Pipeline::InitializePipeline(API api){
        g_CurrentAPI = api;

        if(g_CurrentAPI == API::OPENGL) CoreLogInfo("OpenGL API Enabled");
        else if(g_CurrentAPI == API::VULKAN) CoreLogInfo("Vulkan API Enabled");
        else if(g_CurrentAPI == API::DIRECTX11) CoreLogInfo("DirectX11 API Enabled");
        else if(g_CurrentAPI == API::DIRECT12) CoreLogInfo("DirectX12 API Enabled");
        else CoreLogTrace("Unsupported API was set in Pipline::InitializePipeline(...)");

        //! @note TODO --- Move this OpenGL-code from pipeline
        // What allows for testing
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
    }

    API Pipeline::CurrentAPI() {
        return g_CurrentAPI;
    }

    PipelineSettings Pipeline::GetProperties(){
        return g_Properties;
    }

    void Pipeline::SetClearColor(const glm::vec4& color){
        g_Properties.m_WindowRefreshFrameColor = color;
        glClearColor(g_Properties.m_WindowRefreshFrameColor.r, g_Properties.m_WindowRefreshFrameColor.g, g_Properties.m_WindowRefreshFrameColor.b, g_Properties.m_WindowRefreshFrameColor.a);
    }

    glm::vec2 Pipeline::GetViewportSize() { return g_Properties.m_ViewportSize; }

    void Pipeline::SetViewport(uint32_t w, uint32_t h){
        glViewport(0, 0, w, h);
        g_Properties.m_ViewportSize = {w, h};
    }

    void Pipeline::RefreshFrame(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};