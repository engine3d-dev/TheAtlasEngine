#pragma once

#include <array>
#include <vulkan/vulkan.h>

namespace engine3d{
    /**
     * @name Renderer
     * @note This will be Engine3D's renderer abstracted away the Vulkan abstraction
     * @note Renderer's role within Engine3D's core is to have an API that can be used across to submit tasks
     * @note When to make render passes per our renderer draw call.
     * @note When users want to render primitives such as via Renderer::DrawCube(...) per this draw call will submit this into a queue before submission.
     * @note Core renderer for Engine3D.
     * @note NOTE -- May have Renderer2D to enable rendering in 2-dimensions.
     * @note While Renderer will be used to do real-time (RT) rendering and 3D rendering
    */
    class Renderer{
    public:
    
        //! @note This will be how our renderer get's initialized
        static void Initialize();
        //! @note Per indices will represent our rgba values
        /**
         * @param 0 =   r
         * @param 1 =   g
         * @param 2 =   b
         * @param 3 =   a
        */
        static void SetBackgroundColor(const std::array<float, 4>& rgba);

        //! @note Will want a function to flush our scene when giving the renderer all of our context
        static void FlushScene();

        static VkClearValue* GetClearColor();

        template<typename UTask>
        static void Submit(const UTask&& task){
        }

        //! @note This is what will be used to submit our color when refreshing frames.
        // static void RenderClearColor();
        //! @note Testing rendering basic clear frames
        static void RenderBasicClearColor();

        static void Presentation();

    private:
    };
};