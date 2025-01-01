#include "EditorWorld.hpp"
#include <core/event/input_poll.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <glm/fwd.hpp>
#include <renderer/renderer.hpp>
#include <core/event/mouse_codes.hpp>
#include <core/engine_logger.hpp>
#include <scene/components/components.hpp>
#include <core/math/math.hpp>

// static glm::vec2 s_MousePosition;
// static float s_MoveSpeed = {3.f};
// static float s_LookSpeed = {1.5f};

struct Velocity{
    glm::vec2 Position;
};

EditorWorld::EditorWorld(){
    // s_MousePosition = {0.0f, 0.0f};
}

// EditorWorld::EditorWorld(const std::string& p_Tag) : m_Scene1("Scene 1") {
EditorWorld::EditorWorld(const std::string& p_Tag){
    // ConsoleLogFatal("EditorWorld(const std::string&) gets called!");
    engine3d::ConsoleEngineLogger::AddNewLogger(p_Tag);
    m_Scene1 = engine3d::Scene("New Scene");
    auto entity = m_Scene1.CreateEntity("Bob");
    entity.AddComponent<engine3d::Transform>();

    if(entity.HasComponent<engine3d::Transform>()){
        ConsoleLogInfo("Entity Has the engine3d::Transform component!");
    }
    // engine3d::Transform old_transform = {};
    // Velocity vel = {};
    // entity.SetComponent<engine3d::Transform, Velocity>(old_transform, vel);

    // new_node1 = engine3d::SceneNode(m_Scene1);

    // m_SceneEntities["Camera"] = engine3d::SceneNode(m_Scene1);
    // engine3d::Ref<engine3d::SceneNode> new_camera_node = engine3d::CreateRef<engine3d::SceneNode>(m_Scene1, "camera");
    // new_camera_node->AddComponent<engine3d::Camera>();
    // // new_camera_node->GetComponent<engine3d::Transform>().Position = {-2.f, 5.f, -20.f};
    // auto& camera_transform = new_camera_node->GetComponent<engine3d::Transform>();
    // camera_transform.Position = {-2.f, 5.f, -20.f};
    
    // //! @note In the UI Editor, MeshComponent should be defaulted to a few primitives (I think)
    // engine3d::Ref<engine3d::SceneNode> new_player_node = engine3d::CreateRef<engine3d::SceneNode>(m_Scene1, "player");
    // new_player_node->AddComponent<engine3d::MeshComponent>("3d_models/tutorial/sphere.obj");

    // m_SceneEntities[new_camera_node->GetTag()] = new_camera_node;
    // m_SceneEntities[new_player_node->GetTag()] = new_player_node;

    // auto& player = m_SceneEntities["player"];

    // if(!player->HasComponent<engine3d::MeshComponent>()){
    //     ConsoleLogInfo("Does NOT have the MeshComponent!");
    //     return;
    // }

    // auto& player_transform = player->GetComponent<engine3d::Transform>();
    // player_transform.Position = {-1.f, 5.f, -20.f};
    // player_transform.Scale = {1.f, 1.f, 1.f};

    // // auto& camera_transform = camera->GetComponent<engine3d::Transform>();
    // // new_node1 = engine3d::SceneNode(m_Scene1);
    // // m_ChildrenSceneNodes[0] = engine3d::SceneNode(m_Scene1);
    // // m_ChildrenSceneNodes[0].AddComponent<engine3d::MeshComponent>("3d_models/tutorial/sphere.obj");
    // s_MousePosition = {0.f, 0.f};
}

void EditorWorld::OnStart(){
    // new_node1->AddComponent<engine3d::MeshComponent>("3d_models/tutorial/sphere.obj");
}

void EditorWorld::OnStop(){
}

//! TODO: Implement either a scene graph, or decide the data structure to go with in how we want to handle scenes
void EditorWorld::OnUpdate(){
    // auto& camera_object = m_SceneEntities["camera"];
    // auto& transform = camera_object->GetComponent<engine3d::Transform>();
    // auto& camera_component = camera_object->GetComponent<engine3d::Camera>();


    // // float tempDt_Y;
    // // glm::vec2 temp_position = {0.f, 0.f};
    // constexpr float sensitivity = 2.0f;
    // constexpr float pos_sensitivity = 2.f;
    // constexpr glm::vec2 invert_pos = {1, -1};
    // glm::vec3 rotate{0};

    // if(engine3d::InputPoll::IsMousePressed(engine3d::Mouse::ButtonRight)){
    //     rotate.y += (s_MousePosition.x - engine3d::InputPoll::GetMouseX()) * invert_pos.y;
    //     rotate.x += (s_MousePosition.y - engine3d::InputPoll::GetMouseY()) * invert_pos.x;
    // }

    // s_MousePosition = {engine3d::InputPoll::GetMouseX(), engine3d::InputPoll::GetMouseY()};

    // //! @note Utilize linear interpolation to get smooth camera rotation. 
    // if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()){
    //     float dt = engine3d::SyncUpdate::DeltaTime();
    //     auto temp_rotation = (s_LookSpeed * glm::normalize(rotate) * sensitivity) + transform.Rotation;
    //     transform.Rotation = engine3d::Interpolation::LinearInterpolate(transform.Rotation, temp_rotation, nullptr, dt);
    // }
    //  transform.Rotation.x = glm::clamp(transform.Rotation.x, -1.5f, 1.5f);
    // transform.Rotation.y = glm::mod(transform.Rotation.y, glm::two_pi<float>());

    // float yaw = transform.Rotation.y;
    // const glm::vec3 forward_dir{sin(yaw), 0.f, cos(yaw)};
    // const glm::vec3 right_dir{forward_dir.z, 0.f, -forward_dir.x};
    // const glm::vec3 up_dir{0.f, -1.f, 0.f};

    // glm::vec3 move_dir{0.f};

    // if(engine3d::InputPoll::IsKeyPressed(ENGINE_KEY_W)) move_dir += forward_dir; // FORWARD
    // if(engine3d::InputPoll::IsKeyPressed(ENGINE_KEY_S)) move_dir -= forward_dir; // BACKWARD
    // if(engine3d::InputPoll::IsKeyPressed(ENGINE_KEY_D)) move_dir += right_dir; // RIGHT
    // if(engine3d::InputPoll::IsKeyPressed(ENGINE_KEY_A)) move_dir -= right_dir; // LEFT
    // if(engine3d::InputPoll::IsKeyPressed(ENGINE_KEY_E)) move_dir += up_dir;    // UP
    // if(engine3d::InputPoll::IsKeyPressed(ENGINE_KEY_Q)) move_dir -= up_dir;    // DOWN

    // if(glm::dot(move_dir, move_dir) > std::numeric_limits<float>::epsilon()){
    //     transform.Position += s_MoveSpeed * (engine3d::SyncUpdate::DeltaTime()) * glm::normalize(move_dir) * pos_sensitivity;
    // }

    // camera_component.SetViewXYZ(transform.Position, transform.Rotation);

    // camera_component.SetPerspectiveProjection(glm::radians(50.f), engine3d::ApplicationInstance::GetWindow().GetAspectRatio(), 0.1f, 100.f);

    // engine3d::Renderer::RenderSceneObjects(m_SceneEntities);
}