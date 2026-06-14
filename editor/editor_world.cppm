module;

#include <string>
#include <print>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <flecs.h>
#include <filesystem>

export module editor:world;

import atlas.core.utilities;
import atlas.core.scene;
import atlas.core.scene.world;
import atlas.core.event;
import atlas.core.level_streamer;
import atlas.core.ui;
import atlas.physics.engine;

import :level_scene;
import :icon;
import :utilities;
import :content_browser;
// import :level_scene2;

import atlas.core.editor;
import atlas.core.serialize;
import atlas.physics.engine;
import vk;

enum scene_runtime : uint8_t { edit, play };

/**
 * @brief editor_world is where a lot of the editor logic will be handled
 */
export class editor_world final : public atlas::world {
public:
    editor_world(/*NOLINT*/ std::shared_ptr<atlas::graphics_context> p_context,
                 const std::string& p_tag,
                 atlas::event::bus& p_bus,
                 atlas::level_streamer& p_level_streamer)
      : atlas::world(p_tag, p_level_streamer)
      , m_bus(&p_bus) {
        m_device = p_context->logical_device();
        // Create defualt scene to level streamer
        // Does polymorphic allocations for these customized scenes
        default_custom_scene<level_scene>("LevelScene", p_bus);

        // Experimental for loading a second scene.
        // create_scene<level_scene2>("Level Scene 2", p_bus);

        // Set what our current scene is
        // TODO: Probably have `default_custom_scene<UScene>() set this
        current("LevelScene");

        m_current_scene = this->current();

        m_editor_dockspace.fullscreen(false);
        m_editor_dockspace.dockspace_open(true);

        m_host_bit = p_context->physical_device().memory_properties(
          vk::memory_property::host_visible_bit |
          vk::memory_property::host_coherent_bit);
        // Initializing icon.
        m_play_icon = ui::experimental::icon(
          m_device, m_host_bit, "assets/icons/PlayButton.png");

        if (!m_play_icon.loaded()) {
            std::println("m_play_icon not loaded!");
        }

        m_stop_icon =
          ui::experimental::icon(m_device,
                                 p_context->physical_device().memory_properties(
                                   vk::memory_property::host_visible_bit |
                                   vk::memory_property::host_coherent_bit),
                                 "assets/icons/StopButton.png");

        if (!m_stop_icon.loaded()) {
            std::println("m_stop_icon not loaded!");
        }

        m_content_browser = content_browser_panel(m_device, m_host_bit);

        atlas::register_start(this, &editor_world::preload_assets);
        atlas::register_update(this, &editor_world::update);
        atlas::register_physics(this, &editor_world::physics_update);
        atlas::register_ui(this, &editor_world::ui_update);
        atlas::register_post(this, &editor_world::unload_assets);
    }

    ~editor_world() override = default;

    void preload_assets() {
        m_deserializer_test = atlas::serializer();

        if (!m_deserializer_test.load("LevelScene", *m_current_scene)) {
            console_log_error("Could not load yaml file LevelScene!!!");
        }

        m_physics_engine = atlas::physics::engine(*m_current_scene, *m_bus);
    }

    void update(float p_delta_time) { m_delta_time = p_delta_time; }

    void ui_update() {
        // setting up the dockspace UI widgets at the window toolbar
        if (m_editor_dockspace.begin()) {
            m_editor_menu.begin();

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

            ImGuiID dockspace_id = ImGui::GetID("Dockspace Demo");
            ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
            if (ImGui::Begin("Viewport")) {
                ImVec2 viewport_size = ImGui::GetContentRegionAvail();
                // ImGui::Image(m_viewport_image_id,
                // {static_cast<float>(m_extent.width),
                // static_cast<float>(m_extent.height)});
                if (atlas::g_viewport_image_id == nullptr) {
                    console_log_error("atlas::vulkan::g_viewport_image_id is "
                                      "nullptr!!!!!!!!!!!!!!!!!!!");
                }
                ImGui::Image(atlas::g_viewport_image_id, viewport_size);
                ImGui::End();
            }

            scene_heirarchy_panel();

            properties_panel();

            materials_editor_panel();

            m_content_browser.run();

            ui_toolbar();

            m_editor_dockspace.end();
        }
    }

