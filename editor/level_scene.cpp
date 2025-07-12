
#include "level_scene.hpp"
#include <core/event/event.hpp>
#include <core/ui/widgets.hpp>
#include <core/serialize/serializer.hpp>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <core/scene/components.hpp>
#include <core/utilities/state.hpp>
#include <core/application.hpp>

level_scene::level_scene(const std::string& p_tag)
  : atlas::scene_scope(p_tag) {
    console_log_info("scene_scope::scene_scope with Tag = {} called!", p_tag);

    m_camera = create_object("editor camera");
    m_camera
      ->add<flecs::pair<atlas::tag_redo::editor, atlas::projection_view>>();
    m_camera->set<atlas::transform>(
      { .position = { 3.50f, 4.90f, 36.40f }, .scale{ 1.f } });
    m_camera->set<atlas::perspective_camera>({
      .name = "Camera 1",
      .plane = { 0.1f, 5000.f },
      .is_active = true,
      .field_of_view = 45.f,
      .target = atlas::screen,
    });

    // m_camera2 = create_object("game camera");
    // m_camera2->add<flecs::pair<atlas::tag_redo::editor,
    // atlas::projection_view>>(); m_camera2->set<atlas::transform>({ .position
    // = {-2.70f, 2.70, -8.30f}, 	.scale{1.f}
    // });
    // m_camera2->set<atlas::perspective_camera>({
    // 	.name = "Camera 2",
    //     .plane = {0.1f, 1000.f},
    //     .is_active = false,
    //     .target = atlas::screen
    // });

    // float aspect_ratio = atlas::application::aspect_ratio();
    // atlas::camera camera_comp = atlas::camera(aspect_ratio);
    // camera_comp.Position = { -1.1f, 6.53f, 23.32f };
    // camera_comp.IsMainCamera = true;

    // m_camera->set<atlas::camera>(camera_comp);

    m_viking_room = create_object("Viking Room Object");
    atlas::transform sphere_transform = {
        // .position{0.f, 0.f, 1.60f},
        .position = { -2.70f, 2.70, -8.30f },
        .rotation = { 2.30f, 95.90f, 91.80f },
        .scale{ 1.f },
    };
    m_viking_room->set<atlas::transform>(sphere_transform);
    m_viking_room->set<atlas::material>(
      { .color = { 1.f, 1.f, 1.f, 1.f },
        .model_path = "assets/models/viking_room.obj",
        .texture_path = "assets/models/viking_room.png" });

    m_cube = create_object("Cube");

    atlas::transform cube_transform = {
        // .position = {-2.70, 0.f, 0.f},
        .position = { 0.f, 2.10f, -7.30f },
        .scale = { 0.9f, 0.9f, 0.9f },
    };
    m_cube->set<atlas::transform>(cube_transform);

    m_cube->set<atlas::material>(
      { .color = { 1.f, 1.f, 1.f, 1.f },
        .model_path = "assets/models/E 45 Aircraft_obj.obj",
        .texture_path = "assets/models/E-45-steel detail_2_col.jpg" });

    m_robot_model = create_object("object 1");
    m_robot_model->set<atlas::transform>({
      .position = { 0.f, 0.f, -20.f },
      .scale = { 1.f, 1.f, 1.f },
    });

    m_robot_model->set<atlas::material>({
      .color = { 1.f, 1.f, 1.f, 1.f },
      .model_path = "assets/models/robot.obj",
      // .model_path = "assets/models/Box.gltf",
      .texture_path = "assets/models/container_diffuse.png"
      // .texture_path = "assets/models/Tiles083_8K-JPG_AmbientOcclusion.jpg"
    });

    m_platform = create_object("Platform");

    m_platform->set<atlas::transform>({ // .position = { 0.f, 1.40f, -7.4f },
                                        // .scale = { 2.80f, -0.08f, 3.50f },
                                        .scale = { 15.f, -0.30f, 10.0f } });

    m_platform->set<atlas::material>({
      //   .color = { 0.f, 1.f, 0.f, 1.f },
      .model_path = "assets/models/cube.obj",
      .texture_path = "assets/models/wood.png",
      // .texture_path = "assets/models/Tiles074_8K-JPG_Color.jpg",
    });

    atlas::register_update(this, &level_scene::on_update);
    atlas::register_ui(this, &level_scene::on_ui_update);
}

