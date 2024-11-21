#include "Editor.hpp"
#include "EditorScene.hpp"
#include <Core/internal/Vulkan2Showcase/VulkanModel.hpp>
#include <Core/Scene/SceneTest.hpp>
#include <Core/TimeManagement/GlobalUpdateManager.hpp>
#include <Core/Renderer/Renderer.hpp>

namespace engine3d{

     
     /*
     Ref<vk::VulkanModel> CreateCubeMesh(glm::vec3 offset){
        std::vector<vk::VulkanModel::Vertex> vertices{
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        
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
    */


    EditorApplication::EditorApplication(const std::string& p_DebugName) : ApplicationInstance(p_DebugName) {
        GlobalUpdateManager::GetInstance()->SubscribeApplicationUpdate(this, &EditorApplication::OnApplicationUpdate);
        // Ref<vk::VulkanModel> m_CubeMesh = CreateCubeMesh({.0f, .0f, .0f});


        // auto cube = SceneObjectTutorial::Create();
        // cube.SetModel(m_CubeMesh);
        // cube.GetTransform().Translation = {.0f, .0f, .5f};
        // cube.GetTransform().scale = {.5f, .5f, .5f};

        // m_SceneGameObjects.push_back(cube);

        // ConsoleLogError("m_SceneGameObjects.size() === {}", m_SceneGameObjects.size());
        m_EditorScene = new EditorScene();


        // m_GravitySystem = GravityPhysicsSystem(0.81f);
        // m_SquareMesh = CreateSquareMesh({0.5f, 0.5f});
        
        // //! @note Create Physics Circle Mesh
        // m_CircleMesh = CreateCircleMesh(64);

        // // Creating a red circle.
        // auto red = SceneObject::Create();
        // red.GetTransform().scale = glm::vec2{.05f};
        // red.GetTransform().Translation = {.5f, .5f};
        // red.SetColor({1.f, 0.f, 0.f});
        // red.RigidBodyComponent.velocity = {-.5f, .0f};
        // red.SetModel(m_CircleMesh);

        // m_SceneGameObjects.push_back(red);

        // //! @note Creating a blue circle.
        // auto blue = SceneObject::Create();
        // blue.GetTransform().scale = glm::vec2{.05f};
        // blue.GetTransform().Translation = {-.45f, -.25f};
        // blue.SetColor({0.f, 0.f, 1.f});
        // blue.RigidBodyComponent.velocity = {-.5f, .0f};
        // blue.SetModel(m_CircleMesh);

        // m_SceneGameObjects.push_back(blue);


        // // create vector field
        // // std::vector<SceneObject> vectorField{};
        // int gridCount = 40;
        // for (int i = 0; i < gridCount; i++) {
        //     for (int j = 0; j < gridCount; j++) {
        //         auto vf = SceneObject::Create();
        //         vf.GetTransform().scale = glm::vec2(0.005f);
        //         vf.GetTransform().Translation = {
        //             -1.0f + (i + 0.5f) * 2.0f / gridCount,
        //             -1.0f + (j + 0.5f) * 2.0f / gridCount};
        //         vf.SetColor(glm::vec3(1.0f));
        //         // vf.model = m_SquareMesh;
        //         vf.SetModel(m_SquareMesh);
        //         m_VectorFields.push_back(vf);
        //     }
        // }
    }

    EditorApplication::~EditorApplication() {
    }

    void EditorApplication::OnApplicationUpdate(){
        //! @note Handle Events.
        // m_GravitySystem.update(m_SceneGameObjects, 5);
        // m_VectorFieldSystem.update(m_GravitySystem, m_SceneGameObjects, m_VectorFields);
        auto& objects = m_EditorScene->GetSceneObjects();
        Renderer::RecordSceneGameObjects(objects);
        // Renderer::RecordGameObjects(m_SceneGameObjects);
        // Renderer::RecordGameObjects(m_VectorFields);


        // if(InputPoll::IsKeyPressed(ENGINE_KEY_ESCAPE)){
        //     exit(0);
        // }
    }

    ApplicationInstance* InitializeApplication(){
        return new EditorApplication();
    }



};
