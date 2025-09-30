#include "level_scene.hpp"
#include <core/common.hpp>
#include <core/application.hpp>
#include <drivers/jolt-cpp/jolt_components.hpp>
#include <physics/physics_3d/physics.hpp>
#include <physics/physics_engine.hpp>
#include <vector>

level_scene::level_scene(const std::string& p_name)
  : atlas::scene_scope(p_name) {
    m_camera = create_object("editor camera");
    m_camera->add<flecs::pair<atlas::tag::editor, atlas::projection_view>>();
    m_camera->set<atlas::transform>({
      .position = { 3.50f, 4.90f, 36.40f },
      .scale{ 1.f },
    });
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
    m_viking_room->set<atlas::material>({
      .color = { 1.f, 1.f, 1.f, 1.f },
      .model_path = "assets/models/viking_room.obj",
      .texture_path = "assets/models/viking_room.png",
    });

    m_cube = create_object("Aircraft");

    m_cube->set<atlas::transform>({
      .position = { 0.f, 2.10f, -7.30f },
      .scale = { 0.9f, 0.9f, 0.9f },
    });

    m_cube->set<atlas::material>({
      .color = { 1.f, 1.f, 1.f, 1.f },
      .model_path = "assets/models/E 45 Aircraft_obj.obj",
      .texture_path = "assets/models/E-45-steel detail_2_col.jpg",
    });

    m_robot_model = create_object("Robot");
    // m_robot_model->add<atlas::tag::serialize>();
    m_robot_model->set<atlas::transform>({
      .position = { 0.00f, 3.50f, 4.10f },
      .scale = { 1.f, 1.f, 1.f },
    });

    m_robot_model->set<atlas::material>({
      .color = { 1.f, 1.f, 1.f, 1.f },
      .model_path = "assets/models/cube.obj",
      .texture_path = "assets/models/container_diffuse.png",
    });

    m_robot_model->set<atlas::collider_body>({
      .shape_type = atlas::collider_shape::box,
      .half_extents = { 1.f, 1.f, 1.f },
    });
    m_robot_model->set<atlas::physics_body>({
      // .restitution = 1.25f,
      .body_movement_type = atlas::dynamic,
    });

    m_child_object = create_object("Child");
    m_child_object->child_of(m_robot_model);

    m_platform = create_object("Platform");

    m_platform->set<atlas::transform>({
      .scale = { 15.f, 0.30f, 10.0f },
    });
    m_platform->set<atlas::collider_body>({
      .shape_type = atlas::collider_shape::box,
      .half_extents = { 15.f, 0.30f, 10.0f },
      // .radius = 1.f,
      // .half_extents = {15.f, -0.30f, 10.0f}
    });

    m_platform->set<atlas::material>({
      .model_path = "assets/models/cube.obj",
      .texture_path = "assets/models/wood.png",
    });

    atlas::register_start(this, &level_scene::start);
    atlas::register_physics(this, &level_scene::physics_update);
    atlas::register_update(this, &level_scene::on_update);
    atlas::register_ui(this, &level_scene::on_ui_update);
}

void
level_scene::runtime_start() {
    // runs the physics simulation
    m_physics_is_runtime = true;

    m_physics_engine_handler.start();
}

void
level_scene::runtime_stop() {
    m_physics_is_runtime = false;

    m_physics_engine_handler.stop();

    reset_objects();
}

void
level_scene::reset_objects() {

    m_viking_room->set<atlas::transform>({
      .position = { -2.70f, 2.70, -8.30f },
      .rotation = { 2.30f, 95.90f, 91.80f },
      .scale{ 1.f },
    });

    m_cube->set<atlas::transform>({
      .position = { 0.f, 2.10f, -7.30f },
      .scale = { 0.9f, 0.9f, 0.9f },
    });
    m_robot_model->set<atlas::transform>({
      .position = { 0.00f, 3.50f, 4.10f },
      .scale = { 1.f, 1.f, 1.f },
    });

    m_platform->set<atlas::transform>({
      .scale = { 15.f, -0.30f, 10.0f },
    });
}

