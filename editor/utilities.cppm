module;

#include <flecs.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <string>


export module editor:utilities;
import atlas.core.ui.widgets;
import atlas.core.scene.components;

export void
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