#pragma once
#include <entt/entt.hpp>
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Core/ApplicationManager/Scene.hpp>

namespace engine3d{
    class EditorScene{
    public:
        EditorScene();

        void OnCreate();

        std::vector<SceneObject*>& GetSceneObjects() { return m_SceneObjects; }

    private:
        std::vector<SceneObject*> m_SceneObjects;
        Scene* m_Scene;
    };
};