void
ui_component_list(flecs::entity& p_selected_entity) {
    std::string entity_name = p_selected_entity.name().c_str();

    atlas::ui::draw_input_text(entity_name);
    p_selected_entity.set_name(entity_name.c_str());

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);
    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("Add Component");
    }

    if (ImGui::BeginPopup("Add Component")) {
        if (!p_selected_entity.has<atlas::perspective_camera>()) {
            if (ImGui::MenuItem("Perspective Camera")) {
                p_selected_entity.add<
                  flecs::pair<atlas::tag::editor, atlas::projection_view>>();
                p_selected_entity.add<atlas::perspective_camera>();
                ImGui::CloseCurrentPopup();
            }
        }

        if (!p_selected_entity.has<atlas::material>()) {
            if (ImGui::MenuItem("Material (Mesh Component)")) {
                p_selected_entity.add<atlas::material>();
                ImGui::CloseCurrentPopup();
            }
        }

		if (!p_selected_entity.has<atlas::tag::serialize>()) {
            if (ImGui::MenuItem("Serialize")) {
                p_selected_entity.add<atlas::tag::serialize>();
                ImGui::CloseCurrentPopup();
            }
        }

        // NOTE -- Add this in later...
        // if (!p_selected_entity.has<atlas::tag::serialize>()) {
        //     if (ImGui::MenuItem("Tag::Serialize")) {
        //         p_selected_entity.add<atlas::tag::serialize>();
        //         ImGui::CloseCurrentPopup();
        //     }
        // }

        if (!p_selected_entity.has<atlas::collider_body>()) {
            if (ImGui::MenuItem("Collider")) {
                p_selected_entity.add<atlas::collider_body>();
                ImGui::CloseCurrentPopup();
            }
        }

        if (!p_selected_entity.has<atlas::physics_body>()) {
            if (ImGui::MenuItem("Physics Body")) {
                p_selected_entity.add<atlas::physics_body>();
                ImGui::CloseCurrentPopup();
            }
        }

        // if(!m_selected_entity.has<RigidBody2DComponent>()){
        // 	if(ImGui::MenuItem("Rigidbody 2D")){
        // 		m_selected_entity.add<RigidBody2DComponent>();
        // 		ImGui::CloseCurrentPopup();
        // 	}
        // }

        // if(!m_selected_entity.has<BoxCollider2DComponent>()){
        // 	if(ImGui::MenuItem("Box Collider 2D")){
        // 		m_selected_entity.add<BoxCollider2DComponent>();
        // 		ImGui::CloseCurrentPopup();
        // 	}
        // }
        ImGui::EndPopup();
    }

    ImGui::PopItemWidth();
}

