// #include "internal/VulkanCpp/Vulkan.hpp"
#include <Core/ApplicationInstance.hpp>
#include <Core/Renderer/Renderer.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    static RendererSettings g_Settings;

    RendererSettings& Renderer::GetSettings(){
        return g_Settings;
    }

    // Ref<vk::VulkanSwapchain> g_CurrentSwapchain = nullptr;
    VkCommandBuffer g_ActiveCommandBuffer = nullptr;
    VkRenderPass g_RenderPass;

    void Renderer::BeginFrame(){
        // g_CurrentSwapchain = ApplicationInstance::GetWindow().GetCurrentSwapchain();
        
    }

    void Renderer::SetBackgroundColor(const std::array<float, 4>& p_Rgba){
        VkClearValue clearColorValue = {{p_Rgba[0], p_Rgba[1], p_Rgba[2], p_Rgba[3]}};
    }


    /*
    //! @note This gets submitted into our renderer queue.
    //! @note Few properties of how submission to the queue will work.

    @note These are the things we (as the renderer) should keep track of
    1.) Queues submission idx to know the queue we are submitting to.
    2.) Specify what queue to allocate to our tasks to.
    3.) Example is at the end of frame we execute all of our queue's
    Example
        static uint32_t s_SubmissionCommandQueueCount = 2;
        //! @note Atomic are just thread-safe counters. Which we will use for reading in queue's that we are submitting into.
        static std::atomic<uint32_t> s_SubmissionQueueIdx = 0; // thread-safe in reading the data from queue's that will be needed to render

        Renderer::BeginFrame(){
            // Probably would do some initialization
            s_CurrentRendererCommandQueue[0] = new RenderCommandQueue();
            s_CurrentRendererCommandQueue[1] = new RenderCommandQueue();
        }

        //! @note In our renderer thread we would utilize this to make sure that in our renderer thread.
        //! @note This would be happening in our next frame. (Where we would do something like double-buffering or something like that)
        void Renderer::NextSubmittedQueueInThread(){
            s_SubmissionQueueIdx = (s_SubmissionQueueIdx + 1) % s_SubmissionCommandQueueCount;
        }

        Renderer::EndFrame(){
            s_CurrentRendererCommandQueue[s_SubmissionQueueIdx]->Execute();
        }

        while(IsActive()){
            Renderer::BeginFrame();

            Renderer::EndFrame();
        }

        //! @note This is what the API, would look.
        Renderer::Submit([](){
        });
    */


    void Renderer::EndFrame(){
    }
};