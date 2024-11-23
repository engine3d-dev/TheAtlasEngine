#include "Core/ApplicationManager/Scene.hpp"

// #include "Core/SceneManagment/SceneObjects/SceneObject.hpp"


namespace engine3d{
    class SceneObject;
};

class ShowCaseSceneInstance
{
    public:
        ShowCaseSceneInstance();
        ~ShowCaseSceneInstance();
        engine3d::Scene* m_Scene;
        std::vector<engine3d::SceneObject*> m_SceneObjects;
        engine3d::Scene* GetScene();

    private:
        void CreateObjects();
};