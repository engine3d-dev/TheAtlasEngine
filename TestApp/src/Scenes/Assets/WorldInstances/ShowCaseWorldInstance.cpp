#include "Core/EngineLogger.hpp"
#include <Core/Event/InputPoll.hpp>
#include <Core/Renderer/Renderer.hpp>
#include <Physics/Interfaces/BPLayerInterfaceHandler.hpp>
#include <Core/ApplicationManager/Scene.hpp>
#include <Core/SceneManagment/Components/SPComps/Camera.hpp>
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include <Scenes/Assets/Components/Camera/CameraFollow.hpp>
#include <Scenes/Assets/Components/Physics/PhysicsBody3D.hpp>
#include <Scenes/Assets/Components/testComp.hpp>
#include <Core/ApplicationInstance.hpp>
#include <Scenes/Assets/Components/Bodies/Shapes/BoxShaper.hpp>
#include <Scenes/Assets/Components/Bodies/Shapes/SphereShaper.hpp>
#include <Scenes/Assets/WorldInstances/ShowCaseWorldInstance.hpp>
#include <Scenes/Assets/Components/Graphics/SpriteRender3D.hpp>
#include <Scenes/Assets/Components/Graphics/Meshes/MeshContainer.hpp>
#include <Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/CubeMesh.hpp>
#include <Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/SphereMesh.hpp>
#include <Math/Math.hpp>
#include <Math/Interpolation.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <stdlib.h>
using namespace engine3d;

ShowCaseWorldInstance::ShowCaseWorldInstance()
{
    // Create Object world map for rendering

    engine3d::SyncUpdateManager::GetInstance()->
        Subscribe(this,&ShowCaseWorldInstance::RenderScenes);

    m_Scenes.push_back(new engine3d::Scene());

}

