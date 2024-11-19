#include "Core/ApplicationManager/Scene.hpp"
#include "Core/SceneManagment/SceneObjects/SceneObject.hpp"
#include "Scenes/Assets/Components/Physics/PhysicsBody3D.hpp"
#include "Scenes/Assets/Components/testComp.hpp"
#include <Scenes/Assets/Components/Bodies/Shapes/BoxShaper.hpp>
#include <Scenes/Assets/Components/Bodies/Shapes/SphereShaper.hpp>
#include <Scenes/Assets/SceneInstances/ShowCaseSceneInstance.hpp>

ShowCaseSceneInstance::ShowCaseSceneInstance()
{
    m_Scene = new engine3d::Scene();
    printf("getting here\n");
    CreateObjects();

}

void ShowCaseSceneInstance::CreateObjects()
{
    //Platform
    m_SceneObjects.push_back(new engine3d::SceneObject(m_Scene));
    BodyContainer * l_Body = new BoxShaper();
    m_SceneObjects[0]->AddComponent<PhysicsBody3D>(l_Body);
    m_SceneObjects[0]->name = "Platform1";

    //Sphere
    m_SceneObjects.push_back(new engine3d::SceneObject(m_Scene));
    l_Body = new SphereShaper();
    m_SceneObjects[1]->AddComponent<PhysicsBody3D>(l_Body);
    m_SceneObjects[1]->AddComponent<testComp>();
    m_SceneObjects[1]->name = "Ball";

}

ShowCaseSceneInstance::~ShowCaseSceneInstance()
{
    delete m_Scene;
    for(auto obj : m_SceneObjects)
    {
        delete obj;
    }
}