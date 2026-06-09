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

export module editor:world;

import atlas.core.utilities;
import atlas.core.scene.world;
import atlas.core.event;
import atlas.drivers.renderer_system;
import atlas.core.scene.uuid;
import atlas.core.level_streamer;
import atlas.core.scene;
import :level_scene;
import :icon;
// import :level_scene2;

import atlas.core.editor.dockspace;
import atlas.core.editor.menu_item;
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
      : atlas::world(p_tag, p_level_streamer) {
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

        std::println("Before loading play and stop icons!");
        // Initializing icon.
        m_play_icon =
          ui::experimental::icon(m_device,
                                 p_context->physical_device().memory_properties(
                                   vk::memory_property::host_visible_bit |
                                   vk::memory_property::host_cached_bit),
                                 "assets/icons/PlayButton.png");

        if(!m_play_icon.loaded()) {
            std::println("m_play_icon not loaded!");
        }

        m_stop_icon =
          ui::experimental::icon(m_device,
                                 p_context->physical_device().memory_properties(
                                   vk::memory_property::host_visible_bit |
                                   vk::memory_property::host_cached_bit),
                                 "assets/icons/StopButton.png");


        if(!m_stop_icon.loaded()) {
            std::println("m_stop_icon not loaded!");
        }

        atlas::register_ui(this, &editor_world::ui_update);
    }

    ~editor_world() override = default;

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

            content_browser_panel();

            ui_toolbar();

            m_editor_dockspace.end();
        }
    }

    void scene_heirarchy_panel() {
        if (ImGui::Begin("Scene Heirarchy")) {
            ImGui::End();
        }
    }

    void properties_panel() {
        if (ImGui::Begin("Properties")) {
            ImGui::End();
        }
    }

    void materials_editor_panel() {
        if (ImGui::Begin("Material Editor")) {
            ImGui::End();
        }
    }

    void content_browser_panel() {
        if (ImGui::Begin("Content Browser")) {
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
        // if(ImGui::Begin("##toolbox")) {
        //     ImGui::End();
        // }

        if(ImGui::Begin("##toolbox")) {
            VkDescriptorSet button_id = (m_scene_state == scene_runtime::edit)
                                        ? m_play_icon.texture_id()
                                        : m_stop_icon.texture_id();

            /**
            * @note GetWindowContentRegionMax().x is how much space is there for
            * content (widgets)
            * @note 0.5f is the offset for padding.
            * @note takes button size and halves it and makes the offset the center
            * of that tab. (centering  buttons)
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
                    // m_physics_engine.start();
                }
                else if (m_scene_state == scene_runtime::play) {
                    m_scene_state = scene_runtime::edit;
                    // m_physics_engine.stop();
                    // reset_objects();
                }
            }

            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(3);
            ImGui::End();
        }
    }


    void destruct() {
        m_play_icon.destroy();
        m_stop_icon.destroy();
    }

private:
    std::shared_ptr<vk::device> m_device;
    scene_runtime m_scene_state = scene_runtime::edit;
    std::shared_ptr<atlas::scene> m_current_scene;
    atlas::ui::dockspace m_editor_dockspace;
    atlas::ui::menu_item m_editor_menu;
    ui::experimental::icon m_play_icon;
    ui::experimental::icon m_stop_icon;
};