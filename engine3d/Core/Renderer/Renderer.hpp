#pragma once
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Core/Renderer/RendererSettings.hpp>
#include <array>
#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>

namespace engine3d{
    class Renderer{
    public:
        /**
         * @note A few things to note is when we add a renderer thread.
         * @note I need to ensure that when this renderer thread starts here we can check if its active, delay, and do further status checks.
         * @note For debugging purposes
        */
        static void Initialize(const std::string& p_DebugName);

        static void BeginFrame();

        //! @note Currently the record command buffers just does what "simple_render_system.hpp/.cpp" does already
        //! @note In the future I'll add API's for submitting draw calls.

        //! @note Used for showcase purposes.
        static void RecordSceneGameObjects(std::vector<SceneObject*>& p_SceneObjects, SceneObject* p_CameraObject);
        static void RecordSceneGameObjects(std::unordered_map<std::string, std::vector<SceneObject*>>& p_AllSceneObjects);

        //! @note Editor will utilize this for testing uniform buffers and the newly added descriptor sets.
        static void RecordSceneGameObjectsWithUniformBuffers(std::unordered_map<std::string, std::vector<SceneObject*>>& p_AllSceneObjects);
        static void EndFrame();

        //! @note How we should probably start submitting our commands.
        //! @note This will take in a task and will automatically get flushed onto the renderer thread.
        template<typename UFunction>
        static void Submit(const UFunction& p_Task){
            
        }


        static void SetBackgroundColor(const std::array<float, 4>& rgba);

        static RendererSettings& GetSettings();

    private:
        static VkCommandBuffer GetCurrentCommandBuffer();
    };
};