void
level_scene::on_ui_update() {

    if (ImGui::Begin("Viewport")) {
        glm::vec2 viewport_panel_size =
          glm::vec2{ atlas::application::get_window().width(),
                     atlas::application::get_window().height() };

        ImGui::End();
    }

    defer_begin();
    auto query_builder = this->query_builder<atlas::transform>().build();

    if (ImGui::Begin("Scene Heirarchy")) {
        // @note right click on blank space
        // @param string_id
        // @param popup_flags - will be the mouse flag (0=right, 1=left)
        if (atlas::ui::begin_popup_context_window(nullptr, 1, false)) {
            if (ImGui::MenuItem("Create Empty Entity")) {
                // TODO -- Converting the operation to use strong_ptr to make
                // these operation more conformed
                m_create_entity = create_object("Empty Entity");
            }
            ImGui::EndPopup();
        }

        query_builder.each([&](flecs::entity p_entity, atlas::transform&) {
            // We set the imgui flags for our scene heirarchy panel
            // TODO -- Make the scene heirarchy panel a separate class that is
            // used for specify the layout and other UI elements here
            ImGuiTreeNodeFlags flags =
              ((m_selected_entity == p_entity) ? ImGuiTreeNodeFlags_Selected
                                               : 0) |
              ImGuiTreeNodeFlags_OpenOnArrow;
            flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
            flags |= ImGuiWindowFlags_Popup;
            bool opened = ImGui::TreeNodeEx(p_entity.name().c_str(), flags);
            if (ImGui::IsItemClicked()) {
                m_selected_entity = p_entity;
                // m_create_entity = search_entity(p_entity.name().c_str());
            }

            bool delete_entity = false;
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Delete Entity")) {
                    delete_entity = true;
                }
                ImGui::EndPopup();
            }

            if (delete_entity) {
                // _context->destroyEntity(entity);
                m_selected_entity.destruct();
            }

            if (opened) {
                flags = ImGuiTreeNodeFlags_OpenOnArrow |
                        ImGuiTreeNodeFlags_SpanAvailWidth;
                auto query_children_builder =
                  this->query_builder().with(flecs::ChildOf, p_entity).build();
                int32_t child_count = query_children_builder.count();

                // // Only show children in scene heirarchy panel if there are
                // children entities
                if (child_count > 0) {
                    m_selected_entity.children(
                      [&](flecs::entity p_child_entity) {
                          opened = ImGui::TreeNodeEx(
                            p_child_entity.name().c_str(), flags);
                          if (opened) {
                              if (ImGui::IsItemClicked()) {
                                  m_selected_entity = p_child_entity;
                                  // m_create_entity =
                                  // search_entity(p_child_entity.name().c_str());
                              }
                              ImGui::TreePop();
                          }
                      });
                }

                ImGui::TreePop();
            }
        });

        defer_end();
        ImGui::End();
    }

    if (ImGui::Begin("Properties")) {
        if (m_selected_entity.is_alive()) {
            ui_component_list(m_selected_entity);

            atlas::ui::draw_component<atlas::transform>(
              "transform",
              m_selected_entity,
              [](atlas::transform* p_transform) {
                  atlas::ui::draw_vec3("Position", p_transform->position);
                  atlas::ui::draw_vec3("Scale", p_transform->scale);
                  atlas::ui::draw_vec3("Rotation", p_transform->rotation);
              });

            atlas::ui::draw_component<atlas::perspective_camera>(
              "camera",
              m_selected_entity,
              [](atlas::perspective_camera* p_camera) {
                  atlas::ui::draw_float("field of view",
                                        p_camera->field_of_view);
                  ImGui::Checkbox("is_active", &p_camera->is_active);
              });

            atlas::ui::draw_component<atlas::material>(
              "material", m_selected_entity, [](atlas::material* p_material) {
                  atlas::ui::draw_input_text(p_material->model_path);
              });

            // atlas::ui::draw_component<atlas::collider_body>("Collider",
            // m_selected_entity, [](atlas::collider_body* p_collider)
            // {

            // });

            atlas::ui::draw_component<atlas::physics_body>(
              "Physics Body",
              m_selected_entity,
              [](atlas::physics_body* p_body) {
                  const char* items[] = { "Static", "Kinematic", "Dynamic" };
                  const char* combo_preview = items[p_body->body_type];

                  // Begin the combo box
                  if (ImGui::BeginCombo("Body Type", combo_preview)) {
                      for (int n = 0; n < 3; n++) {
                          // Check if the current item is selected
                          const bool is_selected = (p_body->body_type == n);
                          if (ImGui::Selectable(items[n], is_selected)) {
                              // Update the current type when a new item is
                              // selected
                              p_body->body_type =
                                static_cast<atlas::body_type>(n);
                          }

                          // Set the initial focus when the combo box is first
                          // opened
                          if (is_selected) {
                              ImGui::SetItemDefaultFocus();
                          }
                      }
                      ImGui::EndCombo();
                  }
              });

			atlas::ui::draw_component<atlas::tag::serialize>("Serialize", m_selected_entity, []([[maybe_unused]] atlas::tag::serialize* p_serialize){
				glm::vec3 val;
				atlas::ui::draw_vec3("Enable", val);
			});
        }

        ImGui::End();

        // Note --- just added this temporarily for testing
        // auto time = atlas::application::delta_time();

        // if((int)(time * 10.0f) % 8 > 4) {
        // 	m_blink = !m_blink;
        // }

        // auto width = atlas::application::get_window().width();
        // auto height = atlas::application::get_window().height();

        // ImGui::SetNextWindowPos(ImVec2(static_cast<float>(width) * 0.5f,
        // static_cast<float>(height) * 0.5f), ImGuiCond_Always, ImVec2(0.5f,
        // 0.5f)); ImGui::SetNextWindowSize(ImVec2(200, 20), ImGuiCond_Always);
        // ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
        // ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs;
        // ImGui::SetNextWindowBgAlpha(0.f);

        // if(ImGui::Begin("Testing", nullptr, flags)) {
        	// ImGui::ProgressBar(10.f);

        // 	auto pos = ImGui::GetWindowPos();
        // 	pos.x += (float)width * 0.5f - 300.0f;
        // 	pos.y += 50.0f;
        // 	if(m_blink){
        // 		ImGui::GetForegroundDrawList()->AddText(m_font, 120.0f,
        // pos, 0xffffffff, "Click to Play!");
        // 	}

        // 	ImGui::End();
        // }
    }
}

