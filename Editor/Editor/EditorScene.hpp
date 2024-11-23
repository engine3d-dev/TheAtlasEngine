#pragma once
#include <entt/entt.hpp>
// #include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Core/ApplicationManager/Scene.hpp>
#include <unordered_map>

namespace engine3d{
    class EditorScene{
    public:
        EditorScene();

        void OnCreate();

        void OnUpdate();

        void OnMoveCamUpdate();

        // std::vector<SceneObject*>& GetSceneObjects() { return m_SceneObjects; }

        std::unordered_map<std::string, std::vector<SceneObject*>>& GetAllSceneObjects() { return m_AllSceneObjecs; }

        // SceneObject* GetCameraObject() const { return m_CameraObject; }

    private:
        std::vector<SceneObject*> m_SceneObjects;
        std::vector<SceneObject*> m_PointLightObjects;
        std::vector<SceneObject*> m_CameraObjects;
        std::vector<SceneObject*> m_PointRadioLights;
        

        //! @note All scene objects within the scene.
        std::unordered_map<std::string, std::vector<SceneObject *>> m_AllSceneObjecs;
        Scene* m_Scene;
        // SceneObject* m_CameraObject;
        glm::vec2 m_MousePosition;
        float m_MoveSpeed = {3.f};
        float m_LookSpeed = {1.5f};
    };
};