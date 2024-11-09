#include "Core/ApplicationManager/Scene.hpp"
#include "Core/SceneManagment/SceneObjects/SceneObject.hpp"
#include "Scenes/Assets/Components/testComp.hpp"
#include <TestApp/SceneTest/Scenes/Assets/SceneInstances/ShowCaseSceneInstance.hpp>

ShowCaseSceneInstance::ShowCaseSceneInstance()
{
    m_Scene = new engine3d::Scene();
    printf("getting here\n");
    CreateObjects();

}

ShowCaseSceneInstance::~ShowCaseSceneInstance()
{
    delete m_Scene;
    for(auto obj : m_SceneObjects)
    {
        delete obj;
    }
}

void ShowCaseSceneInstance::CreateObjects()
{
    m_SceneObjects.push_back(new engine3d::SceneObject(m_Scene));
    m_SceneObjects[0]->AddComponent<testComp>();
    m_SceneObjects[0]->name = "Ball";

    
}