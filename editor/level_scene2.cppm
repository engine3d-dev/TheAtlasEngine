module;

#include <string>
#include <flecs.h>
#include <optional>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

export module editor:level_scene2;

import atlas.application;
import atlas.core.event;
import atlas.core.scene;
import atlas.core.utilities;
import atlas.core.math;

// Creating this class for experimentation
export class level_scene2 final : public atlas::scene {
public:
    level_scene2(const std::string& p_name, atlas::event::bus& p_bus)
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

        atlas::game_object cube = entity("Cube");
        cube.set<atlas::transform>({
          .position = { -2.70f, 2.70, -8.30f },
          .rotation = { 2.30f, 95.90f, 91.80f },
          .scale{ 1.f },
        });

        cube.set<atlas::mesh_source>({
          .model_path = "assets/models/cube.obj",
          .diffuse = "assets/models/container_diffuse.png",
        });

        atlas::register_start(this, &level_scene2::start);
        atlas::register_physics(this, &level_scene2::physics_update);
        atlas::register_update(this, &level_scene2::on_update);
    }

    ~level_scene2() override = default;

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

        if (atlas::event::is_key_pressed(key_left_shift)) {
            t->position += up * velocity;
        }

        if (atlas::event::is_key_pressed(key_space)) {
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

        if (atlas::event::is_key_pressed(key_q)) {
            t->rotation.y += rotation_velocity;
        }
        if (atlas::event::is_key_pressed(key_e)) {
            t->rotation.y -= rotation_velocity;
        }

        // Signal to trigger this kind of scene transition
        if (atlas::event::is_key_pressed(key_n)) {
            console_log_error("Signaling to transition to level scene");
            atlas::event::scene_transition scene_transition = {
                .next_scene = "Level Scene",
            };
            signal(scene_transition);
        }

        t->set_rotation(t->rotation);
    }

    // TODO: Have this physics_update be executed during the physics
    // fixed-update framerate
    void physics_update() {}

    // void on_signal(atlas::event::scene_transition& p_transition) {
    //     p_transition.next_scene = "Level Scene 2";
    // }

private:
    std::optional<atlas::game_object> m_editor_camera;
    float m_movement_speed = 10.f;
};