#include "level_scene.hpp"
#include <drivers/vulkan/helper_functions.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <drivers/vulkan/vulkan_swapchain.hpp>

#include <core/event/event.hpp>
#include <core/update_handlers/sync_update.hpp>

#include <core/serialize/serializer.hpp>
#include <core/ui/widgets.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <physics/jolt-cpp/helper_functions.hpp>
#include <physics/jolt-cpp/jolt_api.hpp>
// #include <physics/physics_engine.hpp>
#include <renderer/renderer.hpp>

#include <core/timer.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

struct MeshData {
    glm::vec3 Position{ 0.f };
    glm::vec3 Scale{ 0.f };
    glm::vec3 Rotation{ 0.f };
    std::string mesh_file = "";
    glm::vec3 Color{ 1.f };
};

struct CameraData {
    glm::vec3 Position{ 0.f };
    glm::vec3 Front{ 0.f };
    float Angle = 90.f;
};

static float sensitivity = 0.f;

static MeshData sphere_data;
static MeshData some_mesh_data;
static CameraData camera_data;
static std::string s_SceneFilepath = "";
static glm::vec3 g_light_position = glm::vec3(0.0f, 0.0f, 1.0f);

namespace ui {
    [[maybe_unused]] static bool BeginPopupContextWindow(const char* str_id,
                                                         ImGuiMouseButton mb,
                                                         bool over_items) {
        return ImGui::BeginPopupContextWindow(
          str_id, mb | (over_items ? 0 : ImGuiPopupFlags_NoOpenOverItems));
    }

    // [[maybe_unused]] static void draw_entity_transform(
    //   const atlas::Transform& p_transform) {
    //     atlas::Transform read_transform = p_transform;
    //     read_transform.Position = atlas::ui::draw_vec3("pos 1");
    //     read_transform.Scale = atlas::ui::draw_vec3("scale 1");
    //     read_transform.Rotation = atlas::ui::draw_vec3("rotate 1");
    //     // atlas::ui::draw_vec3("color 1", read_transform.Color);
    // }
}; // namespace ui

level_scene::level_scene() {}

level_scene::level_scene(const std::string& p_tag)
  : atlas::scene_scope(p_tag) {

    console_log_info("scene_scope::scene_scope with Tag = {} called!", p_tag);
    atlas::world_scope world = atlas::system_registry::get_world();
    std::string world_tag = world.get_tag();
    console_log_trace("From level_scene::level_scene() ==> World Tag = {}",
                      world_tag);

    //! @note Creating our objects from our scene
    m_sphere = this->create_new_object("sphere");
    // m_sphere->set<atlas::MeshComponent>({ "assets/models/colored_cube.obj"
    // });
    m_sphere->set<atlas::RenderTarget3D>(
      atlas::RenderTarget3D("assets/models/colored_cube.obj"));

    m_sphere->set<atlas::Transform>({ .Position = { 0.f, 2.10f, -7.30f },
                                      .Scale = { .20f, .20f, .20f },
                                      .Color = { 1.0f, 1.f, 1.f, 1.f } });

    m_sphere->add<atlas::RigidBody3D>();
    m_sphere->add<atlas::Light>();
    // First we get body interface from this scene's physics system
    // BodyInterface is how  jolt's bodies interact with the physics system'
    // main way to interact with the physics systems through body interfaces

    const atlas::Transform* transform = m_sphere->get<atlas::Transform>();

    sphere_data.Position = transform->Position;
    sphere_data.Scale = transform->Scale;
    sphere_data.Rotation = transform->Rotation;

    m_platform = this->create_new_object("mesh1");
    m_platform->set<atlas::Transform>({
      .Position = { 0.f, 1.40f, -7.4f },
      .Scale = { 2.80f, -0.08f, 3.50f },
    });

    auto some_mesh_transform = m_platform->get<atlas::Transform>();
    some_mesh_data.Position = some_mesh_transform->Position;
    some_mesh_data.Scale = some_mesh_transform->Scale;
    some_mesh_data.Rotation = some_mesh_transform->Rotation;
    m_platform->set<atlas::RenderTarget3D>({ "assets/models/cube.obj" });

    /**
    @note Jolt's Body Interface
    @note Used for initiating simulated objects within the physics simulation
    @note CreateBody: Creates new physics body
    @note AddBody: Adds newly (or created) physics body to the simulation
    @note RemoveBody: Remove body from simulation
    @note DestroyBody: Deinitialized destroy body
    @note SetTransform: Sets initial position/orientation of physics body
    @note AddForce: Apply force to the body
    */

    m_camera = this->create_new_object("camera");

    camera_data.Position = { 0.0f, 1.50f, 0.0f };
    camera_data.Front = glm::vec3(-0.0f, 0.0f, -1.0f);

    m_camera->add<atlas::Camera>();

    sensitivity = m_camera->get<atlas::Camera>()->MovementSpeed;

    sync(this, &level_scene::on_update);
    sync_physics(this, &level_scene::on_physics_update);
    attach(this, &level_scene::on_ui_update);

    console_log_warn("World Tag After Initialization ======>>>>> {}",
                     atlas::system_registry::get_world().get_tag());
}

