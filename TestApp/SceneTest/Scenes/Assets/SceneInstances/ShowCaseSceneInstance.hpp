#include "Core/ApplicationManager/Scene.hpp"
#include "Core/SceneManagment/SceneObjects/SceneObject.hpp"
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