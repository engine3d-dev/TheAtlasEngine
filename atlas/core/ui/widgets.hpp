#pragma once
#include <imgui.h>
#include <string>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <core/filesystem/file_dialog.hpp>
#include <core/scene/scene_object.hpp>

namespace atlas::ui {

    bool begin_popup_context_window(const char* p_str_id,
                                    ImGuiMouseButton p_mb,
                                    bool p_over_items);

    /* Rendering glm::vec3's as a UI widget */
    void draw_vec3(const std::string& p_tag,
                   glm::vec3& p_value,
                   float p_reset_value = 0.f);

    /**
     * @name draw_vec4
     * @brief Render values of glm::vec4 as a GUI section
     */
    void draw_vec4(const std::string& p_tag,
                   glm::vec4& p_value,
                   float p_reset_value = 0.f);

    /* Rendering float as a UI widget with default reset values */
    void draw_float(const std::string& p_tag,
                    float& p_value,
                    float p_reset_value = 0.f);

    void draw_input_text(std::string& p_value);
	void draw_text(const std::string& p_value);

    // TODO -- Fix this. In-progress of converting all atlas::ref to
    // atlas::strong_ref in the parameters
    template<typename T, typename UFunction>
    static void draw_panel_component(const std::string& p_tag,
                                     strong_ref<scene_object>& p_entity,
                                     const UFunction& p_callable) {
        const ImGuiTreeNodeFlags tree_node_flags =
          ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
          ImGuiTreeNodeFlags_SpanAvailWidth |
          ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        ImVec2 content_region = ImGui::GetContentRegionAvail();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });

        float line_height =
          ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImGui::Separator();

        bool opened = ImGui::TreeNodeEx(
          (void*)typeid(T).hash_code(), tree_node_flags, "%s", p_tag.c_str());
        ImGui::PopStyleVar();

        ImGui::SameLine(content_region.x - line_height * 0.05f);

        if (ImGui::Button("+", ImVec2(line_height, line_height))) {
            ImGui::OpenPopup("ComponentSettings");
        }

        bool remove_component = false; // @note for deferring when to
        // delete component.
        if (ImGui::BeginPopup("ComponentSettings")) {
            if (ImGui::MenuItem("Remove Component")) {
                remove_component = true;
            }

            ImGui::EndPopup();
        }

        if (remove_component) {
            p_entity->remove<T>();
        }

        if (opened) {
            p_callable();
            ImGui::TreePop();
        }
    }

    /**
        draw panel component

        ImGui Renders individual sections that is per-component section
        @param T is the specified component
        @param p_entity is the entity to do operations with
        @param UFunction is callback to logic for setting up those specific data
       properties
    */
    template<typename T, typename UFunction>
    static void draw_component(const std::string& p_tag,
                               flecs::entity& p_entity,
                               const UFunction& p_callable) {
        const ImGuiTreeNodeFlags tree_node_flags =
          ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
          ImGuiTreeNodeFlags_SpanAvailWidth |
          ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        if (!p_entity.has<T>()) {
            return;
        }

        T* component = p_entity.get_mut<T>();

        ImVec2 content_region = ImGui::GetContentRegionAvail();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });

        float line_height =
          ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImGui::Separator();

        bool opened = ImGui::TreeNodeEx(
          (void*)typeid(T).hash_code(), tree_node_flags, "%s", p_tag.c_str());
        ImGui::PopStyleVar();

        ImGui::SameLine(content_region.x - line_height * 0.05f);

        if (ImGui::Button("+", ImVec2(line_height, line_height))) {
            ImGui::OpenPopup("ComponentSettings");
        }

        bool remove_component = false; // @note for deferring when to
        // delete component.
        if (ImGui::BeginPopup("ComponentSettings")) {
            if (ImGui::MenuItem("Remove Component")) {
                remove_component = true;
            }

            ImGui::EndPopup();
        }

        if (opened) {
            p_callable(component);
            ImGui::TreePop();
        }

        if (remove_component and !std::is_same_v<T, transform>) {
            p_entity.remove<T>();
        }
    }

    //! @note Sets up dockspaces
    void dockspace_window(GLFWwindow* p_window);

    //! @note Sets up UI button for loading files
    void button_open_file_dialog(const std::string& p_tag,
                                 std::string& p_filepath,
                                 const std::string& p_filter = "obj;glftf;fbx");
};