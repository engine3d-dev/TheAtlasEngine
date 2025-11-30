#include "level_scene.hpp"
#include <core/common.hpp>
#include <core/application.hpp>
#include <drivers/jolt-cpp/jolt_components.hpp>
#include <physics/physics_engine.hpp>

level_scene::level_scene(const std::string& p_name,
                         atlas::event::event_bus& p_bus)
  : atlas::scene(p_name, p_bus) {
    m_camera = create("Editor Camera");
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

    m_bob_object = create("Bob");
    m_bob_object->add<atlas::point_light>();

    m_viking_room = create("Viking Room");
    m_viking_room->add<atlas::tag::serialize>();
    m_viking_room->set<atlas::transform>({
      .position = { -2.70f, 2.70, -8.30f },
      .rotation = { 2.30f, 95.90f, 91.80f },
      .scale{ 1.f },
    });

    m_viking_room->set<atlas::sphere_collider>({
      .radius = 1.0f,
    });

    m_viking_room->set<atlas::physics_body>({
      .friction = 15.f,
      .restitution = 0.3f,
      .body_movement_type = atlas::dynamic,
    });

    m_cube = create("Aircraft");

    m_cube->set<atlas::transform>({
      .position = { 0.f, 2.10f, -7.30f },
      .scale = { 0.9f, 0.9f, 0.9f },
    });

    m_cube->set<atlas::mesh_source>({
      .color = { 1.f, 1.f, 1.f, 1.f },
      // .model_path = "assets/models/E 45 Aircraft_obj.obj",
      .model_path = "assets/backpack/backpack.obj",
      .diffuse = "assets/backpack/diffuse.jpg",
      .specular = "assets/backpack/specular.jpg"
      //   .diffuse = "assets/models/E-45-steel detail_2_col.jpg",
    });

    m_robot_model = create("Cube");
    m_robot_model->add<atlas::tag::serialize>();
    // m_robot_model->add<atlas::tag::serialize>();
    m_robot_model->set<atlas::transform>({
      .position = { -2.70, 3.50f, 4.10f },
      .scale = { 1.f, 1.f, 1.f },
    });

    m_robot_model->set<atlas::mesh_source>(
      { .color = { 1.f, 1.f, 1.f, 1.f },
        .model_path = "assets/models/cube.obj",
        .diffuse = "assets/models/container_diffuse.png",
        .specular = "assets/models/container_specular.png" });

    m_robot_model->set<atlas::box_collider>({
      .half_extent = { 1.f, 1.f, 1.f },
    });
    m_robot_model->set<atlas::physics_body>({
      //   .restitution = 1.f,
      .body_movement_type = atlas::dynamic,
    });

    m_platform = create("Platform");

    m_platform->set<atlas::transform>({
      .scale = { 15.f, 0.30f, 10.0f },
    });
    m_platform->set<atlas::mesh_source>({
      .model_path = "assets/models/cube.obj",
      .diffuse = "assets/models/wood.png",
    });
    m_platform->set<atlas::physics_body>({
      .body_movement_type = atlas::fixed,
    });
    m_platform->set<atlas::box_collider>({
      .half_extent = { 15.f, 0.30f, 10.0f },
    });

    m_point_light = create("Point Light 1");
    m_point_light->set<atlas::transform>({
      .position = { 0.f, 2.10f, -7.30f },
      .scale = { 0.9f, 0.9f, 0.9f },
    });

    m_point_light->set<atlas::mesh_source>({
      .model_path = "assets/models/cube.obj",
      .diffuse = "assets/models/wood.png",
    });
    m_point_light->add<atlas::tag::serialize>();

    // for(size_t i = 0; i < 26; i++) {
    // 	auto obj = create(std::format("Object #{}", i));
    // 	obj->set<atlas::physics_body>({
    // 		.restitution = 1.25f,
    // 		.body_movement_type = atlas::dynamic,
    // 	});

    // 	obj->set<atlas::sphere_collider>(
    // 		{
    // 		.radius = 1.0f,
    // 	});

    // 	glm::vec3 pos = {float(0*1.4),float(0 * 1.4),float(0 * -3) };

    // 	obj->set<atlas::transform>({
    // 		.position = pos,
    // 		.rotation = {.3f, 0.0f, 0.0f},
    // 	});

    // 	obj->set<atlas::mesh_source>({
    // 		.model_path = "assets/models/Ball OBJ.obj",
    // 		.diffuse = "assets/models/clear.png",
    // 	});
    // 	m_many_objects.emplace_back(obj);
    // }

    // TODO: Move this outside of level_scene
    m_deserializer_test = atlas::serializer();

    subscribe<atlas::event::collision_enter>(this,
                                             &level_scene::collision_enter);

    atlas::register_start(this, &level_scene::start);
    atlas::register_physics(this, &level_scene::physics_update);
    atlas::register_update(this, &level_scene::on_update);
    atlas::register_ui(this, &level_scene::on_ui_update);
}

