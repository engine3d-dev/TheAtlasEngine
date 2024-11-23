#include "EditorScene.hpp"
#include "Core/GraphicDrivers/VertexBuffer.hpp"
#include <Core/SceneManagment/Components/SPComps/Transform.hpp>
#include <Core/EngineLogger.hpp>
#include <Core/SceneManagment/Components/SPComps/EditorCamera.hpp>
#include <Core/TimeManagement/UpdateManagers/SyncUpdateManager.hpp>
#include <Math/Interpolation.hpp>
#include <Core/ApplicationInstance.hpp>
#include <Core/Event/InputPoll.hpp>

namespace engine3d{
    static Mesh CreateCubeMesh(glm::vec3 offset){
        std::vector<Vertex> vertices = { // left face (white)
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
 
      // right face (yellow)
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
 
      // top face (orange, remember y axis points down)
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
 
      // bottom face (red)
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
 
      // nose face (blue)
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
 
      // tail face (green)
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    };
    for (auto& v : vertices) {
        v.Position += offset;
    }

        std::vector<uint32_t> indices = {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                          12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};
        
        Ref<VertexBuffer> vb = VertexBuffer::Create(vertices);
        Ref<IndexBuffer> ib = IndexBuffer::Create(indices);
        // return VertexBuffer::Create(vertices);
        return Mesh(vb, ib);
    }

    EditorScene::EditorScene(){
        m_Scene = new Scene();

        // SyncUpdateManager::GetInstance()->Subscribe(this, &EditorScene::OnUpdate);

        // auto cube_mesh = CreateCubeMesh({.0f, .0f, .0f});

        //! @note Instead of loading in primitive cubes by hand, we load in .obj's instead.
        // auto cube_mesh = Mesh::LoadModel("3d_models/tutorial/smooth_vase.obj");
        // auto cube_mesh = Mesh::LoadModel("3d_models/tutorial/colored_cube.obj");
        auto cube_mesh = Mesh::LoadModel("3d_models/tutorial/sphere.obj");

        //! @note Make this scene object as part of our current scene.

        // -----------------------------
        // Camera Scene Object Creation
        // -----------------------------
        m_CameraObject = new SceneObject(m_Scene);
        m_CameraObject->AddComponent<EditorCamera>();
        auto& camera_transform = m_CameraObject->SceneGetComponent<Transform>();
        camera_transform.m_Position = {-1.f, -2.f, -20.f};
        auto camera = m_CameraObject->SceneGetComponent<EditorCamera>();

        // -----------------------------
        // Cube 1 Scene object Creation
        // -----------------------------
        SceneObject* cube1 = new SceneObject(m_Scene);
        auto& cube1_transform = cube1->SceneGetComponent<Transform>();
        cube1_transform.m_Position = {.0f, .0f, 2.5};
        cube1_transform.m_Scale = {.5f, .5f, 0.5};
        cube1->SetMesh(cube_mesh);

        // -----------------------------
        // Cube 2 Scene object Creation
        // -----------------------------
        SceneObject* cube2 = new SceneObject(m_Scene);
        auto& cube2_transform = cube2->SceneGetComponent<Transform>();
        // auto aspect_ratio = ApplicationInstance::GetWindow().GetAspectRatio();
        cube2_transform.m_Position = {5.f, .0f, -7.f};
        cube2_transform.m_Scale = {.5f, .5f, 0.5};
        cube2->SetMesh(cube_mesh);



        //! @note Then we add them to our vector.
        m_SceneObjects.push_back(cube1);
        m_SceneObjects.push_back(cube2);

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
        constexpr float sensitivity = 2.0f;
        constexpr float pos_sensitivity = 2.f;
        constexpr glm::vec2 invert_pos = {1, -1};
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
            transform.m_AxisRotation = Interpolation::LinearInterpolate(transform.m_AxisRotation, temp_rotation, nullptr, dt);
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
            transform.m_Position += m_MoveSpeed * (SyncUpdateManager::GetInstance()->m_SyncLocalDeltaTime) * glm::normalize(move_dir) * pos_sensitivity;
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