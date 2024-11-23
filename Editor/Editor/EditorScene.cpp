#include "EditorScene.hpp"
#include <Core/SceneManagment/Components/SPComps/Transform.hpp>
#include <Core/internal/Vulkan2Showcase/VulkanModel.hpp>
#include <Core/EngineLogger.hpp>
#include <Core/SceneManagment/Components/SPComps/EditorCamera.hpp>
#include <Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include <Core/ApplicationInstance.hpp>
#include <Core/Event/InputPoll.hpp>

namespace engine3d{
    Ref<vk::VulkanModel> CreateCubeMesh(glm::vec3 offset){
        std::vector<vk::VulkanModel::Vertex> vertices{
            // left Face (white)
            vk::VulkanModel::Vertex{.Position{-.5f, -.5f, -.5f}, .Color{.9f, .9f, .9f}},
            vk::VulkanModel::Vertex{.Position{-.5f, .5f, .5f}, .Color{.9f, .9f, .9f}},
            vk::VulkanModel::Vertex{.Position ={-.5f, -.5f, .5f}, .Color{.9f, .9f, .9f}},
            vk::VulkanModel::Vertex{.Position ={-.5f, -.5f, -.5f},.Color {.9f, .9f, .9f}},
            vk::VulkanModel::Vertex{.Position ={-.5f, .5f, -.5f}, .Color{.9f, .9f, .9f}},
            vk::VulkanModel::Vertex{.Position ={-.5f, .5f, .5f}, .Color{.9f, .9f, .9f}},
        
            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        
            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        
            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        
            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        
            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        };

        for (auto& v : vertices) {
            v.Position += offset;
        }
        return CreateRef<vk::VulkanModel>(vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }

    EditorScene::EditorScene(){
        m_Scene = new Scene();

        // SyncUpdateManager::GetInstance()->Subscribe(this, &EditorScene::OnUpdate);

        auto cube_mesh = CreateCubeMesh({.0f, .0f, .0f});

        //! @note Make this scene object as part of our current scene.
        // SceneObject* cube = new SceneObject(m_Scene);
        SceneObject* cube = new SceneObject(m_Scene);
        m_CameraObject = new SceneObject(m_Scene);

        m_CameraObject->AddComponent<EditorCamera>();

        // auto cube = SceneObject::Create();
        // cube.SetModel(m_CubeMesh);
        // cube.GetTransform().Translation = {.0f, .0f, .5f};
        // cube.GetTransform().scale = {.5f, .5f, .5f};
        auto& camera_transform = m_CameraObject->SceneGetComponent<Transform>();
        camera_transform.m_Position = {-1.f, -2.f, -20.f};

        auto camera = m_CameraObject->SceneGetComponent<EditorCamera>();

        //! @note Setting our properties
        auto& transform = cube->SceneGetComponent<Transform>();

        // cube->AddComponent<EditorCamera>();

        // auto& camera = cube->SceneGetComponent<EditorCamera>();
        // camera.SetInitialProperties(30.0f, 0.1f, 1000.0f);
        // camera.SetView(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
        auto aspect_ratio = ApplicationInstance::GetWindow().GetAspectRatio();
        // camera.SetOrthoProjection(-1, 1, -1, 1, -1, 1);

        //! @note Shows the actual cube in center of screen that works
        //! @note Because z = -20.f your gonna want to increase the near clip by 100.0f
        // camera.SetViewTarget({-1.f, -2.f, -20.f}, {0.f, 0.f, 2.5f});
        // camera.SetViewTarget(glm::vec3(-1.f, -2.f, -20.f), glm::vec3(0.f, 0.f, 2.5f));

        // negative-float closer, positive-float z further
        // transform.m_Position = {.0f, .0f, .75f};
        // transform.m_Position = {.0f, .0f, 2.5f};
        transform.m_Position = {.0f, .0f, 2.5};
        // transform.m_Position.z = transform.m_Position.z - 2.5f;
        // camera.SetViewXYZ(transform.m_Position, transform.m_AxisRotation);
        // transform.m_Scale = {.5f, .5f, 0.5};
        transform.m_Scale = {.5f, .5f, 0.5};
        // cube->SetModel(cube_mesh);
        cube->SetModal(cube_mesh);

        //! @note Then we add them to our vector.
        m_SceneObjects.push_back(cube);

    }

    void EditorScene::OnCreate(){
    }

    void EditorScene::OnUpdate(){
        // glm::vec3 m_MoveDirection{0.f};
        // glm::vec3 m_Rotation{0};

        // for(const auto& obj : m_SceneObjects){
        //     // auto& transform_compoent = obj->SceneGetComponent<Transform>();
        //     auto& camera_component = obj->SceneGetComponent<EditorCamera>();
        //     camera_component.SetPerspectiveProjection(glm::radians(50.f), ApplicationInstance::GetWindow().GetAspectRatio(), 0.1f, 50.f);
        // }
    }

    void EditorScene::OnMoveCamUpdate(){
        auto& transform = m_CameraObject->SceneGetComponent<Transform>();
        auto& camera = m_CameraObject->SceneGetComponent<EditorCamera>();
        auto cube_transform = m_SceneObjects[0]->SceneGetComponent<Transform>();
        // float tempDt_Y;
        glm::vec2 temp_position = {0.f, 0.f};
        constexpr float sensitivity = 5.0f;
        constexpr glm::vec2 invert_pos = {-1, 1};
        // ConsoleLogInfo("x = {}, y = {}, z = {}", transform.m_Position.x, transform.m_Position.y, transform.m_Position.z);
        // ConsoleLogInfo("x = {}, y = {}, z = {}\n", cube_transform.m_Position.x, cube_transform.m_Position.y, cube_transform.m_Position.z);

        /*
        move-right = D
        move-left = A
        move-forward = W
        move-backward = S

        move-up = E
        move-down = Q
        
        look-left = LEFT
        look-right = RIGHT
        look-up = UP
        look-down = DOWN
        */

        /*
        for(const auto& obj : m_SceneObjects){
            glm::vec3 rotate{0};
            if(InputPoll::IsKeyPressed(ENGINE_KEY_LEFT)) rotate.y += 1.f;
            if(InputPoll::IsKeyPressed(ENGINE_KEY_RIGHT)) rotate.y -= 1.f;
            if(InputPoll::IsKeyPressed(ENGINE_KEY_UP)) rotate.x += 1.f;
            if(InputPoll::IsKeyPressed(ENGINE_KEY_DOWN)) rotate.x -= 1.f;

            auto& transform = obj->SceneGetComponent<Transform>();
            auto& camera = obj->SceneGetComponent<EditorCamera>();

            if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()){
                transform.m_AxisRotation += m_LookSpeed * SyncUpdateManager::GetInstance()->m_SyncLocalDeltaTime * glm::normalize(rotate);
            }

            transform.m_AxisRotation.x = glm::clamp(transform.m_AxisRotation.x, -1.5f, 1.5f);
            transform.m_AxisRotation.y = glm::mod(transform.m_AxisRotation.y, glm::two_pi<float>());

            float yaw = transform.m_AxisRotation.y;
            const glm::vec3 forward_dir{sin(yaw), 0.f, cos(yaw)};
            const glm::vec3 right_dir{forward_dir.z, 0.f, -forward_dir.y};
            const glm::vec3 up_dir{0.f, -1.f, 0.f};

            glm::vec3 move_dir{0.f};

            if(InputPoll::IsKeyPressed(ENGINE_KEY_W)) move_dir += forward_dir; // FORWARD
            if(InputPoll::IsKeyPressed(ENGINE_KEY_S)) move_dir -= forward_dir; // BACKWARD
            if(InputPoll::IsKeyPressed(ENGINE_KEY_D)) move_dir += right_dir; // RIGHT
            if(InputPoll::IsKeyPressed(ENGINE_KEY_A)) move_dir -= right_dir; // LEFT
            if(InputPoll::IsKeyPressed(ENGINE_KEY_E)) move_dir += up_dir;    // UP
            if(InputPoll::IsKeyPressed(ENGINE_KEY_Q)) move_dir -= up_dir;    // DOWN

            if(glm::dot(move_dir, move_dir) > std::numeric_limits<float>::epsilon()){
                transform.m_Position += m_MoveSpeed * (SyncUpdateManager::GetInstance()->m_SyncLocalDeltaTime) * glm::normalize(move_dir);
            }

            // camera.SetViewTarget({-1.f, -2.f, -20.f}, transform.m_Position);
            // camera.SetViewTarget({0.f, 0.f, 0.f}, transform.m_Position);
            camera.SetViewXYZ(transform.m_Position, transform.m_AxisRotation);
        }
        */

        glm::vec3 rotate{0};

        if(InputPoll::IsMousePressed(Mouse::ButtonRight)){
            // temp_position.x = m_MousePosition.x - InputPoll::GetMouseX();
            rotate.y += (m_MousePosition.x - InputPoll::GetMouseX()) * invert_pos.y;
            rotate.x += (m_MousePosition.y - InputPoll::GetMouseY()) * invert_pos.x;
        }
        // if(InputPoll::IsKeyPressed(ENGINE_KEY_LEFT)) rotate.y += 1.f;
        // if(InputPoll::IsKeyPressed(ENGINE_KEY_RIGHT)) rotate.y -= 1.f;
        // if(InputPoll::IsKeyPressed(ENGINE_KEY_UP)) rotate.x += 1.f;
        // if(InputPoll::IsKeyPressed(ENGINE_KEY_DOWN)) rotate.x -= 1.f;

        m_MousePosition = InputPoll::GetMousePosition();
        
        if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()){
            transform.m_AxisRotation += m_LookSpeed * SyncUpdateManager::GetInstance()->m_SyncLocalDeltaTime * glm::normalize(rotate) * sensitivity;
        }

        transform.m_AxisRotation.x = glm::clamp(transform.m_AxisRotation.x, -1.5f, 1.5f);
        transform.m_AxisRotation.y = glm::mod(transform.m_AxisRotation.y, glm::two_pi<float>());

        float yaw = transform.m_AxisRotation.y;
        const glm::vec3 forward_dir{sin(yaw), 0.f, cos(yaw)};
        const glm::vec3 right_dir{forward_dir.z, 0.f, -forward_dir.y};
        const glm::vec3 up_dir{0.f, -1.f, 0.f};

        glm::vec3 move_dir{0.f};

        if(InputPoll::IsKeyPressed(ENGINE_KEY_W)) move_dir += forward_dir; // FORWARD
        if(InputPoll::IsKeyPressed(ENGINE_KEY_S)) move_dir -= forward_dir; // BACKWARD
        if(InputPoll::IsKeyPressed(ENGINE_KEY_D)) move_dir += right_dir; // RIGHT
        if(InputPoll::IsKeyPressed(ENGINE_KEY_A)) move_dir -= right_dir; // LEFT
        if(InputPoll::IsKeyPressed(ENGINE_KEY_E)) move_dir += up_dir;    // UP
        if(InputPoll::IsKeyPressed(ENGINE_KEY_Q)) move_dir -= up_dir;    // DOWN

        if(glm::dot(move_dir, move_dir) > std::numeric_limits<float>::epsilon()){
            transform.m_Position += m_MoveSpeed * (SyncUpdateManager::GetInstance()->m_SyncLocalDeltaTime) * glm::normalize(move_dir);
        }

        camera.SetViewXYZ(transform.m_Position, transform.m_AxisRotation);


        camera.SetPerspectiveProjection(glm::radians(50.f), ApplicationInstance::GetWindow().GetAspectRatio(), 0.1f, 100.f);
        
    }

    // void EditorScene::OnCameraUpdate(){
    //     for(const auto& obj : m_SceneObjects){
    //         obj->SceneGetComponent<EditorCamera>().OnUpdate();
    //     }
    // }
};