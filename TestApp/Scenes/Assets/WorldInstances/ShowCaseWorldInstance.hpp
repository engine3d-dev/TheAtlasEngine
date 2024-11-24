#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Core/ApplicationManager/Scene.hpp>


#include <vector>

// #include "Core/SceneManagment/SceneObjects/SceneObject.hpp"
class ShowCaseWorldInstance
{
    public:
        ShowCaseWorldInstance();
        ~ShowCaseWorldInstance();
        std::vector<engine3d::Scene*> m_Scenes;
        engine3d::Scene* GetScene();
        void RenderScenes();


    private:
        void CreateObjects();
        std::unordered_map<std::string, std::vector<engine3d::SceneObject *>> m_AllSceneObjecs;
        std::vector<engine3d::SceneObject*> m_SceneObjectList;
        std::vector<engine3d::SceneObject*> m_RenderedObjectList;
        std::vector<engine3d::SceneObject*> m_PointLightObjectList;
        std::vector<engine3d::SceneObject*> m_CameraObjectList;
        std::vector<engine3d::SceneObject*> m_PointRadiusLightList;

        glm::vec2 m_MousePosition;
        float m_MoveSpeed = {5.f};
        float m_LookSpeed = {1.5f};
};