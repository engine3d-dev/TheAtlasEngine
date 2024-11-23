#include "Editor.hpp"
#include "EditorScene.hpp"
#include <Core/TimeManagement/GlobalUpdateManager.hpp>
#include <Core/Renderer/Renderer.hpp>

namespace engine3d{

    struct SimplePushConstantData{
        glm::mat2 Transform{1.f};
        glm::vec2 Offsets;
        alignas(16) glm::vec3 Color;
    };

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
