module;

#include <string>
#include <print>
#include <flecs.h>
#include <optional>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>

export module editor:level_scene;

import atlas.application;
import atlas.core.event;
import atlas.core.scene;
import atlas.core.scene.game_object;
import atlas.core.scene.components;
import atlas.core.utilities;
import atlas.core.math;

export class level_scene final : public atlas::scene {
public:
    level_scene(const std::string& p_name, atlas::event::bus& p_bus)
      : atlas::scene(p_name, p_bus) {

        m_editor_camera = entity("Editor Camera");
        m_editor_camera
          ->add<flecs::pair<atlas::tag::editor, atlas::projection_view>>();
        m_editor_camera->set<atlas::transform>({
          .position = { 3.50f, 4.90f, 36.40f },
          .scale{ 1.f },
        });
        m_editor_camera->set<atlas::perspective_camera>({
          .plane = { 0.1f, 5000.f },
          .is_active = true,
          .field_of_view = 45.f,
        });

        atlas::game_object viking_room = entity("Viking Room");
        viking_room.set<atlas::transform>({
          .position = { 5, 5, 0.f },
        //   .position = { 1.f, 1.f, 1.f },
          .rotation = { 2.30f, 95.90f, 91.80f },
        //   .rotation = { 1.f, 1.f, 1.f },
          .scale{ 1.f },
        });

        viking_room.set<atlas::mesh_source>({
          .model_path = "assets/models/viking_room.obj",
          .diffuse = "assets/models/viking_room.png",
        //   .model_path = "assets/models/cube.obj",
        //   .diffuse = "assets/models/container_diffuse.png",
        //   .diffuse = "assets/models/container_specular.png",
        });


        // const size_t grid_width = 5; 
        // const float spacing = 5;

        // for(size_t i = 0; i < 100; i++) {
        //     atlas::game_object obj = entity(std::format("Object #{}", i));

        //     size_t col = i % grid_width;
        //     size_t row = i / grid_width;

        //     glm::vec3 pos = {
        //         float(static_cast<float>(col) * spacing),
        //         0.0f,
        //         float(static_cast<float>(row) * -spacing) 
        //     };

        //     obj.set<atlas::transform>({
        //         .position = pos,
        //         .rotation = {.3f, 0.0f, 0.0f},
        //     });
        //     obj.set<atlas::mesh_source>({
        //         .model_path = "assets/models/Ball OBJ.obj",
        //         .diffuse = "assets/models/clear.png",
        //     });
        // }

        atlas::register_start(this, &level_scene::start);
        atlas::register_physics(this, &level_scene::physics_update);
        atlas::register_update(this, &level_scene::on_update);
    }

    ~level_scene() override = default;

    void start() {}

    void on_update(float p_delta_time) {
        atlas::transform* t = m_editor_camera->get_mut<atlas::transform>();
        float dt = p_delta_time;

        // current default movement speed that does not applied modified speed
        float default_speed = 10.f;
        float rotation_speed = 1.f;
        float velocity = default_speed * dt;
        if (atlas::event::is_mouse_pressed(mouse_button_middle)) {
            velocity = m_movement_speed * dt;
        }

        float rotation_velocity = rotation_speed * dt;

        glm::quat to_quaternion = atlas::to_quat(t->quaternion);

        glm::vec3 up = glm::rotate(to_quaternion, atlas::math::up());
        glm::vec3 forward = glm::rotate(to_quaternion, atlas::math::backward());
        glm::vec3 right = glm::rotate(to_quaternion, atlas::math::right());

        // Q = UP
        if (atlas::event::is_key_pressed(key_q)) {
            t->position += up * velocity;
        }

        // E = DOWN
        if (atlas::event::is_key_pressed(key_e)) {
            t->position -= up * velocity;
        }

        if (atlas::event::is_key_pressed(key_w)) {
            t->position += forward * velocity;
        }
        if (atlas::event::is_key_pressed(key_s)) {
            t->position -= forward * velocity;
        }

        if (atlas::event::is_key_pressed(key_d)) {
            t->position += right * velocity;
        }
        if (atlas::event::is_key_pressed(key_a)) {
            t->position -= right * velocity;
        }

        glm::vec2 current_cursor_pos = atlas::event::cursor_position();
        if(atlas::event::is_key_pressed(key_left_shift)) {
            glm::vec2 cursor_dt = current_cursor_pos - m_last_cursor_pos;
            float mouse_sensitivity = 0.1;

            // Update Euler angles

            if(atlas::event::is_mouse_pressed(mouse_button_left)) {
                t->rotation.y -= (cursor_dt.x * mouse_sensitivity) * rotation_velocity;
            }

            if(atlas::event::is_mouse_pressed(mouse_button_right)) {
                t->rotation.x -= (cursor_dt.y * mouse_sensitivity) * rotation_velocity;
            }

            // Clamp pitch to prevent the camera from flipping completely upside down (optional but recommended)
            t->rotation.x = glm::clamp(t->rotation.x, -89.0f, 89.0f);
        }

        // Signal to trigger this kind of scene transition
        // Experimental: This was used for testing.
        // if(atlas::event::is_key_pressed(key_n)) {
        //     std::println("Signaling to transition to next_scene");
        //     atlas::event::scene_transition scene_transition = {
        //         .next_scene = "Level Scene 2",
        //     };
        //     signal<atlas::event::scene_transition>(scene_transition);
        // }
        t->set_rotation(t->rotation);

        m_last_cursor_pos = current_cursor_pos;
    }

    // TODO: Have this physics_update be executed during the physics
    // fixed-update framerate
    void physics_update() {}

private:
    std::optional<atlas::game_object> m_editor_camera;
    float m_movement_speed = 10.f;
    glm::vec2 m_last_cursor_pos{};
};