#include "Editor.hpp"
#include <Core/Core.hpp>
#include "Core/Renderer/Renderer.hpp"
#include "Core/Scene/SceneObject.hpp"
// #include "internal/VulkanCpp/helper_functions.hpp"
#include <Core/internal/Vulkan2Showcase/helper_functions.hpp>
#include <Core/internal/Vulkan2Showcase/VulkanContext.hpp>
#include <Core/Event/InputPoll.hpp>
#include <Core/Event/KeyCodes.hpp>
#include <Core/EngineLogger.hpp>
#include <Core/Timestep.hpp>
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/constants.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d{

    Ref<vk::VulkanModel> CreateSquareMesh(glm::vec2 offset) {
        std::vector<vk::VulkanModel::Vertex> vertices = {
            {{-0.5f, -0.5f}},
            {{0.5f, 0.5f}},
            {{-0.5f, 0.5f}},
            {{-0.5f, -0.5f}},
            {{0.5f, -0.5f}},
            {{0.5f, 0.5f}},  //
        };
        for (auto& v : vertices) {
            v.Position += offset;
        }
        return CreateRef<vk::VulkanModel>(vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }
    
    Ref<vk::VulkanModel> CreateCircleMesh(unsigned int numSides) {
        std::vector<vk::VulkanModel::Vertex> uniqueVertices{};
        for (int i = 0; i < numSides; i++) {
            float angle = i * glm::two_pi<float>() / numSides;
            uniqueVertices.push_back({{glm::cos(angle), glm::sin(angle)}});
        }
        uniqueVertices.push_back({});  // adds center vertex at 0, 0
        
        std::vector<vk::VulkanModel::Vertex> vertices{};
        for (int i = 0; i < numSides; i++) {
            vertices.push_back(uniqueVertices[i]);
            vertices.push_back(uniqueVertices[(i + 1) % numSides]);
            vertices.push_back(uniqueVertices[numSides]);
        }
        return CreateRef<vk::VulkanModel>(vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }


    EditorApplication::EditorApplication(const std::string& p_DebugName) : ApplicationInstance(p_DebugName) {
        GlobalUpdateManager::GetInstance()->SubscribeApplicationUpdate(this, &EditorApplication::OnApplicationUpdate);

        m_GravitySystem = GravityPhysicsSystem(0.81f);
        m_SquareMesh = CreateSquareMesh({0.5f, 0.5f});
        
        //! @note Create Physics Circle Mesh
        m_CircleMesh = CreateCircleMesh(64);

        // Creating a red circle.
        auto red = SceneObject::Create();
        red.GetTransform().scale = glm::vec2{.05f};
        red.GetTransform().Translation = {.5f, .5f};
        red.SetColor({1.f, 0.f, 0.f});
        red.RigidBodyComponent.velocity = {-.5f, .0f};
        red.SetModel(m_CircleMesh);

        m_SceneGameObjects.push_back(red);

        //! @note Creating a blue circle.
        auto blue = SceneObject::Create();
        blue.GetTransform().scale = glm::vec2{.05f};
        blue.GetTransform().Translation = {-.45f, -.25f};
        blue.SetColor({0.f, 0.f, 1.f});
        blue.RigidBodyComponent.velocity = {-.5f, .0f};
        blue.SetModel(m_CircleMesh);

        m_SceneGameObjects.push_back(blue);


        // create vector field
        // std::vector<SceneObject> vectorField{};
        int gridCount = 40;
        for (int i = 0; i < gridCount; i++) {
            for (int j = 0; j < gridCount; j++) {
                auto vf = SceneObject::Create();
                vf.GetTransform().scale = glm::vec2(0.005f);
                vf.GetTransform().Translation = {
                    -1.0f + (i + 0.5f) * 2.0f / gridCount,
                    -1.0f + (j + 0.5f) * 2.0f / gridCount};
                vf.SetColor(glm::vec3(1.0f));
                // vf.model = m_SquareMesh;
                vf.SetModel(m_SquareMesh);
                m_VectorFields.push_back(vf);
            }
        }
    }

    EditorApplication::~EditorApplication() {
    }

    void EditorApplication::OnApplicationUpdate(){
        //! @note Handle Events.
        m_GravitySystem.update(m_SceneGameObjects, 5);
        m_VectorFieldSystem.update(m_GravitySystem, m_SceneGameObjects, m_VectorFields);
        Renderer::RecordGameObjects(m_SceneGameObjects);
        Renderer::RecordGameObjects(m_VectorFields);


        // if(InputPoll::IsKeyPressed(ENGINE_KEY_ESCAPE)){
        //     exit(0);
        // }
    }

    ApplicationInstance* InitializeApplication(){
        return new EditorApplication();
    }



};