void
level_scene::on_ui_update() {
    flecs::world reg = *this;
    atlas::transform* viking_transform =
      m_viking_room->get_mut<atlas::transform>();
    atlas::transform* robot_transform =
      m_robot_model->get_mut<atlas::transform>();
    // atlas::transform* platform_transform =
    // m_platform->get_mut<atlas::transform>();

    atlas::transform* platform_transform =
      m_camera->get_mut<atlas::transform>();

    // flecs::entity platform = reg.entity("Platform");
    // atlas::transform* platform_transform =
    // platform.get_mut<atlas::transform>();
    atlas::transform* cube_transform = m_cube->get_mut<atlas::transform>();
    // atlas::camera* camera_transform = m_camera->get_mut<atlas::camera>();

    atlas::material* viking_room_material =
      m_viking_room->get_mut<atlas::material>();
    atlas::material* platform_material = m_platform->get_mut<atlas::material>();

    atlas::transform* camera_transform = m_camera->get_mut<atlas::transform>();

    atlas::perspective_camera* persp_cam =
      m_camera->get_mut<atlas::perspective_camera>();

    if (ImGui::Begin("Viewport")) {
        glm::vec2 viewport_panel_size =
          glm::vec2{ atlas::application::get_window().width(),
                     atlas::application::get_window().height() };

        ImGui::End();
    }

    if (ImGui::Begin("Scene Heirarchy")) {
        // @note right click on blank space
        // @param string_id
        // @param popup_flags - will be the mouse flag (0=right, 1=left)
        if (atlas::ui::begin_popup_context_window(nullptr, 1, false)) {
            if (ImGui::MenuItem("Create Empty Entity")) {
                // m_scene_objects.insert({"Empty Entity",
                // this->create_new_object("Empty Object")});
            }
            ImGui::EndPopup();
        }
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
        atlas::ui::draw_panel_component<atlas::material>("Sphere", [&]() {
            std::string sphere_filepath = "";
            atlas::ui::draw_vec3("pos 1", viking_transform->position);
            atlas::ui::draw_vec3("camera pos", camera_transform->position);
            atlas::ui::draw_vec3("scale 1", viking_transform->scale);
            atlas::ui::draw_vec3("rotate 1", viking_transform->rotation);
            atlas::ui::draw_vec4("color 1", viking_room_material->color);

            atlas::ui::draw_float("fov", persp_cam->field_of_view);

            // atlas::ui::draw_vec3("Light Pos", g_light_position);
            atlas::ui::button_open_file_dialog("Load Mesh 1", sphere_filepath);
            // atlas::ui::draw_vec3("camera pos", camera_transform->Position);

            if (sphere_filepath != "") {
                std::filesystem::path relative_path =
                  std::filesystem::relative(sphere_filepath, "./");
                console_log_trace("Sphere Filepath = {}", sphere_filepath);
                viking_room_material->model_path = { relative_path.string() };
                //! TODO: Empty String again to reset the filepath set
                sphere_filepath = "";
            }
        });

        atlas::ui::draw_panel_component<atlas::material>("Robot", [&]() {
            atlas::ui::draw_vec3("position", robot_transform->position);
            atlas::ui::draw_vec3("rob scale", robot_transform->scale);
        });

        atlas::ui::draw_panel_component<atlas::material>("Cube", [&]() {
            atlas::ui::draw_vec3("cube pos", cube_transform->position);
        });

        atlas::ui::draw_panel_component<atlas::material>(
          "platform", [&platform_transform, &platform_material]() {
              atlas::ui::draw_vec3("platform pos",
                                   platform_transform->position);
              atlas::ui::draw_vec3("platform scale", platform_transform->scale);
              if (ImGui::DragFloat3(
                    "Some Pos", glm::value_ptr(platform_transform->rotation))) {
                  // glm::quat to_quaternion = ;
                  auto quaternion = glm::quat(platform_transform->rotation);
                  platform_transform->quaternion = glm::vec4(
                    { quaternion.x, quaternion.y, quaternion.z, quaternion.w });
              }
              atlas::ui::draw_vec4("platform rgb", platform_material->color);
          });

        ImGui::End();
    }
}

