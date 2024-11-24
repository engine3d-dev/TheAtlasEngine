#include "Core/Event/InputPoll.hpp"
#include "Core/Renderer/Renderer.hpp"
#include <Core/ApplicationManager/Scene.hpp>
#include <Core/SceneManagment/Components/SPComps/Camera.hpp>
#include <Core/SceneManagment/SceneObjects/SceneObject.hpp>
#include <Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
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
#include <glm/trigonometric.hpp>
#include <stdlib.h>
using namespace engine3d;

ShowCaseWorldInstance::ShowCaseWorldInstance()
{
    // Create Object world map for rendering

    engine3d::SyncUpdateManager::GetInstance()->
        Subscribe(this,&ShowCaseWorldInstance::RenderScenes);

    m_Scenes.push_back(new engine3d::Scene());
    CreateObjects();

}

void ShowCaseWorldInstance::CreateObjects()
{
    //Platform
    engine3d::SceneObject* platform = new engine3d::SceneObject(m_Scenes[0]);
    BodyContainer * l_Body = new BoxShaper();
    platform->AddComponent<PhysicsBody3D>(l_Body);
    auto& platformPhysicsBody = platform->GetComponent<PhysicsBody3D>();
    platformPhysicsBody.SetScale(10.0f, .5f, 10.0f); 
    platform->name = "Platform1";
    platform->AddComponent<SpriteRender3D>("3d_models/tutorial/cube.obj", platform);
    m_RenderedObjectList.push_back(platform);

    //Platform
    engine3d::SceneObject* platform1 = new engine3d::SceneObject(m_Scenes[0]);
    BodyContainer * l_Body1 = new BoxShaper();
    platform1->AddComponent<PhysicsBody3D>(l_Body1);
    auto& platformPhysicsBody1 = platform1->GetComponent<PhysicsBody3D>();
    platformPhysicsBody1.SetPosition(0.0f, 5.f, 0.0f);
    platformPhysicsBody1.SetScale(10.0f, 2.0f, 2.0f);
    JPH::Vec3 eulerangles(glm::radians(45.f),0.f,0.f); 
    platformPhysicsBody1.SetRotation(Quat::sEulerAngles(eulerangles));
    platform1->name = "Platform1";
    platform1->AddComponent<SpriteRender3D>("3d_models/tutorial/cube.obj", platform1);
    m_RenderedObjectList.push_back(platform1);

    //Sphere
    engine3d::SceneObject* player = new engine3d::SceneObject(m_Scenes[0]);
    l_Body = new SphereShaper();
    player->AddComponent<PhysicsBody3D>(l_Body);
    auto& player_physicsBody = player->GetComponent<PhysicsBody3D>();
    player_physicsBody.SetScale(.5f, .5f, .5f);
    player_physicsBody.SetPosition(0.0f, 10.0f, 0.0f);
    player->AddComponent<SpriteRender3D>("3d_models/tutorial/sphere.obj", player);
    player->AddComponent<testComp>();
    player->name = "Player";
    m_RenderedObjectList.push_back(player);


    //Main Game Camera
    engine3d::SceneObject* MainCamera = new engine3d::SceneObject(m_Scenes[0]);
    auto& camera_transform = MainCamera->GetComponent<engine3d::Transform>();
    
    camera_transform.m_Position = {-.2f,-.2f,20.f};
    camera_transform.m_AxisRotation = {0.f,180.f,0.f};
    MainCamera->AddComponent<engine3d::Camera>();
    m_CameraObjectList.push_back(MainCamera);

    m_AllSceneObjecs.insert({"PointLights", m_PointLightObjectList});
    m_AllSceneObjecs.insert({"RadiusLights", m_PointRadiusLightList});
    m_AllSceneObjecs.insert({"SceneObjects", m_SceneObjectList});
    m_AllSceneObjecs.insert({"RenderedObjects", m_RenderedObjectList});
    m_AllSceneObjecs.insert({"Cameras", m_CameraObjectList});
}

void ShowCaseWorldInstance::RenderScenes()
{
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
    // float tempDt_Y;
    glm::vec2 temp_position = {0.f, 0.f};
    constexpr float sensitivity = 2.0f;
    float pos_sensitivity = 2.f;
    constexpr glm::vec2 invert_pos = {-1, -1};
    glm::vec3 rotate{0};

    //! @note Make sure that our mouse controls how camera rotates.
    if(InputPoll::IsMousePressed(Mouse::ButtonRight)){
        // temp_position.x = m_MousePosition.x - InputPoll::GetMouseX();
        rotate.y += (m_MousePosition.x - InputPoll::GetMouseX()) * invert_pos.y;
        rotate.x += (m_MousePosition.y - InputPoll::GetMouseY()) * invert_pos.x;
    }

    m_MousePosition = InputPoll::GetMousePosition();
    
    //! @note Utilize linear interpolation to get smooth camera rotation. 
    if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()){
        float dt = SyncUpdateManager::GetInstance()->m_SyncLocalDeltaTime;
        auto temp_rotation = (m_LookSpeed * glm::normalize(rotate) * sensitivity) + transform.m_AxisRotation;
        transform.m_AxisRotation = engine3d::Interpolation::LinearInterpolate(transform.m_AxisRotation, temp_rotation, nullptr, dt);
    }

    transform.m_AxisRotation.x = glm::clamp(transform.m_AxisRotation.x, -1.5f, 1.5f);
    transform.m_AxisRotation.y = glm::mod(transform.m_AxisRotation.y, glm::two_pi<float>());

    float yaw = transform.m_AxisRotation.y;
    const glm::vec3 forward_dir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 right_dir{forward_dir.z, 0.f, -forward_dir.x};
    const glm::vec3 up_dir{0.f, -1.f, 0.f};

    glm::vec3 move_dir{0.f};

    if(InputPoll::IsKeyPressed(ENGINE_KEY_W)) move_dir += forward_dir; // FORWARD
    if(InputPoll::IsKeyPressed(ENGINE_KEY_S)) move_dir -= forward_dir; // BACKWARD
    if(InputPoll::IsKeyPressed(ENGINE_KEY_D)) move_dir += right_dir; // RIGHT
    if(InputPoll::IsKeyPressed(ENGINE_KEY_A)) move_dir -= right_dir; // LEFT
    if(InputPoll::IsKeyPressed(ENGINE_KEY_SPACE)) move_dir -= up_dir;    // UP
    if(InputPoll::IsKeyPressed(ENGINE_KEY_LEFT_SHIFT)) move_dir += up_dir;    // DOWN

    if(InputPoll::IsMousePressed(ENGINE_MOUSE_BUTTON_MIDDLE)){
        pos_sensitivity += (m_MousePosition.y - InputPoll::GetMouseY()) * invert_pos.x;
    }

    if(glm::dot(move_dir, move_dir) > std::numeric_limits<float>::epsilon()){
        transform.m_Position += m_MoveSpeed * (SyncUpdateManager::GetInstance()->m_SyncLocalDeltaTime) * glm::normalize(move_dir) * pos_sensitivity;
    }

    //auto& sphere_transform = m_AllSceneObjecs["RenderedObjects"][0]->GetComponent<engine3d::Transform>();
    camera.SetViewXYZ(transform.m_Position, transform.m_AxisRotation);
    camera.SetPerspectiveProjection(glm::radians(50.f), engine3d::ApplicationInstance::GetWindow().GetAspectRatio(), 1.f, 1000.f);
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