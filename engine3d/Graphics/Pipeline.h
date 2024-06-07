#pragma once
#include <engine3d/Core/core.h>
#include "glm/glm.hpp"

namespace Engine3D{
    enum class API : uint8_t {
        UNSUPPORTED,
        OPENGL,
        VULKAN,
        DIRECTX11, DIRECT12
    };

    //! @note Pipeline Settings used for specifying how we may want to pass data through our pipeline.
    struct PipelineSettings{
        //! @note Window background color used for every time when we refresh our screen
        //! @note Set to a default color
        glm::vec4 m_WindowRefreshFrameColor=glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        glm::vec2 m_ViewportSize; // Our viewport dimensions set
    };

    /**
     * @name Pipeline
     * @note Used to interact with our various pipelines OpenGL, Vulkan, and DirectX
    */
    class Pipeline{
    public:
        static void InitializePipeline(API api);

        //! @note Should give us what our currently enabled graphics API's implementation
        static API CurrentAPI();

        //! @note Retrieving our pipelining data
        static PipelineSettings GetProperties();

        static glm::vec2 GetViewportSize();

        //! @note TODO ---- Maybe move these to OpenGL or Vulkan-Specific implementations
        //! @note Window-specific frame data set for each frame on the screen.
        static void SetClearColor(const glm::vec4& color);

        static void SetViewport(uint32_t w, uint32_t h);
        static void RefreshFrame();
    };

};