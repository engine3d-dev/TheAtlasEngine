#pragma once
#include "engine3d/Core/backend/Vulkan/Shaders/VulkanShaderCompiler.h"
#include "engine3d/Core/backend/Vulkan/Shaders/VulkanShaderModule.h"
#include "engine3d/Core/backend/Vulkan/VulkanRenderPass.h"
#include <engine3d/Core/EngineLogger.h>
#include <engine3d/Core/BaseApplication.h>
#include <engine3d/Core/backend/Vulkan/Vulkan.h>
#include <engine3d/Core/backend/Vulkan/VulkanDevice.h>
#include <engine3d/Core/backend/Vulkan/VulkanSwapchain.h>
#include <engine3d/Core/backend/Vulkan/VulkanCommandBuffer.h>
#include <engine3d/Core/backend/Vulkan/VulkanCommandQueue.h>
#include <engine3d/Core/Layer.h>
#include <array>

namespace engine3d{

    //! @note Our actual editor application
    class EditorApplication{
    public:
        EditorApplication();
        ~EditorApplication();

        void RunEditor();
    private:
        void InitiateEditor();
        void ShutdownEditor();
        
    private:
        //! @note Editor application, Engine, UI Layer.
        std::array<Layer *, 1> m_Layers;
        float m_LastFrameTime = 0.0f;
        vk::VulkanPipeline m_Pipeline;
        vk::VulkanDevice m_VulkDevice;
        vk::VulkanSwapchain m_Swapchain;
        vk::VulkanCommandBuffer m_CmdBuffer;
        vk::VulkanCommandQueue m_CmdQueue;
        vk::VulkanRenderPass m_RenderPass;
        vk::VulkanShaderModule m_TriangleShaderVertModule;
        vk::VulkanShaderModule m_TriangleShaderFragModule;
        vk::VulkanShaderCompiler m_ShaderPipeline;
    };
};