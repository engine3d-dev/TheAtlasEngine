#pragma once
#include <entt/entt.hpp>
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Core/ApplicationManager/Scene.hpp>

namespace engine3d{
    class EditorScene{
    public:
        EditorScene();

        void OnCreate();

        void OnUpdate();

        void OnMoveCamUpdate();

        std::vector<SceneObject*>& GetSceneObjects() { return m_SceneObjects; }

        SceneObject* GetCameraObject() const { return m_CameraObject; }

    private:
        std::vector<SceneObject*> m_SceneObjects;
        Scene* m_Scene;
        SceneObject* m_CameraObject;
        glm::vec2 m_MousePosition;
        float m_MoveSpeed = {3.f};
        float m_LookSpeed = {1.5f};
    };
};