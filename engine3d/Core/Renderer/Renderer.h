#pragma once
#include <Core/backend/Vulkan/Shaders/VulkanShaderPipelineBuilder.h>
#include <Core/backend/Vulkan/VulkanCommandBuffer.h>
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

        //! @note TODO -- Move this into swapchain.
        //! @note We have the final pass within swapchain.
        static VkRenderPass& GetCurrentRenderPass();

        static void RenderTriangle(const vk::VulkanCommandBuffer &cmdBuffer, engine3d::vk::VulkanShaderPipelineBuilder &shaderPipeline);

        //! @note Per indices will represent our rgba values
        /**
         * @param 0 =   r
         * @param 1 =   g
         * @param 2 =   b
         * @param 3 =   a
        */
        static void SetBackgroundColor(const std::array<float, 4>& rgba);
        /**
         * @name Submit
         * @note Submits our tasks that are defined as callback function object
         * @note This way we can have them be stored in a storage buffer.
         * @note Storage buffer will handle how each tasks may operate.
         * @note How this function operates?
         * @note When you submit a function you do any vulkan-related logic then that function object gets rendered in the precedence order that the time each tasks been submitted.
         * @note Which also vary at when each tasks get submitted
         * @note The table to think about this is the following : Submit(=>[task(), task(), task(), imguiTask(), ....] -> swapchain -> presentation
         * @note TODO -- Some kind of storage API to help with managing all of our tasks as part of our submission flow.
         * @note Since submitting work can vary on flags and various other factors this submission allows for flexibility in types of commands to submit and how these commands are prepared.
         * @note frames in flight - means to ensure syncrhonization between host and device (CPU and GPU)
         * 
        
        Example - If this was to happen in UICore or UI-related abstraction around imgui.

        // This is a quick API flow example of what the behavior would be utilizing the Renderer::Submit call
        // Allowing to submit per-task based commands per submission.
        // Allowing to have a generic API for submitting different tasks to render passes before sending them to the swapchain.

        // Just adding this to be clear where this code may reside at.
        // In reality the instead of creating a pointer to our current instance, we might directly modify to using the "this" keyword
        UICore* core = this;
        Renderer::Submit([core](){
            VulkanCommandBuffer buffer = VulkanCommandBuffer::Create(...);
            VulkanRenderPass renderPass = VulkanRenderPass(1, buffer.data());

            vk::BeginRenderPass(renderPass, renderPass.BeginInfo(), flags);
            vk::Begin(buffer, flags);
            vk::End(buffer);
            vk::EndRenderPass(renderPass);
        });

        */
        template<typename Command>
        static void Submit(const Command&& function){
        }

        //! @note Will want a function to flush our scene when giving the renderer all of our context
        //! @note For now this flush function is how we flush our current primitives to get them presented onto the screen.
        //! @note TODO -- Move the logic that acquires next image into the vulkan abstraction
        static void FlushScene();

        static VkClearValue* GetClearColor();

        // std::unique_ptr<Renderer> m_Test;
    private:
    };
};