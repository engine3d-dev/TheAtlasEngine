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
        // m_EditorScene->OnUpdate();
        m_EditorScene->OnMoveCamUpdate();
        Renderer::RecordSceneGameObjects(m_EditorScene->GetSceneObjects(), m_EditorScene->GetCameraObject());
    }

    ApplicationInstance* InitializeApplication(){
        return new EditorApplication();
    }



};