void
level_scene::on_ui_update() {

    if (ImGui::Begin("Viewport")) {
        glm::vec2 viewportPanelSize =
          glm::vec2{ atlas::application::get_window().get_width(),
                     atlas::application::get_window().get_height() };

        if (ImGui::Begin("Image")) {
            ImGui::End();
        }
        ImGui::End();
    }

    if (ImGui::Begin("Settings")) {
        ImGui::End();
    }

    if (ImGui::Begin("Scene Heirarchy")) {
        // @note right click on blank space
        // @param string_id
        // @param popup_flags - will be the mouse flag (0=right, 1=left)
        if (ui::BeginPopupContextWindow(0, 1, false)) {
            if (ImGui::MenuItem("Create Empty Entity")) {
                // m_scene_objects.insert({"Empty Entity",
                // this->create_new_object("Empty Object")});
            }
            ImGui::EndPopup();
        }
        ImGui::End();
    }

    if (ImGui::Begin("Content Browser")) {
        ImGui::End();
    }

    if (ImGui::Begin("Properties Panel")) {

        //! @note THERE IS AN ERROR. Where if the imgui docking window is
        //! outside of the window
        //! @note Imgui will just have a window that appears until when you exit
        //! the application and the UI is not docked outside the window
        atlas::ui::draw_panel_component<atlas::RenderTarget3D>("Sphere", [&]() {
            atlas::ui::draw_vec3("pos 1", sphere_data.Position);
            atlas::ui::draw_vec3("scale 1", sphere_data.Scale);
            atlas::ui::draw_vec3("rotate 1", sphere_data.Rotation);
            atlas::ui::draw_vec3("color 1", sphere_data.Color);
            atlas::ui::draw_vec3("Light Pos", g_light_position);
            atlas::ui::button_open_file_dialog("Load Mesh 1",
                                               sphere_data.mesh_file);

            if (sphere_data.mesh_file != "") {
                std::filesystem::path relative_path =
                  std::filesystem::relative(sphere_data.mesh_file, "./");
                console_log_trace("Filepath = {}", sphere_data.mesh_file);
                m_sphere->set<atlas::RenderTarget3D>(
                  { relative_path.string() });
                //! TODO: Empty String again to reset the filepath set
                sphere_data.mesh_file = "";
            }
        });

        atlas::ui::draw_panel_component<atlas::RenderTarget3D>(
          "Some Mesh", [&]() {
              atlas::ui::draw_vec3("Position 2", some_mesh_data.Position);
              atlas::ui::draw_vec3("Scale 2", some_mesh_data.Scale);
              atlas::ui::draw_vec3("Rotation 2", some_mesh_data.Rotation);
              atlas::ui::draw_vec3("Color 2", some_mesh_data.Color);
              atlas::ui::draw_float("Mouse Sensitivity", sensitivity);
              atlas::ui::button_open_file_dialog("Load Mesh 2",
                                                 some_mesh_data.mesh_file);

              //   if (some_mesh_data.mesh_file != "") {
              //       std::filesystem::path relative_path =
              //         std::filesystem::relative(some_mesh_data.mesh_file,
              //         "./");
              //       console_log_trace("in branch 2 mesh_file = {}",
              //                         relative_path.filename().string());
              //       m_platform->set<atlas::MeshComponent>(
              //         { some_mesh_data.mesh_file });
              //       some_mesh_data.mesh_file = "";
              //   }
          });

        // atlas::ui::draw_panel_component<atlas::Camera>(
        //   "Camera", [&]() {
        //       camera_data.Position = atlas::ui::draw_vec3("Position");
        //       camera_data.Front = atlas::ui::draw_vec3("Front");
        //   });

        if (ImGui::Button("Save As")) {
            std::string output_path = atlas::filesystem::SaveToFile("");
            // console_log_trace("Output Path = {}", output);
            atlas::serializer serializer(this);
            serializer.save_as(output_path);
        }

        ImGui::End();
    }
}

