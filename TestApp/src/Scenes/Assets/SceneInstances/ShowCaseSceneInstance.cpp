#include "Core/ApplicationManager/Scene.hpp"
#include "Core/SceneManagment/SceneObjects/SceneObject.hpp"
#include "Scenes/Assets/Components/Physics/PhysicsBody3D.hpp"
#include "Scenes/Assets/Components/testComp.hpp"
#include <Scenes/Assets/Components/Bodies/Shapes/BoxShaper.hpp>
#include <Scenes/Assets/Components/Bodies/Shapes/SphereShaper.hpp>
#include <Scenes/Assets/SceneInstances/ShowCaseSceneInstance.hpp>
#include <Scenes/Assets/Components/Graphics/Meshes/MeshContainer.hpp>
#include <Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/CubeMesh.hpp>
#include <Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/SphereMesh.hpp>

ShowCaseSceneInstance::ShowCaseSceneInstance()
{
    m_Scene = new engine3d::Scene();
    printf("getting here/n");
    CreateObjects();

}

void ShowCaseSceneInstance::CreateObjects()
{
    //Platform
    m_SceneObjects.push_back(new engine3d::SceneObject(m_Scene));
    BodyContainer * l_Body = new BoxShaper();
    m_SceneObjects[0]->AddComponent<PhysicsBody3D>(l_Body);

    //MeshContainer * l_Mesh = new CubeMesh();
    //m_SceneObjects[0]->AddComponent<SpriteRender3D>(l_Mesh);
    m_SceneObjects[0]->name = "Platform1";

    //Sphere
    m_SceneObjects.push_back(new engine3d::SceneObject(m_Scene));
    l_Body = new SphereShaper();
    m_SceneObjects[1]->AddComponent<PhysicsBody3D>(l_Body);
    // why is l_Mesh assigned the address of SphereMesh()
    // the first index of m_SceneObjects is assigned the value of 
    // SpriteRender3D instance?? But what does l_mesh do in this line?
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