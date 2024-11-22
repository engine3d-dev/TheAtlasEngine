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
        // auto cube = SceneObject::Create();
        // cube.SetModel(m_CubeMesh);
        // cube.GetTransform().Translation = {.0f, .0f, .5f};
        // cube.GetTransform().scale = {.5f, .5f, .5f};

        //! @note Setting our properties
        auto& transform = cube->SceneGetComponent<Transform>();

        cube->AddComponent<EditorCamera>();

        auto& camera = cube->SceneGetComponent<EditorCamera>();
        // camera.SetInitialProperties(30.0f, 0.1f, 1000.0f);
        // camera.SetView(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
        auto aspect_ratio = ApplicationInstance::GetWindow().GetAspectRatio();
        // camera.SetOrthoProjection(-1, 1, -1, 1, -1, 1);

        //! @note Shows the actual cube in center of screen that works
        //! @note Because z = -20.f your gonna want to increase the near clip by 100.0f
        camera.SetViewTarget({-1.f, -2.f, -20.f}, {0.f, 0.f, 2.5f});

        // negative-float closer, positive-float z further
        // transform.m_Position = {.0f, .0f, .75f};
        transform.m_Position = {.0f, .0f, 2.5f};
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

        for(const auto& obj : m_SceneObjects){
            // auto& transform_compoent = obj->SceneGetComponent<Transform>();
            auto& camera_component = obj->SceneGetComponent<EditorCamera>();
            camera_component.SetPerspectiveProjection(glm::radians(50.f), ApplicationInstance::GetWindow().GetAspectRatio(), 0.1f, 100.f);
        }
    }

    void EditorScene::OnMoveCamUpdate(){}

    // void EditorScene::OnCameraUpdate(){
    //     for(const auto& obj : m_SceneObjects){
    //         obj->SceneGetComponent<EditorCamera>().OnUpdate();
    //     }
    // }
};