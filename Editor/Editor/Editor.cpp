#include "Editor.hpp"
#include "EditorScene.hpp"
#include <Core/internal/Vulkan2Showcase/VulkanModel.hpp>
#include <Core/Scene/SceneTest.hpp>
#include <Core/TimeManagement/GlobalUpdateManager.hpp>
#include <Core/Renderer/Renderer.hpp>

namespace engine3d{

    EditorApplication::EditorApplication(const std::string& p_DebugName) : ApplicationInstance(p_DebugName) {
        GlobalUpdateManager::GetInstance()->SubscribeApplicationUpdate(this, &EditorApplication::OnApplicationUpdate);
        m_EditorScene = new EditorScene();
    }

    EditorApplication::~EditorApplication() {
    }

    void EditorApplication::OnApplicationUpdate(){
        //! @note Handle Events.
        auto& objects = m_EditorScene->GetSceneObjects();
        Renderer::RecordSceneGameObjects(objects);
    }

    ApplicationInstance* InitializeApplication(){
        return new EditorApplication();
    }



};