void
level_scene::collision_enter(atlas::event::collision_enter& p_event) {
    console_log_warn("collision_enter event!!!");
    flecs::world registry = *this;
    flecs::entity e1 = registry.entity(p_event.entity1);
    flecs::entity e2 = registry.entity(p_event.entity2);

    console_log_warn("Entity1 = {}", e1.name().c_str());
    console_log_warn("Entity2 = {}", e2.name().c_str());
}

void
level_scene::collision_persisted(atlas::event::collision_persisted& p_event) {
    console_log_warn("collision_persisted(p_event) invoked!!");
    flecs::world registry = *this;
    flecs::entity e1 = registry.entity(p_event.entity1);
    flecs::entity e2 = registry.entity(p_event.entity2);

    console_log_warn("Entity1 = {}", e1.name().c_str());
    console_log_warn("Entity2 = {}", e2.name().c_str());
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

    if (!m_deserializer_test.load("LevelScene", *this)) {
        console_log_error("Could not load yaml file LevelScene!!!");
    }
}

void
ui_component_list(flecs::entity& p_selected_entity) {
    std::string entity_name = p_selected_entity.name().c_str();
    atlas::ui::draw_input_text(entity_name);

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

        if (!p_selected_entity.has<atlas::mesh_source>()) {
            if (ImGui::MenuItem("Mesh Source")) {
                p_selected_entity.add<atlas::mesh_source>();
                ImGui::CloseCurrentPopup();
            }
        }

        if (!p_selected_entity.has<atlas::point_light>()) {
            if (ImGui::MenuItem("Point Light")) {
                p_selected_entity.add<atlas::point_light>();
                ImGui::CloseCurrentPopup();
            }
        }

        if (!p_selected_entity.has<atlas::tag::serialize>()) {
            if (ImGui::MenuItem("Serialize")) {
                p_selected_entity.add<atlas::tag::serialize>();
                ImGui::CloseCurrentPopup();
            }
        }

        if (!p_selected_entity.has<atlas::physics_body>()) {
            if (ImGui::MenuItem("Physics Body")) {
                p_selected_entity.add<atlas::physics_body>();
                ImGui::CloseCurrentPopup();
            }
        }

        if (!p_selected_entity.has<atlas::box_collider>()) {
            if (ImGui::MenuItem("Box Collider")) {
                p_selected_entity.add<atlas::box_collider>();
                ImGui::CloseCurrentPopup();
            }
        }

        if (!p_selected_entity.has<atlas::sphere_collider>()) {
            if (ImGui::MenuItem("Sphere Collider")) {
                p_selected_entity.add<atlas::sphere_collider>();
                ImGui::CloseCurrentPopup();
            }
        }

        if (!p_selected_entity.has<atlas::capsule_collider>()) {
            if (ImGui::MenuItem("Capsule Collider")) {
                p_selected_entity.add<atlas::capsule_collider>();
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }

    ImGui::PopItemWidth();
}

void
level_scene::on_ui_update() {

    // setting up the dockspace UI widgets at the window toolbar
    m_editor_dockspace.begin();

    try {
        m_editor_menu.begin();
    }
    catch (const atlas::ui::menu_bar_exception& e) {
    }

    // specify the label and the state to execute when this specific widget has
    // been triggered
    m_editor_menu.add_child("Exit", []() {
        glfwSetWindowShouldClose(atlas::application::get_window(), true);
    });
    m_editor_menu.end();

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
                // m_create_entity = create("Empty Entity");
                m_current_entity = create("Empty Entity");
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
                    m_selected_entity.children([&](flecs::entity p_child) {
                        opened =
                          ImGui::TreeNodeEx(p_child.name().c_str(), flags);
                        if (opened) {
                            if (ImGui::IsItemClicked()) {
                                m_selected_entity = p_child;
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
              [this](atlas::perspective_camera* p_camera) {
                  atlas::ui::draw_float("field of view",
                                        p_camera->field_of_view);
                  ImGui::Checkbox("is_active", &p_camera->is_active);
                  ImGui::DragFloat("Speed", &m_movement_speed);
              });

            /*
            atlas::ui::draw_component<atlas::directional_light>("Directional
            Light", m_selected_entity, [](atlas::directional_light*
            p_dir_light){ ImGui::DragFloat4("Direction",
            glm::value_ptr(p_dir_light->direction)); ImGui::DragFloat4("View
            Pos", glm::value_ptr(p_dir_light->view_position));
                ImGui::DragFloat4("Color", glm::value_ptr(p_dir_light->color));
                ImGui::DragFloat4("Ambient",
            glm::value_ptr(p_dir_light->ambient)); ImGui::DragFloat4("Diffuse",
            glm::value_ptr(p_dir_light->diffuse)); ImGui::DragFloat4("Specular",
            glm::value_ptr(p_dir_light->specular));
            });
            */

            atlas::ui::draw_component<atlas::point_light>(
              "Point Light",
              m_selected_entity,
              [](atlas::point_light* p_dir_light) {
                  ImGui::DragFloat4(
                    "Color", glm::value_ptr(p_dir_light->color), 0.01);
                  ImGui::DragFloat(
                    "Attenuation", &p_dir_light->attenuation, 0.001);
                  ImGui::DragFloat4(
                    "Ambient", glm::value_ptr(p_dir_light->ambient), 0.01);
                  ImGui::DragFloat4(
                    "Diffuse", glm::value_ptr(p_dir_light->diffuse), 0.01);
                  ImGui::DragFloat4(
                    "Specular", glm::value_ptr(p_dir_light->specular), 0.01);
                  ImGui::DragFloat("Constant", &p_dir_light->constant, 0.01);
                  ImGui::DragFloat("Linear", &p_dir_light->linear, 0.01);
                  ImGui::DragFloat("Quadratic", &p_dir_light->quadratic, 0.01);
              });

            atlas::ui::draw_component<atlas::mesh_source>(
              "atlas::mesh_source",
              m_selected_entity,
              [](atlas::mesh_source* p_source) {
                  atlas::ui::draw_input_text(p_source->model_path);
                  atlas::ui::draw_vec4("Color", p_source->color);
              });

            atlas::ui::draw_component<atlas::material_metadata>(
              "material",
              m_selected_entity,
              [](atlas::material_metadata* p_source) {
                  float speed = 0.01f;
                  ImGui::DragFloat4(
                    "Ambient", glm::value_ptr(p_source->ambient), speed);
                  ImGui::DragFloat4(
                    "Diffuse", glm::value_ptr(p_source->diffuse), speed);
                  ImGui::DragFloat4(
                    "Specular", glm::value_ptr(p_source->specular), speed);
                  atlas::ui::draw_float("Shininess", p_source->shininess);
              });

            atlas::ui::draw_component<atlas::physics_body>(
              "Physics Body",
              m_selected_entity,
              [](atlas::physics_body* p_body) {
                  std::array<std::string, 3> items = {
                      "Static",
                      "Kinematic",
                      "Dynamic",
                  };
                  std::string combo_preview = items[p_body->body_movement_type];

                  // Begin the combo box
                  if (ImGui::BeginCombo("Body Type", combo_preview.data())) {
                      for (int n = 0; n < 3; n++) {
                          // Check if the current item is selected
                          const bool is_selected =
                            (p_body->body_movement_type == n);
                          if (ImGui::Selectable(items[n].data(), is_selected)) {
                              // Update the current type when a new item is
                              // selected
                              p_body->body_movement_type =
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

                  // physics body parameters
                  atlas::ui::draw_vec3("Linear Velocity",
                                       p_body->linear_velocity);
                  atlas::ui::draw_vec3("Angular Velocity",
                                       p_body->angular_velocity);
                  atlas::ui::draw_vec3("Force", p_body->force);
                  atlas::ui::draw_vec3("Impulse", p_body->impulse);
                  atlas::ui::draw_vec3("Torque", p_body->torque);
                  atlas::ui::draw_vec3("Center Mass",
                                       p_body->center_mass_position);
              });

            atlas::ui::draw_component<atlas::box_collider>(
              "Box Collider",
              m_selected_entity,
              [](atlas::box_collider* p_collider) {
                  atlas::ui::draw_vec3("Half Extent", p_collider->half_extent);
              });

            atlas::ui::draw_component<atlas::sphere_collider>(
              "Box Collider",
              m_selected_entity,
              [](atlas::sphere_collider* p_collider) {
                  atlas::ui::draw_float("Radius", p_collider->radius);
              });

            atlas::ui::draw_component<atlas::capsule_collider>(
              "Box Collider",
              m_selected_entity,
              [](atlas::capsule_collider* p_collider) {
                  atlas::ui::draw_float("Half Height", p_collider->half_height);
                  atlas::ui::draw_float("Radius", p_collider->radius);
              });

            atlas::ui::draw_component<atlas::tag::serialize>(
              "Serialize",
              m_selected_entity,
              [](atlas::tag::serialize* p_serialize) {
                  ImGui::Checkbox("Enable", &p_serialize->enable);
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

    m_editor_dockspace.end();
}

void
level_scene::start() {
    m_deserializer_test = atlas::serializer();

    if (!m_deserializer_test.load("LevelScene", *this)) {
        console_log_error("Could not load yaml file LevelScene!!!");
    }

    // testing the flip parameter inside atlas::mesh_source
    m_viking_room->set<atlas::mesh_source>({
      .flip = true,
      .color = { 1.f, 1.f, 1.f, 1.f },
      .model_path = "assets/models/viking_room.obj",
      .diffuse = "assets/models/viking_room.png",
      // .model_path = "assets/models/Ball OBJ.obj",
      // .diffuse = "assets/models/clear.png",
    });

    // TODO: Make this contain an atlas::directional_light
    // If the scene opject dooes not have a atlas::directional_light, then we
    // set the default values to 1.0f m_robot_model sets the cube.obj 3d model
    // and loads it
    m_robot_model->set<atlas::material_metadata>({
      .shininess = 64.f,
    });

    // Initiating physics system
    atlas::physics::jolt_settings settings = {};
    flecs::world registry = *this;
    m_physics_engine_handler =
      atlas::physics::physics_engine(settings, registry, *event_handle());

    // Note -- just added for temporary
    // ImGuiIO io = ImGui::GetIO();
    // m_font = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf",
    // 120.0f);
}

void
level_scene::on_update() {

    auto query_cameras =
      query_builder<atlas::perspective_camera, atlas::transform>().build();

    query_cameras.each([this](atlas::perspective_camera& p_camera,
                              atlas::transform& p_transform) {
        if (!p_camera.is_active) {
            return;
        }

        float dt = atlas::application::delta_time();
        float default_speed = 10.f; // current default movement speed that does
                                    // not applied modified speed
        float rotation_speed = 1.f;
        float velocity = default_speed * dt;
        if (atlas::event::is_mouse_pressed(mouse_button_middle)) {
            velocity = m_movement_speed * dt;
        }
        float rotation_velocity = rotation_speed * dt;

        glm::quat to_quaternion = atlas::to_quat(p_transform.quaternion);

        glm::vec3 up = glm::rotate(to_quaternion, atlas::math::up());
        glm::vec3 forward = glm::rotate(to_quaternion, atlas::math::backward());
        glm::vec3 right = glm::rotate(to_quaternion, atlas::math::right());

        if (atlas::event::is_key_pressed(key_left_shift)) {
            p_transform.position += up * velocity;
        }

        if (atlas::event::is_key_pressed(key_space)) {
            p_transform.position -= up * velocity;
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
    float dt = atlas::application::delta_time();
    if (atlas::event::is_key_pressed(key_r) and !m_physics_is_runtime) {
        runtime_start();
    }

    atlas::physics_body* sphere_body =
      m_viking_room->get_mut<atlas::physics_body>();
    // U = +up
    // J = -up
    // H = +left
    // L = -Left
    if (atlas::event::is_key_pressed(key_space)) {
        glm::vec3 linear_velocity = { 0.f, 10.0f, 0.f };
        sphere_body->linear_velocity = linear_velocity;
        sphere_body->impulse = linear_velocity;
    }

    if (atlas::event::is_key_pressed(key_j)) {
        glm::vec3 angular_vel = { -10.f, 0.f, 0.f };
        sphere_body->angular_velocity = angular_vel;
    }

    if (atlas::event::is_key_pressed(key_h)) {
        glm::vec3 angular_vel = { 10.f, 0.f, 0.f };
        sphere_body->angular_velocity = angular_vel;
    }

    if (atlas::event::is_key_pressed(key_l)) {
        glm::vec3 angular_vel = { -0.1f, 0.f, 0.f };
        sphere_body->angular_velocity = angular_vel;
    }

    if (m_physics_is_runtime) {
        m_physics_engine_handler.update(dt);
    }

    if (atlas::event::is_key_pressed(key_l) and m_physics_is_runtime) {
        runtime_stop();
    }
}
