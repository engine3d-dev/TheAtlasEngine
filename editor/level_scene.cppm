module;

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <flecs.h>
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <format>
#include <filesystem>
#include <unordered_map>

export module level_scene;

import atlas.core.utilities;
import atlas.application;
import atlas.core.scene;
import atlas.core.scene.game_object;
import atlas.core.event;
import atlas.core.scene.components;
import atlas.core.utilities.state;
import atlas.core.math;
import atlas.core.editor.dockspace;
import atlas.application;
import atlas.core.ui.widgets;
import atlas.core.editor.menu_item;
import atlas.core.serialize;
import atlas.physics.engine;
import atlas.drivers.vulkan.imgui_context;
import atlas.drivers.vulkan.instance_context;
import atlas.core.ui.widgets.imgui_stdlib;
import vk;
import content_browser;

static void
ui_component_list(flecs::entity& p_selected_entity) {
    std::string entity_name = p_selected_entity.name().c_str();
    std::string new_entity_name = "";
    atlas::ui::draw_input_text(new_entity_name, entity_name);

    p_selected_entity.set_name(new_entity_name.c_str());

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

        if (!p_selected_entity.has<atlas::material_metadata>()) {
            if (ImGui::MenuItem("Material")) {
                p_selected_entity.add<atlas::material_metadata>();
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

enum scene_runtime { edit, play };

namespace ui::experimental {
    /**
     * @brief This is an experimental feature for setting up an image
     * thumbnail-like abstraction
     * TODO: This should be considered to being abstracted in another approach
     * as this is a temporary solution for it. OR this could be the way we
     * handle icons for the time being for simplicity.
     */
    class icon {
    public:
        icon() = default;
        icon(const std::filesystem::path& p_filename) {
            vk::texture_info config_texture = {
                .phsyical_memory_properties =
                  atlas::vulkan::instance_context::physical_driver()
                    .memory_properties(),
                .filepath = p_filename
            };
            m_icon_image =
              vk::texture(atlas::vulkan::instance_context::logical_device(),
                          config_texture);
            if (!m_icon_image.loaded()) {
                console_log_info("Play Button Could not be loaded!!");
            }
            m_icon_image_id =
              static_cast<ImTextureID>(ImGui_ImplVulkan_AddTexture(
                m_icon_image.image().sampler(),
                m_icon_image.image().image_view(),
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
        }

        icon(const vk::image_extent& p_extent) {
            // vk::texture_info config_texture = {
            //     .phsyical_memory_properties =
            //     atlas::vulkan::instance_context::physical_driver().memory_properties(),
            //     .filepath = p_filename
            // };
            m_icon_image =
              vk::texture(atlas::vulkan::instance_context::logical_device(),
                          p_extent,
                          atlas::vulkan::instance_context::physical_driver()
                            .memory_properties());
            // if (!m_icon_image.loaded()) {
            //     console_log_info("Play Button Could not be loaded!!");
            // }
            m_icon_image_id =
              static_cast<ImTextureID>(ImGui_ImplVulkan_AddTexture(
                m_icon_image.image().sampler(),
                m_icon_image.image().image_view(),
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
        }

        ~icon() {
            // implicitly destroy
            // destroy();
        }

        [[nodiscard]] uint32_t width() const { return m_icon_image.width(); }

        [[nodiscard]] uint32_t height() const { return m_icon_image.height(); }

        [[nodiscard]] ImTextureID texture_id() const { return m_icon_image_id; }

        void destroy() { m_icon_image.destroy(); }

    private:
        vk::texture m_icon_image;
        ImTextureID m_icon_image_id;
    };
};

// TODO: This is just a temporary solution for loading material texture icons to
// ImGui::Image This should be replaced with something like
// atlas::material_manager, eventually.
struct material {
    ui::experimental::icon specular;
    ui::experimental::icon diffuse;
};

export class level_scene final : public atlas::scene {
public:
    level_scene(const std::string& p_name, atlas::event::bus& p_bus)
      : atlas::scene(p_name, p_bus) {
        auto editor_camera = entity("Editor Camera");
        editor_camera
          .add<flecs::pair<atlas::tag::editor, atlas::projection_view>>();
        editor_camera.set<atlas::transform>({
          .position = { 3.50f, 4.90f, 36.40f },
          .scale{ 1.f },
        });
        editor_camera.set<atlas::perspective_camera>({
          .plane = { 0.1f, 5000.f },
          .is_active = true,
          .field_of_view = 45.f,
        });

        atlas::game_object bob_object = entity("Bob");
        bob_object.add<atlas::point_light>();

        // @brief For now adding this in
        // because we do not have a way to handle empty scenes
        // so we are adding this in just to have a specific mesh associated with
        // the renderer for the time being, just to make sure it works.
        atlas::game_object viking_room = entity("Viking Room");
        viking_room.add<atlas::tag::serialize>();
        viking_room.set<atlas::transform>({
          .position = { -2.70f, 2.70, -8.30f },
          .rotation = { 2.30f, 95.90f, 91.80f },
          .scale{ 1.f },
        });

        viking_room.set<atlas::sphere_collider>({
          .radius = 1.0f,
        });

        viking_room.set<atlas::physics_body>({
          .friction = 15.f,
          .restitution = 0.3f,
          .body_movement_type = atlas::dynamic,
        });

        atlas::game_object cube = entity("Aircraft");

        cube.set<atlas::transform>({
          .position = { 0.f, 2.10f, -7.30f },
          .scale = { 0.9f, 0.9f, 0.9f },
        });

        cube.set<atlas::mesh_source>({
          .color = { 1.f, 1.f, 1.f, 1.f },
          // .model_path = "assets/models/E 45 Aircraft_obj.obj",
          .model_path = "assets/backpack/backpack.obj",
          .diffuse = "assets/backpack/diffuse.jpg",
          .specular = "assets/backpack/specular.jpg"
          //   .diffuse = "assets/models/E-45-steel detail_2_col.jpg",
        });

        atlas::game_object robot_model = entity("Cube");
        robot_model.add<atlas::tag::serialize>();
        // robot_model.add<atlas::tag::serialize>();
        robot_model.set<atlas::transform>({
          .position = { -2.70, 3.50f, 4.10f },
          .scale = { 1.f, 1.f, 1.f },
        });

        robot_model.set<atlas::mesh_source>(
          { .color = { 1.f, 1.f, 1.f, 1.f },
            .model_path = "assets/models/cube.obj",
            .diffuse = "assets/models/container_diffuse.png",
            .specular = "assets/models/container_specular.png" });

        robot_model.set<atlas::box_collider>({
          .half_extent = { 1.f, 1.f, 1.f },
        });
        robot_model.set<atlas::physics_body>({
          //   .restitution = 1.f,
          .body_movement_type = atlas::dynamic,
        });

        atlas::game_object platform = entity("Platform");

        platform.set<atlas::transform>({
          .scale = { 15.f, 0.30f, 10.0f },
        });
        platform.set<atlas::mesh_source>({
          .model_path = "assets/models/cube.obj",
          .diffuse = "assets/models/wood.png",
        });
        platform.set<atlas::physics_body>({
          .body_movement_type = atlas::fixed,
        });
        platform.set<atlas::box_collider>({
          .half_extent = { 15.f, 0.30f, 10.0f },
        });

        atlas::game_object point_light = entity("Point Light 1");
        point_light.set<atlas::transform>({
          .position = { 0.f, 2.10f, -7.30f },
          .scale = { 0.9f, 0.9f, 0.9f },
        });
        point_light.add<atlas::tag::serialize>();

        // for(size_t i = 0; i < 26; i++) {
        // 	auto obj = entity(std::format("Object #{}", i));
        // 	obj.set<atlas::physics_body>({
        // 		.restitution = 1.25f,
        // 		.body_movement_type = atlas::dynamic,
        // 	});

        // 	obj.set<atlas::sphere_collider>(
        // 		{
        // 		.radius = 1.0f,
        // 	});

        // 	glm::vec3 pos = {float(0*1.4),float(0 * 1.4),float(0 * -3) };

        // 	obj.set<atlas::transform>({
        // 		.position = pos,
        // 		.rotation = {.3f, 0.0f, 0.0f},
        // 	});

        // 	obj.set<atlas::mesh_source>({
        // 		.model_path = "assets/models/Ball OBJ.obj",
        // 		.diffuse = "assets/models/clear.png",
        // 	});
        // }

        m_editor_dockspace.fullscreen(false);
        m_editor_dockspace.dockspace_open(true);

        atlas::game_object gerald = entity("Gerald");
        gerald.add<atlas::point_light>();

        // TODO: Move this outside of level_scene
        m_deserializer_test = atlas::serializer();

        subscribe<atlas::event::collision_enter>(this,
                                                 &level_scene::collision_enter);

        atlas::register_start(this, &level_scene::start);
        atlas::register_physics(this, &level_scene::physics_update);
        atlas::register_update(this, &level_scene::on_update);
        atlas::register_ui(this, &level_scene::on_ui_update);

        // Just loading a default icon that has a white texture
        // TODO: This should be considered handled differently.
        vk::image_extent extent = {
            .width = 1,
            .height = 1,
        };
        m_default_material_icon.specular = ui::experimental::icon(extent);
        m_default_material_icon.diffuse = ui::experimental::icon(extent);

        // TEMP: This is for testing if the triggered event only occurs once
        // whenever a signal occurs. This should be moved to the internal
        // application... OR the renderer
        // trigger<atlas::event::mesh_reload>(this, &level_scene::reload_mesh);

        // @note checking to see what state we are in. (If playing/stopping)
        // Ref<Texture2D> icon = _sceneState == SceneState::Edit ? _iconPlay :
        // _iconStop;
        vk::texture_info config_texture = {
            .phsyical_memory_properties =
              atlas::vulkan::instance_context::physical_driver()
                .memory_properties(),
            .filepath = std::filesystem::path("assets/icons/PlayButton.png")
        };
        m_play_button = vk::texture(
          atlas::vulkan::instance_context::logical_device(), config_texture);
        if (!m_play_button.loaded()) {
            console_log_info("Play Button Could not be loaded!!");
        }
        config_texture = { .phsyical_memory_properties =
                             atlas::vulkan::instance_context::physical_driver()
                               .memory_properties(),
                           .filepath = std::filesystem::path(
                             "assets/icons/StopButton.png") };
        m_stop_button = vk::texture(
          atlas::vulkan::instance_context::logical_device(), config_texture);
        if (!m_stop_button.loaded()) {
            console_log_info("Stop Button Could not be loaded!!");
        }

        m_play_button_id = static_cast<ImTextureID>(ImGui_ImplVulkan_AddTexture(
          m_play_button.image().sampler(),
          m_play_button.image().image_view(),
          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
        m_stop_button_id = static_cast<ImTextureID>(ImGui_ImplVulkan_AddTexture(
          m_stop_button.image().sampler(),
          m_stop_button.image().image_view(),
          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));

        atlas::vulkan::instance_context::submit_resource_free([this]() {
            m_play_button.destroy();
            m_stop_button.destroy();
            for (auto& [key, value] : m_material_icons) {
                value.specular.destroy();
                value.diffuse.destroy();
            }

            m_default_material_icon.specular.destroy();
            m_default_material_icon.diffuse.destroy();

            m_content_browser.destroy();
        });
    }

    ~level_scene() override = default;

    void start() {
        m_deserializer_test = atlas::serializer();

        if (!m_deserializer_test.load("LevelScene", *this)) {
            console_log_error("Could not load yaml file LevelScene!!!");
        }

        flecs::world registry = *this;

        // TODO: Should consider having this be a custom UI property to the editor.
        auto viking_room = entity("Viking Room");
        atlas::mesh_source* src = viking_room.get_mut<atlas::mesh_source>();
        src->flip = true;
        m_physics_engine = atlas::physics::engine(registry, *event_handle());
    }

    void on_update(float p_delta_time) {
        auto query_cameras =
          query_builder<atlas::perspective_camera, atlas::transform>().build();
        float dt = p_delta_time;

        query_cameras.each([this, dt](atlas::perspective_camera& p_camera,
                                      atlas::transform& p_transform) {
            if (!p_camera.is_active) {
                return;
            }

            float default_speed = 10.f; // current default movement speed that
                                        // does not applied modified speed
            float rotation_speed = 1.f;
            float velocity = default_speed * dt;
            if (atlas::event::is_mouse_pressed(mouse_button_middle)) {
                velocity = m_movement_speed * dt;
            }
            float rotation_velocity = rotation_speed * dt;

            glm::quat to_quaternion = atlas::to_quat(p_transform.quaternion);

            glm::vec3 up = glm::rotate(to_quaternion, atlas::math::up());
            glm::vec3 forward =
              glm::rotate(to_quaternion, atlas::math::backward());
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

    void on_ui_update() {
        // setting up the dockspace UI widgets at the window toolbar
        if (m_editor_dockspace.begin()) {
            try {
                m_editor_menu.begin();
            }
            catch (const atlas::ui::menu_bar_exception& e) {
            }

            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Save")) {
                    // m_deserializer_test.save("LevelScene");
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Exit")) {
                    // glfwSetWindowShouldClose(atlas::application::close(),
                    // true);
                }

                ImGui::EndMenu();
            }

            m_editor_menu.end();
        }
        ImGuiID dockspace_id = ImGui::GetID("Dockspace Demo");
        ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Viewport")) {
            ImVec2 viewport_size = ImGui::GetContentRegionAvail();
            // ImGui::Image(m_viewport_image_id,
            // {static_cast<float>(m_extent.width),
            // static_cast<float>(m_extent.height)});
            if (atlas::vulkan::g_viewport_image_id == nullptr) {
                console_log_error("atlas::vulkan::g_viewport_image_id is "
                                  "nullptr!!!!!!!!!!!!!!!!!!!");
            }
            ImGui::Image(atlas::vulkan::g_viewport_image_id, viewport_size);
            ImGui::End();
        }

        // if (ImGui::Begin("Viewport")) {
        //     // TODO: Consider doing this a different way, but not with
        //     static.
        //     // glm::vec2 viewport_panel_size =
        //     // glm::vec2{ atlas::application::params().width,
        //     //             atlas::application::params().height };

        //     ImGui::End();
        // }

        defer_begin();
        auto query_builder = this->query_builder<atlas::transform>().build();

        if (ImGui::Begin("Scene Heirarchy")) {
            // @note right click on blank space
            // @param string_id
            // @param popup_flags - will be the mouse flag (0=right, 1=left)
            if (atlas::ui::begin_popup_context_window(nullptr, 1, false)) {
                if (ImGui::MenuItem("Create Empty Entity")) {
                    m_current_entity = entity("Empty Entity");
                }
                ImGui::EndPopup();
            }

            query_builder.each([&](flecs::entity p_entity, atlas::transform&) {
                // We set the imgui flags for our scene heirarchy panel
                // TODO -- Make the scene heirarchy panel a separate class that
                // is used for specify the layout and other UI elements here
                ImGuiTreeNodeFlags flags =
                  ((m_selected_entity == p_entity) ? ImGuiTreeNodeFlags_Selected
                                                   : 0) |
                  ImGuiTreeNodeFlags_OpenOnArrow;
                flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
                flags |= ImGuiWindowFlags_Popup;
                flags |= ImGuiTreeNodeFlags_AllowItemOverlap;

                bool opened = ImGui::TreeNodeEx(p_entity.name().c_str(), flags);

                if (ImGui::IsItemClicked()) {
                    m_selected_entity = p_entity;
                }

                bool delete_entity = false;
                if (ImGui::BeginPopupContextItem()) {
                    if (ImGui::MenuItem("Delete Entity")) {
                        delete_entity = true;
                    }
                    ImGui::EndPopup();
                }

                if (delete_entity) {
                    m_selected_entity.destruct();
                }

                ImGui::SameLine();
                ImGui::TextDisabled("(%llu)", p_entity.id());

                if (opened) {
                    flags = ImGuiTreeNodeFlags_OpenOnArrow |
                            ImGuiTreeNodeFlags_SpanAvailWidth;
                    auto query_children_builder =
                      this->query_builder()
                        .with(flecs::ChildOf, p_entity)
                        .build();
                    int32_t child_count = query_children_builder.count();

                    // // Only show children in scene heirarchy panel if there
                    // are children entities
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

                atlas::ui::draw_component<atlas::mesh_source>(
                  "atlas::mesh_source",
                  m_selected_entity,
                  [this](atlas::mesh_source* p_source) {
                      if (ImGui::InputText(
                            "Input Label",
                            &p_source->model_path,
                            ImGuiInputTextFlags_EnterReturnsTrue)) {
                          console_log_info("mesh_src = {}",
                                           p_source->model_path);
                          atlas::event::mesh_reload reload_request = {
                              .entity_id = m_selected_entity.id(),
                              .filename = p_source->model_path,
                          };

                          if (std::filesystem::exists(p_source->model_path)) {
                              signal(reload_request);
                          }
                      }
                      atlas::ui::draw_vec4("Color", p_source->color);

                      if (ImGui::Button("Reload Material")) {
                          atlas::event::material_reload
                            reload_material_request = {
                                .entity_id = m_selected_entity.id(),
                                .diffuse = "assets/models/viking_room.png",
                                .specular = "",
                            };

                          signal(reload_material_request);
                      }
                  });

                atlas::ui::draw_component<atlas::material_metadata>(
                  "material",
                  m_selected_entity,
                  [this](atlas::material_metadata* p_source) {
                      // We only want to load in our material textures If
                      // requested
                      // TODO: Move this log outside from the add component
                      // logic. This should be automatically done rather then
                      // explictly requested
                      if (!m_material_icons.contains(m_selected_entity.id())) {
                          if (m_selected_entity.has<atlas::mesh_source>()) {
                              material new_mat = {};
                              const auto* src =
                                m_selected_entity.get<atlas::mesh_source>();
                              console_log_info("Specular Path: {}",
                                               src->specular);
                              if (std::filesystem::exists(src->specular)) {
                                  new_mat.specular = ui::experimental::icon(
                                    std::filesystem::path(src->specular));
                              }
                              else {
                                  console_log_info("Specular (white) Path");
                                  vk::image_extent extent = { .width = 1,
                                                              .height = 1 };
                                  new_mat.specular =
                                    ui::experimental::icon(extent);
                              }

                              if (std::filesystem::exists(src->diffuse)) {
                                  console_log_info("Diffuse Path: {}",
                                                   src->diffuse);
                                  new_mat.diffuse = ui::experimental::icon(
                                    std::filesystem::path(src->diffuse));
                              }
                              else {
                                  console_log_info("Diffuse (white) Path");
                                  vk::image_extent extent = { .width = 1,
                                                              .height = 1 };
                                  new_mat.diffuse =
                                    ui::experimental::icon(extent);
                              }

                              m_material_icons.emplace(m_selected_entity.id(),
                                                       new_mat);
                          }
                      }

                      float speed = 0.01f;
                      ImGui::DragFloat4(
                        "Ambient", glm::value_ptr(p_source->ambient), speed);
                      ImGui::DragFloat4(
                        "Diffuse", glm::value_ptr(p_source->diffuse), speed);
                      ImGui::DragFloat4(
                        "Specular", glm::value_ptr(p_source->specular), speed);
                      atlas::ui::draw_float("Shininess", p_source->shininess);
                  });

                /*
                atlas::ui::draw_component<atlas::directional_light>("Directional
                Light", m_selected_entity, [](atlas::directional_light*
                p_dir_light){ ImGui::DragFloat4("Direction",
                glm::value_ptr(p_dir_light->direction)); ImGui::DragFloat4("View
                Pos", glm::value_ptr(p_dir_light->view_position));
                    ImGui::DragFloat4("Color",
                glm::value_ptr(p_dir_light->color));
                    ImGui::DragFloat4("Ambient",
                glm::value_ptr(p_dir_light->ambient));
                ImGui::DragFloat4("Diffuse",
                glm::value_ptr(p_dir_light->diffuse));
                ImGui::DragFloat4("Specular",
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
                      ImGui::DragFloat4("Specular",
                                        glm::value_ptr(p_dir_light->specular),
                                        0.01);
                      ImGui::DragFloat(
                        "Constant", &p_dir_light->constant, 0.01);
                      ImGui::DragFloat("Linear", &p_dir_light->linear, 0.01);
                      ImGui::DragFloat(
                        "Quadratic", &p_dir_light->quadratic, 0.01);
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
                      std::string combo_preview =
                        items[p_body->body_movement_type];

                      // Begin the combo box
                      if (ImGui::BeginCombo("Body Type",
                                            combo_preview.data())) {
                          for (int n = 0; n < 3; n++) {
                              // Check if the current item is selected
                              const bool is_selected =
                                (p_body->body_movement_type == n);
                              if (ImGui::Selectable(items[n].data(),
                                                    is_selected)) {
                                  // Update the current type when a new item is
                                  // selected
                                  p_body->body_movement_type =
                                    static_cast<atlas::body_type>(n);
                              }

                              // Set the initial focus when the combo box is
                              // first opened
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
                      atlas::ui::draw_vec3("Half Extent",
                                           p_collider->half_extent);
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
                      atlas::ui::draw_float("Half Height",
                                            p_collider->half_height);
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

            // Material Properties Panel
            // TODO: Make this an abstraction to map out the materials
            if (ImGui::Begin("Material Editor")) {

                // Specular
                if (ImGui::BeginChild("Specular", ImVec2(150, 200), true)) {
                    if (m_material_icons.contains(m_selected_entity.id())) {
                        ImGui::Text("Specular");
                        ImGui::Separator();
                        const auto specular_icon =
                          m_material_icons[m_selected_entity.id()].specular;
                        ImGui::Text("size = %d x %d",
                                    specular_icon.width(),
                                    specular_icon.height());
                        ImGui::Image(specular_icon.texture_id(),
                                     ImVec2(128, 128));
                    }
                    else {
                        const auto specular_icon =
                          m_default_material_icon.specular;
                        ImGui::Text("size = %d x %d",
                                    specular_icon.width(),
                                    specular_icon.height());
                        ImGui::Image(specular_icon.texture_id(),
                                     ImVec2(128, 128));
                    }
                    ImGui::EndChild();

                    // Diffuse
                    ImGui::Spacing();
                    if (ImGui::BeginChild("Diffuse", ImVec2(150, 200), true)) {
                        ImGui::Text("Diffuse");
                        ImGui::Separator();
                        if (m_material_icons.contains(m_selected_entity.id())) {
                            const auto diffuse_icon =
                              m_material_icons[m_selected_entity.id()].diffuse;
                            ImGui::Text("size = %d x %d",
                                        diffuse_icon.width(),
                                        diffuse_icon.height());
                            ImGui::Image(diffuse_icon.texture_id(),
                                         ImVec2(128, 128));
                        }
                        else {
                            const auto diffuse_icon =
                              m_default_material_icon.diffuse;
                            ImGui::Text("size = %d x %d",
                                        diffuse_icon.width(),
                                        diffuse_icon.height());
                            ImGui::Image(diffuse_icon.texture_id(),
                                         ImVec2(128, 128));
                        }
                        ImGui::EndChild();
                    }
                }

                ImGui::End();
            }

            // Note --- just added this temporarily for testing
            // auto time = atlas::application::delta_time();

            // if((int)(time * 10.0f) % 8 > 4) {
            // 	m_blink = !m_blink;
            // }

            // auto width = atlas::application::get_window().width();
            // auto height = atlas::application::get_window().height();

            // ImGui::SetNextWindowPos(ImVec2(static_cast<float>(width) * 0.5f,
            // static_cast<float>(height) * 0.5f), ImGuiCond_Always,
            // ImVec2(0.5f, 0.5f)); ImGui::SetNextWindowSize(ImVec2(200, 20),
            // ImGuiCond_Always); ImGuiWindowFlags flags =
            // ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground |
            // ImGuiWindowFlags_NoInputs; ImGui::SetNextWindowBgAlpha(0.f);

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

        ui_toolbar();

        m_content_browser.run();

        m_editor_dockspace.end();
    }

    void ui_toolbar() {
        ImGui::PushStyleVar(
          ImGuiStyleVar_WindowPadding,
          ImVec2(0, 2)); // @note ImVec making button not touch bottom
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 2));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        auto& color = ImGui::GetStyle().Colors;
        auto& buttonHovered = color[ImGuiCol_ButtonHovered];
        auto& buttonActive = color[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(
          ImGuiCol_ButtonHovered,
          ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
        ImGui::PushStyleColor(
          ImGuiCol_ButtonHovered,
          ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

        float button_size = 20.0f;
        ImGui::Begin("##toolbox");

        ImTextureID button_id = (m_scene_state == scene_runtime::edit)
                                  ? m_play_button_id
                                  : m_stop_button_id;

        // @note GetWindowContentRegionMax().x is how much space is there for
        // content (widgets)
        // @note 0.5f is the offset for padding.
        // @note takes button size and halves it and makes the offset the center
        // of that tab. (centering  buttons)
        ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) -
                        (button_size * 0.5f));

        if (ImGui::ImageButton("##Button",
                               button_id,
                               ImVec2{ button_size, button_size },
                               ImVec2(0, 0),
                               ImVec2(1, 1))) {
            if (m_scene_state == scene_runtime::edit) {
                m_scene_state = scene_runtime::play;
                m_physics_engine.start();
            }
            else if (m_scene_state == scene_runtime::play) {
                m_scene_state = scene_runtime::edit;
                m_physics_engine.stop();
                reset_objects();
            }
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
        ImGui::End();
    }

    void physics_update() {
        // TODO: Replace delta_time with physics fixed-timestep instead
        float dt = atlas::application::delta_time();
        auto viking_room = entity("Viking Room");

        atlas::physics_body* sphere_body =
          viking_room.get_mut<atlas::physics_body>();
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

        if (m_scene_state == scene_runtime::play) {
            m_physics_engine.update(dt);
        }
    }

    void reset_objects() {
        if (!m_deserializer_test.load("LevelScene", *this)) {
            console_log_error("Could not load yaml file LevelScene!!!");
        }
    }

private:
    void collision_enter(atlas::event::collision_enter& p_event) {
        console_log_warn("collision_enter event!!!");
        atlas::game_object e1 = entity(p_event.entity1);
        atlas::game_object e2 = entity(p_event.entity2);

        console_log_warn("Entity1 = {}", e1.name().c_str());
        console_log_warn("Entity2 = {}", e2.name().c_str());
    }

    void collision_persisted(atlas::event::collision_persisted& p_event) {
        console_log_warn("collision_persisted(p_event) invoked!!");
        atlas::game_object e1 = entity(p_event.entity1);
        atlas::game_object e2 = entity(p_event.entity2);

        console_log_warn("Entity1 = {}", e1.name().c_str());
        console_log_warn("Entity2 = {}", e2.name().c_str());
    }

private:
    atlas::serializer m_deserializer_test;
    flecs::entity m_selected_entity;

    atlas::game_object_optional m_current_entity;
    float m_movement_speed = 10.f;

    atlas::physics::engine m_physics_engine;

    atlas::ui::dockspace m_editor_dockspace;
    atlas::ui::menu_item m_editor_menu;
    // ui::experimental::icon m_default_icon;
    material m_default_material_icon;

    scene_runtime m_scene_state = scene_runtime::edit;
    vk::texture m_play_button;
    vk::texture m_stop_button;
    ImTextureID m_play_button_id;
    ImTextureID m_stop_button_id;

    content_browser_panel m_content_browser;

    std::unordered_map<uint64_t, material> m_material_icons;

    // Note -- Added this temporarily
    // ImFont* m_font;
};