void
level_scene::start() {
    m_deserializer_test = atlas::serializer();

    if (!m_deserializer_test.load("LevelScene", *this)) {
        console_log_error("Could not load yaml file LevelScene!!!");
    }

	// Initiating physics system
	atlas::physics::jolt_settings settings = {};
	atlas::physics::jolt_config config = {};
	flecs::world registry = *this;
	m_physics_engine_handler = atlas::physics::physics_engine(settings, config, &registry);

    // Note -- just added for temporary
    // ImGuiIO io = ImGui::GetIO();
    // m_font = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf",
    // 120.0f);
}

void
level_scene::on_update() {
    // atlas::transform* camera_transform =
    // m_camera->get_mut<atlas::transform>();
    auto query_cameras =
      query_builder<atlas::perspective_camera, atlas::transform>().build();

    query_cameras.each(
      [](atlas::perspective_camera& p_camera, atlas::transform& p_transform) {
          if (!p_camera.is_active) {
              return;
          }

          // auto camera_transform = p_entity.get_mut<atlas::transform>();
          float dt = atlas::application::delta_time();
          float movement_speed = 10.f;
          float rotation_speed = 1.f;
          float velocity = movement_speed * dt;
          float rotation_velocity = rotation_speed * dt;

          glm::quat to_quaternion = atlas::to_quat(p_transform.quaternion);

          glm::vec3 up = glm::rotate(to_quaternion, glm::vec3(0.f, 1.f, 0.f));
          glm::vec3 forward =
            glm::rotate(to_quaternion, glm::vec3(0.f, 0.f, -1.f));
          glm::vec3 right =
            glm::rotate(to_quaternion, glm::vec3(1.0f, 0.0f, 0.0f));

          if (atlas::event::is_key_pressed(key_left_shift)) {
              if (atlas::event::is_mouse_pressed(
                    atlas::event::Mouse::ButtonMiddle)) {
                  p_transform.position += up * velocity;
              }

              if (atlas::event::is_mouse_pressed(
                    atlas::event::Mouse::ButtonRight)) {
                  p_transform.position -= up * velocity;
              }
          }

          if (atlas::event::is_key_pressed(key_w)) {
              p_transform.position += forward * velocity;
          }
          if (atlas::event::is_key_pressed(key_s)) {
              p_transform.position -= forward * velocity;
          }

          if (atlas::event::is_key_pressed(key_d)) {
              p_transform.position += right * velocity;
          }
          if (atlas::event::is_key_pressed(key_a)) {
              p_transform.position -= right * velocity;
          }

          if (atlas::event::is_key_pressed(key_q)) {
              p_transform.rotation.y += rotation_velocity;
          }
          if (atlas::event::is_key_pressed(key_e)) {
              p_transform.rotation.y -= rotation_velocity;
          }

          p_transform.set_rotation(p_transform.rotation);
      });
}

void
level_scene::physics_update() {
    if (atlas::event::is_key_pressed(key_r) and !m_physics_is_runtime) {
        runtime_start();
    }

    if (m_physics_is_runtime) {
        m_physics_engine_handler.update();
        // m_physics_engine_handler.execute_collisions();
    }

    if (atlas::event::is_key_pressed(key_l) and m_physics_is_runtime) {
        runtime_stop();
    }
}
