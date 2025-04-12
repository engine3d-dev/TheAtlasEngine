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
#include <renderer/renderer.hpp>

#include <core/timer.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

static float sensitivity = 0.f;

// static CameraData camera_data;
static std::string s_scene_filepath = "";
static glm::vec3 g_light_position = glm::vec3(0.0f, 0.0f, 1.0f);

namespace ui {}; // namespace ui

//! @brief These structs were just for testing add_query<UComponent...>()
struct Component1 {};

struct Component2 {};

struct Component3 {};

struct Component4 {};

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

    m_platform = this->create_new_object("mesh1");
    m_platform->set<atlas::Transform>({
      .Position = { 0.f, 1.40f, -7.4f },
      .Scale = { 2.80f, -0.08f, 3.50f },
    });

    m_platform->set<atlas::RenderTarget3D>({ "assets/models/cube.obj" });

    //! @note This is just for testing if we can query multiple components to
    //! the entities
    // m_test_object = create_new_object("test object");
    // m_test_object->add_query<component1, component2, component3,
    // component4>();

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

    // camera_data.Position = { 0.0f, 1.50f, 0.0f };
    // camera_data.Front = glm::vec3(-0.0f, 0.0f, -1.0f);

    m_camera->add<atlas::camera>();

    sensitivity = m_camera->get<atlas::camera>()->MovementSpeed;

    sync(this, &level_scene::on_update);
    sync_physics(this, &level_scene::on_physics_update);
    attach(this, &level_scene::on_ui_update);

    console_log_warn("World Tag After Initialization ======>>>>> {}",
                     atlas::system_registry::get_world().get_tag());
}

void
level_scene::on_ui_update() {
    atlas::Transform* sphere_transform = m_sphere->get_mut<atlas::Transform>();

    atlas::RenderTarget3D* sphere_render_object =
      m_sphere->get_mut<atlas::RenderTarget3D>();

    atlas::RenderTarget3D* platform_render_target =
      m_platform->get_mut<atlas::RenderTarget3D>();

    atlas::Transform* platform_transform =
      m_platform->get_mut<atlas::Transform>();

    if (ImGui::Begin("Viewport")) {
        glm::vec2 viewport_panel_size =
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
        if (atlas::ui::begin_popup_context_window(0, 1, false)) {
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
        //! TODO: Assign widgets in this lambda to correspond to panel's as
        //! groups
        //! @note Or else there will be conflict in naming ID's
        atlas::ui::draw_panel_component<atlas::RenderTarget3D>("Sphere", [&]() {
            std::string sphere_filepath = "";
            atlas::ui::draw_vec3("pos 1", sphere_transform->Position);
            atlas::ui::draw_vec3("scale 1", sphere_transform->Scale);
            atlas::ui::draw_vec3("rotate 1", sphere_transform->Rotation);
            atlas::ui::draw_vec4("color 1", sphere_transform->Color);
            atlas::ui::draw_vec3("Light Pos", g_light_position);
            atlas::ui::button_open_file_dialog("Load Mesh 1", sphere_filepath);

            if (sphere_filepath != "") {
                std::filesystem::path relative_path =
                  std::filesystem::relative(sphere_filepath, "./");
                console_log_trace("Sphere Filepath = {}", sphere_filepath);
                sphere_render_object->MeshMetaData = { relative_path.string() };
                //! TODO: Empty String again to reset the filepath set
                sphere_filepath = "";
            }
        });

        atlas::ui::draw_panel_component<atlas::RenderTarget3D>(
          "Platform", [&]() {
              std::string platform_filepath = "";
              atlas::ui::draw_vec3("Position 2", platform_transform->Position);
              atlas::ui::draw_vec3("Scale 2", platform_transform->Scale);
              atlas::ui::draw_vec3("Rotation 2", platform_transform->Rotation);
              atlas::ui::draw_vec4("Color 2", platform_transform->Color);
              atlas::ui::draw_float("Mouse Sensitivity", sensitivity);
              atlas::ui::button_open_file_dialog("Load Mesh 2",
                                                 platform_filepath);
              if (platform_filepath != "") {
                  std::filesystem::path relative_path =
                    std::filesystem::relative(platform_filepath, "./");
                  console_log_trace("Platform Filepath = {}",
                                    platform_filepath);
                  platform_render_target->MeshMetaData = {
                      relative_path.string()
                  };
                  //! TODO: Empty String again to reset the filepath set
                  platform_filepath = "";
              }
          });

        if (ImGui::Button("Save As")) {
            std::string output_path = atlas::filesystem::SaveToFile("");
            atlas::serializer serializer(this);
            serializer.save_as(output_path);
        }

        ImGui::End();
    }
}

//! TODO: Separate between on_update tasks and OnRender-like tasks
void
level_scene::on_update() {
    // auto camera_comp = *m_camera->get<atlas::Camera>();
    atlas::camera* camera = m_camera->get_mut<atlas::camera>();

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
    float delta_time = atlas::application::delta_time();
    // float physics_step = atlas::timer::physcs_step();

    if (atlas::event::is_key_pressed(KEY_ESCAPE)) {
        atlas::application::get_window().close();
    }

    if (atlas::event::is_key_pressed(KEY_W)) {
        // console_log_trace(""
        camera->process_keyboard(atlas::FORWARD, delta_time);
    }
    if (atlas::event::is_key_pressed(KEY_S)) {
        camera->process_keyboard(atlas::BACKWARD, delta_time);
    }
    if (atlas::event::is_key_pressed(KEY_A)) {
        camera->process_keyboard(atlas::LEFT, delta_time);
    }
    if (atlas::event::is_key_pressed(KEY_D)) {
        camera->process_keyboard(atlas::RIGHT, delta_time);
    }
    if (atlas::event::is_key_pressed(KEY_Q)) {
        camera->process_keyboard(atlas::UP, delta_time);
    }
    if (atlas::event::is_key_pressed(KEY_E)) {
        camera->process_keyboard(atlas::DOWN, delta_time);
    }

    //! @note Press shift key to move using the mouse to rotate around
    if (atlas::event::is_key_pressed(KEY_LEFT_SHIFT)) {
        if (atlas::event::is_mouse_pressed(MOUSE_BUTTON_RIGHT)) {
            glm::vec2 cursor_pos = atlas::event::cursor_position();

            float x_offset = cursor_pos.x;
            float velocity = x_offset * delta_time;
            camera->process_mouse_movement(-velocity, 0.f);
        }

        if (atlas::event::is_mouse_pressed(MOUSE_BUTTON_LEFT)) {
            glm::vec2 cursor_pos = atlas::event::cursor_position();

            float x_offset = cursor_pos.x;
            float velocity = x_offset * delta_time;
            camera->process_mouse_movement(velocity, 0.f);
        }

        if (atlas::event::is_mouse_pressed(MOUSE_BUTTON_MIDDLE)) {
            glm::vec2 cursor_pos = atlas::event::cursor_position();

            float velocity = cursor_pos.y * delta_time;
            camera->process_mouse_movement(0.f, velocity);
        }

        if (atlas::event::is_key_pressed(KEY_SPACE)) {
            glm::vec2 cursor_pos = atlas::event::cursor_position();
            float velocity = cursor_pos.y * delta_time;
            camera->process_mouse_movement(0.f, -velocity);
        }
    }

    camera->MovementSpeed = sensitivity;
    camera->update_proj_view();
    camera->IsMainCamera = true;
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
