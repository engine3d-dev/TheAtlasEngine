#pragma once
#include <Core/GraphicDrivers/GraphicSwapchain.hpp>
#include <Core/GraphicDrivers/Shader.hpp>
#include <Core/Scene/SceneObject.hpp>
#include <Core/Renderer/RendererBackend.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    class VulkanRenderer : public RendererBackend{
    public:
        VulkanRenderer(const std::string& p_DebugName);

    private:
        void StartFrame() override;
        void FinishFrame() override;

        template<typename UFunction>
        void DrawMesh(const UFunction& p_Function){
            
        }

    private:
        VkCommandBuffer CurrentCommandBuffer();

    private:
        Ref<GraphicSwapchain> m_Swapchain;
        VkPipeline m_Pipeline;
        std::vector<VkCommandBuffer> m_CommandBuffers;
        uint32_t m_CurrentFrameIndex = -1;
        bool m_IsFrameStarted = false;

        //! @note Editor application, Engine, UI Layer.
        float m_LastFrameTime = 0.0f;
        //! @note Essentially our VulkanShader is VkPipeline, ShaderModule all in one.
        Ref<Shader> m_Shader;
        VkPipelineLayout m_PipelineLayout;
        VkCommandPool m_CommandPool;
        std::vector<SceneObject> m_GameObjects;
    };
};