void
level_scene::on_update() {
    atlas::transform* camera_transform = m_camera->get_mut<atlas::transform>();
    float dt = atlas::application::delta_time();
    float movement_speed = 10.f;
    float rotation_speed = 1.f;
    float velocity = movement_speed * dt;
    float rotation_velocity = rotation_speed * dt;

    // auto to_quaternion = glm::quat(camera_transform->quaternion);
    glm::quat to_quaternion = glm::quat({ camera_transform->quaternion.w,
                                          camera_transform->quaternion.x,
                                          camera_transform->quaternion.y,
                                          camera_transform->quaternion.z });

    glm::vec3 up = glm::rotate(to_quaternion, glm::vec3(0.f, 1.f, 0.f));
    glm::vec3 forward = glm::rotate(to_quaternion, glm::vec3(0.f, 0.f, -1.f));
    glm::vec3 right = glm::rotate(to_quaternion, glm::vec3(1.0f, 0.0f, 0.0f));

    if (atlas::event::is_key_pressed(key_left_shift)) {
        if (atlas::event::is_mouse_pressed(atlas::event::Mouse::ButtonMiddle)) {
            camera_transform->position += up * velocity;
        }

        if (atlas::event::is_mouse_pressed(atlas::event::Mouse::ButtonRight)) {
            camera_transform->position -= up * velocity;
        }
    }

    if (atlas::event::is_key_pressed(key_w)) {
        camera_transform->position += forward * velocity;
    }
    if (atlas::event::is_key_pressed(key_s)) {
        camera_transform->position -= forward * velocity;
    }

    if (atlas::event::is_key_pressed(key_d)) {
        camera_transform->position += right * velocity;
    }
    if (atlas::event::is_key_pressed(key_a)) {
        camera_transform->position -= right * velocity;
    }

    if (atlas::event::is_key_pressed(key_q)) {
        camera_transform->rotation.y += rotation_velocity;
    }
    if (atlas::event::is_key_pressed(key_e)) {
        camera_transform->rotation.y -= rotation_velocity;
    }

    auto quaternion = glm::quat(camera_transform->rotation);
    camera_transform->quaternion =
      glm::vec4({ quaternion.x, quaternion.y, quaternion.z, quaternion.w });

    // atlas::camera* camera = m_camera->get_mut<atlas::camera>();

    // float delta_time = atlas::application::delta_time();

    // if (atlas::event::is_key_pressed(key_w)) {
    //     camera->process_keyboard(atlas::forward, delta_time);
    // }
    // if (atlas::event::is_key_pressed(key_s)) {
    //     camera->process_keyboard(atlas::backward, delta_time);
    // }
    // if (atlas::event::is_key_pressed(key_a)) {
    //     camera->process_keyboard(atlas::left, delta_time);
    // }
    // if (atlas::event::is_key_pressed(key_d)) {
    //     camera->process_keyboard(atlas::right, delta_time);
    // }
    // if (atlas::event::is_key_pressed(key_q)) {
    //     camera->process_keyboard(atlas::up, delta_time);
    // }
    // if (atlas::event::is_key_pressed(key_e)) {
    //     camera->process_keyboard(atlas::down, delta_time);
    // }

    // //! @note Press shift key to move using the mouse to rotate around
    // if (atlas::event::is_key_pressed(key_left_shift)) {
    //     if (atlas::event::is_mouse_pressed(mouse_button_right)) {
    //         glm::vec2 cursor_pos = atlas::event::cursor_position();

    //         float x_offset = cursor_pos.x;
    //         float velocity = x_offset * delta_time;
    //         camera->process_mouse_movement(-velocity, 0.f);
    //     }

    //     if (atlas::event::is_mouse_pressed(mouse_button_left)) {
    //         glm::vec2 cursor_pos = atlas::event::cursor_position();

    //         float x_offset = cursor_pos.x;
    //         float velocity = x_offset * delta_time;
    //         camera->process_mouse_movement(velocity, 0.f);
    //     }

    //     if (atlas::event::is_mouse_pressed(mouse_button_middle)) {
    //         glm::vec2 cursor_pos = atlas::event::cursor_position();

    //         float velocity = cursor_pos.y * delta_time;
    //         camera->process_mouse_movement(0.f, velocity);
    //     }

    //     if (atlas::event::is_key_pressed(key_space)) {
    //         glm::vec2 cursor_pos = atlas::event::cursor_position();
    //         float velocity = cursor_pos.y * delta_time;
    //         camera->process_mouse_movement(0.f, -velocity);
    //     }
    // }

    // // camera->MovementSpeed = camera->MovementSpeed;
    // camera->update_proj_view();
}
