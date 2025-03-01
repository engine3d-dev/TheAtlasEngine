#pragma once
#include <drivers/render_pass.hpp>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

namespace atlas::vk{
    class VulkanRenderPass : public renderpass{
    public:
        VulkanRenderPass(const renderpass_properties& p_Properties);
        
    protected:
        void BeginRenderPass() override;
        void EndRenderPass() override;

    private:
        VkRenderPass m_RenderPassHandler;
    };
};