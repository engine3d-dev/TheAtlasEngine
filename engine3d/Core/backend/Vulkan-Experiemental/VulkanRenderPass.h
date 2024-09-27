#pragma once
#include <string>
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
         * 
         * @note RenderPasses contains List of resources (attachments)
         * @note Dependencies (Subpasses) - implying how you use subpasses when dealing with multiple buffers like shadow passing/g-buffers.
         * @note Subpasses encompasses all draw commands that uses the same input and output resources
         * 
         * @note Formats
         * @note Loading/Storing Methods
         * @note Layouts
         * 
         * @note Renderpasses in an abstract manner refers to its input/output resources
         * 
         * @note Each operation has it's own render pass. Such as deferred rendering, shadow passes would each have their own renderpass
         * @note TODO -- Should think of another way to get VulkanRenderPass working.
         * 
         * @param VK_SUBPASS_CONTENTS_INLINE specifies contents of subpass will be recorded inline in primary cmd buffer and secondary cmd buffer must not be executed within subpass
        */
        class VulkanRenderPass{
        public:
            VulkanRenderPass() = default;
            VulkanRenderPass(const std::string& debugName, const std::vector<VkAttachmentDescription>& p_Attachments = {});

            /**
             * @note TODO --- should have Begin() and End() functions be outside of these class instances
             * @note Since some of the concepts within Vulkan requires there to be vkCmdBegin* and vkCmdEnd*, I'm thinking 
             */
            void Begin(VkCommandBuffer buffer, VkSubpassContents contents);
            void End(VkCommandBuffer buffer);

            void SetCurrentFramebuffer(uint32_t idx);

            VkRenderPassBeginInfo& GetBeginInfo();

            VkFramebuffer& GetFramebuffer(uint32_t idx);

            VkRenderPass& GetVkRenderPass();

        private:
            std::vector<VkAttachmentDescription> m_AttachmentDescriptions;
            std::vector<VkFramebuffer> m_Framebuffers;
            VkRenderPass m_RenderPass;

        };
    };
};