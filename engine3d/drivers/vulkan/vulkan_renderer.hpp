#pragma once
#include <string>
#include <vulkan/vulkan_core.h>
#include <drivers/renderer_backend.hpp>

namespace engine3d::vk{
    /**
     * @name VulkanRenderer
     * @note vulkan-backend specific renderer
     * @note renderer that is implemented with the vulkan's API
     * @note Provide an API on directly submitting tasks to the vulkan API that gets send directly to the GPU
    */
    class VulkanRenderer : public RendererContext{
    public:
        VulkanRenderer(const std::string& Tag);
        //! @note Implementation-details for sending draw calls.

        VkCommandBuffer GetCurrentCommandBuffer();
        static VkCommandBuffer CurrentCommandBuffer();
        static VkFramebuffer CurrentFramebuffer();
        static uint32_t GetCurrentCommandBufferIndex();

    private:
        void BeginFrame() override;
        void EndFrame() override;
        void DrawScene(Ref<SceneObject> p_SceneContext) override;
        void DrawSceneObjects(const Ref<SceneScope>& p_CurrentScene) override;

        void DrawSceneObject(Ref<SceneObject>& p_CurrentObject) override;

        void DrawObjectWithCamera(Ref<SceneObject>& p_Object, Ref<SceneObject>& p_CameraObject) override;

    private:
        //! @note TODO: Moving VkPipeline/VkPipelineLayout out of the renderer and asbtracting this.
        void InitializeRendererPipeline();
    private:
    };
};