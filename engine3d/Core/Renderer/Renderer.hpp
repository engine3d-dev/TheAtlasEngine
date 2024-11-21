#pragma once
#include <Core/Scene/SceneTest.hpp>
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Core/Renderer/RendererSettings.hpp>
#include <array>
#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>

namespace engine3d{
    class Renderer{
    public:
        static void Initialize(const std::string& p_DebugName);

        static VkCommandBuffer BeginFrame();

        //! @note Currently the record command buffers just does what "simple_render_system.hpp/.cpp" does already
        //! @note In the future I'll add API's for submitting draw calls.
        static void RecordCommandBuffers(VkCommandBuffer p_CommandBuffer);
        static void RecordGameObjects(std::vector<SceneObjectTutorial>& p_Objects);
        static void RecordSceneGameObjects(std::vector<SceneObject*>& p_SceneObjects);
        static void EndFrame();

        template<typename UFunction>
        static void Submit(const UFunction& p_Commands){
            
        }


        static void SetBackgroundColor(const std::array<float, 4>& rgba);

        static RendererSettings& GetSettings();

    private:
        static VkCommandBuffer GetCurrentCommandBuffer();
    };
};