void ShowCaseWorldInstance::CreateObjects()
{
    //Platform
    engine3d::SceneObject* platform = new engine3d::SceneObject(m_Scenes[0]);
    printf("Getting here\n");
    BodyContainer * l_Body = new BoxShaper(JPH::EMotionType::Kinematic, Engine3DLayers::Kenmatic);
    platform->AddComponent<PhysicsBody3D>(l_Body);
    printf("Getting here4\n");
    auto& platformPhysicsBody = platform->GetComponent<PhysicsBody3D>();
    printf("Getting here5\n");
    platformPhysicsBody.ForcedSetScale(100.0f, .50f, 20.0f);
    JPH::Vec3 eulerangles(glm::radians(0.f),glm::radians(0.f),glm::radians(-20.f)); 
    platformPhysicsBody.ForcedSetPosition(-99.0f, -10.0f, 0.0f);
    printf("Getting here6\n");
    platform->name = "Platform1";
    platform->AddComponent<SpriteRender3D>("3d_models/tutorial/cube.obj", platform);
    m_RenderedObjectList.push_back(platform);

    //Platform 2
    engine3d::SceneObject* platform1 = new engine3d::SceneObject(m_Scenes[0]);
    BodyContainer * l_Body1 = new BoxShaper(JPH::EMotionType::Kinematic, Engine3DLayers::Kenmatic);
    platform1->AddComponent<PhysicsBody3D>(l_Body1);
    auto& platformPhysicsBody1 = platform1->GetComponent<PhysicsBody3D>();
    platformPhysicsBody1.ForcedSetScale(10.0f, 2.0f, 2.0f);
    JPH::Vec3 eulerangles1(glm::radians(65.f),glm::radians(90.f),0.f); 
    platformPhysicsBody1.ForcedSetRotation(JPH::Quat::sEulerAngles(eulerangles1));
    platformPhysicsBody1.ForcedSetPosition(0.0f, 0.0f, 0.0f);
    platform1->name = "Platform2";
    platform1->AddComponent<SpriteRender3D>("3d_models/tutorial/cube.obj", platform1);
    m_RenderedObjectList.push_back(platform1);

    //Platform 3
    engine3d::SceneObject* platform3 = new engine3d::SceneObject(m_Scenes[0]);
    BodyContainer * l_Body3 = new BoxShaper(JPH::EMotionType::Kinematic, Engine3DLayers::Kenmatic);
    platform3->AddComponent<PhysicsBody3D>(l_Body3);
    printf("Getting here4\n");
    auto& platformPhysicsBody3 = platform3->GetComponent<PhysicsBody3D>();
    platformPhysicsBody3.ForcedSetScale(100.0f, 2.0f, 1.0f);
    JPH::Vec3 eulerangles2(glm::radians(0.f),glm::radians(0.f),0.f); 
    platformPhysicsBody3.ForcedSetRotation(JPH::Quat::sEulerAngles(eulerangles2));
    platformPhysicsBody3.ForcedSetPosition(-99.0f, -8.0f, 20.0f);
    platform3->name = "Platform3";
    platform3->AddComponent<SpriteRender3D>("3d_models/tutorial/cube.obj", platform3);
    m_RenderedObjectList.push_back(platform3);

    //Platform 4
    engine3d::SceneObject* platform4 = new engine3d::SceneObject(m_Scenes[0]);
    BodyContainer * l_Body4 = new BoxShaper(JPH::EMotionType::Kinematic, Engine3DLayers::Kenmatic);
    platform4->AddComponent<PhysicsBody3D>(l_Body4);
    printf("Getting here4\n");
    auto& platformPhysicsBody4 = platform4->GetComponent<PhysicsBody3D>();
    platformPhysicsBody4.ForcedSetScale(1.0f, 1.0f, 1.0f);
    JPH::Vec3 eulerangles4(glm::radians(0.f),glm::radians(0.f),0.f); 
    platformPhysicsBody4.ForcedSetRotation(JPH::Quat::sEulerAngles(eulerangles4));
    platformPhysicsBody4.ForcedSetPosition(-199.0f, -11.0f, 0.0f);
    platform4->name = "Platform4";
    platform4->AddComponent<SpriteRender3D>("3d_models/tutorial/Archway.obj", platform4);
    m_RenderedObjectList.push_back(platform4);

    //Sphere
    engine3d::SceneObject* player = new engine3d::SceneObject(m_Scenes[0]);
    l_Body = new SphereShaper(JPH::EMotionType::Dynamic, Engine3DLayers::Dynamic);
    player->AddComponent<PhysicsBody3D>(l_Body);
    auto& player_physicsBody = player->GetComponent<PhysicsBody3D>();
    player_physicsBody.ForcedSetScale(.5f, .5f, .5f);
    player_physicsBody.ForcedSetPosition(0.0f, 5.0f, 0.0f);
    player->AddComponent<SpriteRender3D>("3d_models/tutorial/sphere.obj", player);
    player->AddComponent<testComp>();
    player->name = "Player";
    m_RenderedObjectList.push_back(player);

      //Sphere
    engine3d::SceneObject* background = new engine3d::SceneObject(m_Scenes[0]);
    background ->AddComponent<SpriteRender3D>("3d_models/tutorial/sphere.obj", background);
    background ->name = "Player";
    background->GetComponent<Transform>().SetScale<glm::vec3>({100,100,100});
    m_RenderedObjectList.push_back(background);

    //Main Game Camera
    engine3d::SceneObject* MainCamera = new engine3d::SceneObject(m_Scenes[0]);
    auto& camera_transform = MainCamera->GetComponent<engine3d::Transform>();
    
    camera_transform.m_Position = {20.f,45.f,0.f};
    camera_transform.m_AxisRotation = {0.f,0.f,0.f};
    MainCamera->AddComponent<engine3d::Camera>();
    m_CameraObjectList.push_back(MainCamera);

    MainCamera->AddComponent<CameraFollow>(player->GetComponent<Transform>());

    m_AllSceneObjecs.insert({"PointLights", m_PointLightObjectList});
    m_AllSceneObjecs.insert({"RadiusLights", m_PointRadiusLightList});
    m_AllSceneObjecs.insert({"SceneObjects", m_SceneObjectList});
    m_AllSceneObjecs.insert({"RenderedObjects", m_RenderedObjectList});
    m_AllSceneObjecs.insert({"Cameras", m_CameraObjectList});


    //Set camera perspective
    auto& camera =  m_AllSceneObjecs["Cameras"].at(0)->GetComponent<Camera>();
    camera.SetPerspectiveProjection(glm::radians(50.f), engine3d::ApplicationInstance::GetWindow().GetAspectRatio(), 1.f, 1000.f);


}

void ShowCaseWorldInstance::RenderScenes()
{
    if(!callOnce)
    {
        CreateObjects();
        callOnce = true;
    }
    // auto& cameraObj = m_AllSceneObjecs["Cameras"][0];
    // auto& camera = cameraObj->GetComponent<engine3d::Camera>();
    // for(const auto& m_RenderedObjects : m_RenderedObjectList)
    // {
    //     auto& spriteRenderer = m_RenderedObjects->GetComponent<SpriteRender3D>();
    //     spriteRenderer.RenderObject();
    // }
    // auto& camera_transform = cameraObj->GetComponent<engine3d::Transform>();

    auto& cameraObject = m_AllSceneObjecs["Cameras"].at(0);
    auto& transform = cameraObject->GetComponent<Transform>();
    auto& camera = cameraObject->GetComponent<Camera>();
  
    engine3d::Renderer::RecordSceneGameObjects(m_AllSceneObjecs);
}

ShowCaseWorldInstance::~ShowCaseWorldInstance()
{
        delete[] m_Scenes.data();
        delete[] m_SceneObjectList.data();
        delete[] m_PointRadiusLightList.data();
        delete[] m_CameraObjectList.data();
        delete[] m_PointLightObjectList.data();
}