#pragma once
// #include <engine3d/Core/ApplicationInstance.hpp>
#include <Core/ApplicationInstance.hpp>
#include <Core/internal/Vulkan2Showcase/Shaders/VulkanShader.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d{

    //! @note Our actual editor application
    //! @note TODO -- Having a cenrtal application that will be used across both editor and the game(being developed)
    //! @note So when they're packaged, they can be shipped under one application excluding editor stuff and only things relevant to the game.
    class EditorApplication : public ApplicationInstance{
    public:
        EditorApplication(const std::string& debugName = "Engine3D Editor");
        virtual ~EditorApplication();
    private:
        //! @note TODO -- Probably have a cleanup handler for this
        //! @note Such as WorldCleanup() or some API to make sure we can cleanly deallocate and delete things...
        // void ShutdownEditor();
        void UpdateThisApplicationInstance() override;
        
    private:
        //! @note Editor application, Engine, UI Layer.
        float m_LastFrameTime = 0.0f;
        //! @note Essentially our VulkanShader is VkPipeline, ShaderModule all in one.
        vk::VulkanShader m_Shader;
        VkPipelineLayout m_PipelineLayout;
        VkCommandPool m_CommandPool;
        std::vector<VkCommandBuffer> m_CommandBuffers;
    };
};