//! TODO: Separate between on_update tasks and OnRender-like tasks
void
level_scene::on_update() {

    auto camera_transform = *m_camera->get<atlas::Transform>();

    auto camera_comp = *m_camera->get<atlas::Camera>();

    //! TODO: Move DeltaTime out of global update
    //! TODO: global_update just uses delta timer for frame-rate stuff, and we
    //! should move that in timer namespace
    //! TODO: Add a reset() function simply for resetting the fps, or anything
    //! time related
    //! TODO: By either doing timer::reset(dt) or doing timer::reset()
    //! @note Where reset that takes a parameter resets the delta time
    //! specified.
    //! @note And timer::reset with no params resets our timer automatically
    //! (though would effect the entire timing variables though unless we
    //! separate them some way)
    float deltaTime = atlas::sync_update::delta_time();
    // float physics_step = atlas::timer::physcs_step();

    if (atlas::event::is_key_pressed(KEY_ESCAPE)) {
        atlas::application::get_window().close();
    }

    if (atlas::event::is_key_pressed(KEY_W)) {
        camera_comp.ProcessKeyboard(atlas::FORWARD, deltaTime);
    }
    if (atlas::event::is_key_pressed(KEY_S)) {
        camera_comp.ProcessKeyboard(atlas::BACKWARD, deltaTime);
    }
    if (atlas::event::is_key_pressed(KEY_A)) {
        camera_comp.ProcessKeyboard(atlas::LEFT, deltaTime);
    }
    if (atlas::event::is_key_pressed(KEY_D)) {
        camera_comp.ProcessKeyboard(atlas::RIGHT, deltaTime);
    }
    if (atlas::event::is_key_pressed(KEY_Q)) {
        camera_comp.ProcessKeyboard(atlas::UP, deltaTime);
    }
    if (atlas::event::is_key_pressed(KEY_E)) {
        camera_comp.ProcessKeyboard(atlas::DOWN, deltaTime);
    }

    //! @note Press shift key to move using the mouse to rotate around
    if (atlas::event::is_key_pressed(KEY_LEFT_SHIFT)) {
        if (atlas::event::is_mouse_pressed(MOUSE_BUTTON_RIGHT)) {
            glm::vec2 cursor_pos = atlas::event::cursor_position();

            float x_offset = cursor_pos.x;
            float velocity = x_offset * deltaTime;
            camera_comp.ProcessMouseMovement(-velocity, 0.f);
        }

        if (atlas::event::is_mouse_pressed(MOUSE_BUTTON_LEFT)) {
            glm::vec2 cursor_pos = atlas::event::cursor_position();

            float x_offset = cursor_pos.x;
            float velocity = x_offset * deltaTime;
            camera_comp.ProcessMouseMovement(velocity, 0.f);
        }

        if (atlas::event::is_mouse_pressed(MOUSE_BUTTON_MIDDLE)) {
            glm::vec2 cursor_pos = atlas::event::cursor_position();

            float velocity = cursor_pos.y * deltaTime;
            camera_comp.ProcessMouseMovement(0.f, velocity);
        }

        if (atlas::event::is_key_pressed(KEY_SPACE)) {
            glm::vec2 cursor_pos = atlas::event::cursor_position();
            float velocity = cursor_pos.y * deltaTime;
            camera_comp.ProcessMouseMovement(0.f, -velocity);
        }
    }

    //! @note
    camera_comp.MovementSpeed = sensitivity;
    camera_comp.UpdateProjView();
    camera_comp.IsMainCamera = true;

    m_camera->set<atlas::Camera>(camera_comp);
    m_camera->set<atlas::Transform>(camera_transform);

    atlas::Transform sphere_transform = {
        .Position = sphere_data.Position,
        .Rotation = { sphere_data.Rotation.x,
                      sphere_data.Rotation.y,
                      sphere_data.Rotation.z },
        .Scale = sphere_data.Scale,
        .Color = { sphere_data.Color.x,
                   sphere_data.Color.y,
                   sphere_data.Color.z,
                   1.0f },
    };

    atlas::Transform platform_transform = {
        .Position = some_mesh_data.Position,
        .Rotation = some_mesh_data.Rotation,
        .Scale = some_mesh_data.Scale,
        // .Color = platform
        .Color = { some_mesh_data.Color.x,
                   some_mesh_data.Color.y,
                   some_mesh_data.Color.z,
                   1.0f },
    };

    m_sphere->set<atlas::Transform>(sphere_transform);

    m_platform->set<atlas::Transform>(platform_transform);

    //! @note Update the physics runtime
    //! @note Also does a check if the physics is enabled automatically.
    // m_PhysicSceneSimulation.on_runtime_update(physics_step);
}

void
level_scene::on_physics_update() {
    // console_log_trace("on_physics_update Called!!!");

    //! @note For now this will be used to simulate the physics system
    //! @note This would be replaced with the play button. Once we get textures
    //! into the mix (after getting the viewport settled)
    if (atlas::event::is_key_pressed(KEY_L)) {
        m_is_simulation_enabled = true;
    }

    if (atlas::event::is_key_pressed(KEY_K)) {
        m_is_simulation_enabled = false;
    }

    if (m_is_simulation_enabled) {
        on_runtime_start();
    }
    else {
        on_runtime_stop();
    }

    // retreives us the timer frequency specifically for physics
    // float steps = atlas::time::physcs_step();

    if (m_is_simulation_enabled) {
        // m_physics_scene.on_runtime_update(steps);
    }
}

void
level_scene::on_runtime_start() {
    /*
        Physics Utilizing RigidBody3D Component

        Assume m_entity->add<RigidBody3D>();

        we would set our physics body in the physics scene below

        // Every time the transforms get modified then these will also be
       modified as well m_physics_scene.add_entity(m_sphere);
        m_physics_scene.add_entity(m_Floor);
    */
    // m_physics_scene = atlas::physics::physics_scene(this);

    // m_physics_scene.on_runtime_start();
}

void
level_scene::on_runtime_stop() {
    // m_physics_scene.on_runtime_stop();
}
