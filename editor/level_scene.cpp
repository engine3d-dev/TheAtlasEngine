#include "level_scene.hpp"
#include <core/common.hpp>
#include <core/application.hpp>

level_scene::level_scene(const std::string& p_name)
  : atlas::scene_scope(p_name) {

    m_camera = create_object("editor camera");
    m_camera->add<flecs::pair<atlas::tag::editor, atlas::projection_view>>();
    m_camera->set<atlas::transform>(
      { .position = { 3.50f, 4.90f, 36.40f }, .scale{ 1.f } });
    m_camera->set<atlas::perspective_camera>({
      .plane = { 0.1f, 5000.f },
      .is_active = true,
      .field_of_view = 45.f,
    });

    m_viking_room = create_object("Viking Room Object");
	m_viking_room->add<atlas::tag::serialize>();
    m_viking_room->set<atlas::transform>({
		.position = { -2.70f, 2.70, -8.30f },
        .rotation = { 2.30f, 95.90f, 91.80f },
        .scale{ 1.f },
	});
    m_viking_room->set<atlas::material>(
      { .color = { 1.f, 1.f, 1.f, 1.f },
        .model_path = "assets/models/viking_room.obj",
        .texture_path = "assets/models/viking_room.png",
	});

    m_cube = create_object("Cube");

    m_cube->set<atlas::transform>({
		.position = { 0.f, 2.10f, -7.30f },
        .scale = { 0.9f, 0.9f, 0.9f },
	});

    m_cube->set<atlas::material>(
      { .color = { 1.f, 1.f, 1.f, 1.f },
        .model_path = "assets/models/E 45 Aircraft_obj.obj",
        .texture_path = "assets/models/E-45-steel detail_2_col.jpg",
	});

    m_robot_model = create_object("object 1");
	m_robot_model->add<atlas::tag::serialize>();
    m_robot_model->set<atlas::transform>({
      .position = { 0.f, 0.f, -20.f },
      .scale = { 1.f, 1.f, 1.f },
    });

    m_robot_model->set<atlas::material>({
		.color = { 1.f, 1.f, 1.f, 1.f },
        .model_path = "assets/models/robot.obj",
        .texture_path = "assets/models/container_diffuse.png",
	});

    m_platform = create_object("Platform");

    m_platform->set<atlas::transform>({
      .scale = { 15.f, -0.30f, 10.0f },
    });

    m_platform->set<atlas::material>({
      .model_path = "assets/models/cube.obj",
      .texture_path = "assets/models/wood.png",
    });

	atlas::register_start(this, &level_scene::start);
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

    atlas::transform* platform_transform =
      m_camera->get_mut<atlas::transform>();

    atlas::transform* cube_transform = m_cube->get_mut<atlas::transform>();

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
            }
            ImGui::EndPopup();
        }
        ImGui::End();
    }

    if (ImGui::Begin("Properties Panel")) {
        atlas::ui::draw_panel_component<atlas::material>("Sphere", [&]() {
            std::string sphere_filepath = "";
            atlas::ui::draw_vec3("pos 1", viking_transform->position);
            atlas::ui::draw_vec3("camera pos", camera_transform->position);
            atlas::ui::draw_vec3("scale 1", viking_transform->scale);
            atlas::ui::draw_vec3("rotate 1", viking_transform->rotation);
            atlas::ui::draw_vec4("color 1", viking_room_material->color);

            atlas::ui::draw_float("fov", persp_cam->field_of_view);

            atlas::ui::button_open_file_dialog("Load Mesh 1", sphere_filepath);

            if (sphere_filepath != "") {
                std::filesystem::path relative_path =
                  std::filesystem::relative(sphere_filepath, "./");
                console_log_trace("Sphere Filepath = {}", sphere_filepath);
                viking_room_material->model_path = { relative_path.string() };
                // Empty String again to reset the filepath set
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
                  auto quaternion = glm::quat(platform_transform->rotation);
                  platform_transform->quaternion = glm::vec4(
                    { quaternion.x, quaternion.y, quaternion.z, quaternion.w });
              }
              atlas::ui::draw_vec4("platform rgb", platform_material->color);
          });

        ImGui::End();
    }
}

void level_scene::start() {
	m_deserializer_test = atlas::serializer();

	if(!m_deserializer_test.load("LevelScene", *this)) {
		console_log_error("Could not load yaml file LevelScene!!!");
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

	glm::quat to_quaternion = atlas::to_quat(camera_transform->quaternion);

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

	camera_transform->set_rotation(camera_transform->rotation);
}