    void scene_heirarchy_panel() {
        if (ImGui::Begin("Scene Heirarchy")) {
            m_current_scene->defer_begin();

            auto query_all_transforms =
              m_current_scene->query_builder<atlas::transform>().build();

            query_all_transforms.each([&](flecs::entity p_entity,
                                          atlas::transform&) {
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
                      m_current_scene->query_builder()
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

            m_current_scene->defer_end();

            ImGui::End();
        }
    }

    void properties_panel() {
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
                  [](atlas::mesh_source* p_source) {
                      //   if (ImGui::InputText(
                      //         "Input Label",
                      //         &p_source->model_path,
                      //         ImGuiInputTextFlags_EnterReturnsTrue)) {
                      //       console_log_info("mesh_src = {}",
                      //                        p_source->model_path);
                      //     //   atlas::event::mesh_reload reload_request = {
                      //     //       .entity_id = m_selected_entity.id(),
                      //     //       .filename = p_source->model_path,
                      //     //   };

                      //     //   if
                      //     (std::filesystem::exists(p_source->model_path)) {
                      //     //       signal(reload_request);
                      //     //   }
                      //   }
                      atlas::ui::draw_vec4("Color", p_source->color);

                      //   if (ImGui::Button("Reload Material")) {
                      //       atlas::event::material_reload
                      //         // reload_material_request = {
                      //         //     .entity_id = m_selected_entity.id(),
                      //         //     .diffuse =
                      //         "assets/models/viking_room.png",
                      //         //     .specular = "",
                      //         // };

                      //     //   signal(reload_material_request);
                      //   }
                  });

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

                      // Begin the combo box
                      if (ImGui::BeginCombo(
                            "Body Type",
                            items[p_body->body_movement_type].data())) {
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
        }
    }

    void materials_editor_panel() {
        if (ImGui::Begin("Material Editor")) {
            ImGui::End();
        }
    }

    void ui_toolbar() {
        ImGui::PushStyleVar(
          ImGuiStyleVar_WindowPadding,
          ImVec2(0, 2)); // @note ImVec making button not touch bottom
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 2));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        auto& color = ImGui::GetStyle().Colors;
        auto& button_hovered = color[ImGuiCol_ButtonHovered];
        auto& button_active = color[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(
          ImGuiCol_ButtonHovered,
          ImVec4(button_hovered.x, button_hovered.y, button_hovered.z, 0.5f));
        ImGui::PushStyleColor(
          ImGuiCol_ButtonHovered,
          ImVec4(button_active.x, button_active.y, button_active.z, 0.5f));

        float button_size = 20.0f;

        if (ImGui::Begin("##toolbox")) {
            VkDescriptorSet button_id = (m_scene_state == scene_runtime::edit)
                                          ? m_play_icon.texture_id()
                                          : m_stop_icon.texture_id();

            /**
             * @note GetWindowContentRegionMax().x is how much space is there
             * for content (widgets)
             * @note 0.5f is the offset for padding.
             * @note takes button size and halves it and makes the offset the
             * center of that tab. (centering  buttons)
             */
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

                    if (!m_deserializer_test.load("LevelScene", *m_current_scene)) {
                        console_log_error("Could not load yaml file LevelScene!!!");
                    }
                }
            }

            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(3);
            ImGui::End();
        }
    }

    void physics_update() {
        if (m_scene_state == scene_runtime::play) {
            m_physics_engine.update(m_delta_time);
        }
    }

    void unload_assets() {
        // console_log_info("Unloading Assets");

        m_play_icon.destroy();
        m_stop_icon.destroy();
        m_content_browser.destroy();
    }

private:
    atlas::event::bus* m_bus = nullptr;
    float m_delta_time;
    atlas::physics::engine m_physics_engine;
    atlas::serializer m_deserializer_test;
    uint32_t m_host_bit = 0;
    float m_movement_speed = 10.f;
    flecs::entity m_selected_entity;
    std::shared_ptr<vk::device> m_device;
    scene_runtime m_scene_state = scene_runtime::edit;
    std::shared_ptr<atlas::scene> m_current_scene;
    atlas::ui::dockspace m_editor_dockspace;
    atlas::ui::menu_item m_editor_menu;
    ui::experimental::icon m_play_icon;
    ui::experimental::icon m_stop_icon;
    content_browser_panel m_content_browser;
};