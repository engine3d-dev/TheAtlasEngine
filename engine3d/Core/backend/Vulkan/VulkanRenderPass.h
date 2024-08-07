#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    namespace vk{
        /**
         * @name Vulkan Render Pass
         * @note Renderpasses are where rendering occurs
         * @note VkRenderPass encapsulates the idea of state needed to setup targeted rendering, state of images that'll be rendered.
         * @note Renderpass is concept known to vulkan for allowing driver to know more about state of the images you render
         * @note Framebuffers will be requiring a specific render pass. 
        */
        class VulkanRenderPass{
        public:
        void InitializeRenderPass();

        //! @note Renderpass are needed first before getting framebuffers working
        void InitializeFramebuffers();

        VkRenderPass GetRenderPassInstnace();

        void BeginPass(uint32_t swapchainIdx, VkCommandBuffer command, VkSubpassContents flags, VkPipeline pipeline);
        void EndPass(VkCommandBuffer command);

        void BeginPass();
        void EndPass();

        private:
            std::vector<VkFramebuffer> m_Framebuffers;
            VkRenderPass m_RenderPass;
            
        };
    };
};