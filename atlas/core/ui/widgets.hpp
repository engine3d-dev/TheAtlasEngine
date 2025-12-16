#pragma once
#include <imgui.h>
#include <string>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <core/filesystem/file_dialog.hpp>
#include <core/scene/components.hpp>

namespace atlas::ui {

    /**
     * @brief This is a free-standing function around
     * ImGui::BeginPopupContextWindow
     *
     * It is primarily used to check if your mouse cursor is hovering over any
     * items and to opening items based on if the mouse is specifically hovering
     * over that particular item
     *
     *
     * @param p_name is the string ID that is needed to be specified by imgui
     * @param p_mb is the mouse button identifier that is an int to represent if
     * the presses were (0=left, 1=right, 2=middle)
     * @param p_over_items is a boolean to check if the mouse cursor has
     * hoivered over that particular item specified by the string ID
     *
     * @return true if successful, otherwise return false
     *
     */
    bool begin_popup_context_window(const char* p_name,
                                    ImGuiMouseButton p_mb,
                                    bool p_over_items);

    /**
     * @brief UI render glm::vec3 values to imgui
     *
     * @param p_name is the specified name ID to associate with
     * @param p_value is the value parameter to modify through this widget
     * @param p_reset_value is the reset value to set the glm::vec3
     */
    void draw_vec3(const std::string& p_name,
                   glm::vec3& p_value,
                   float p_reset_value = 0.f);

    /**
     * @brief UI render glm::vec4 values to imgui
     *
     * @param p_name is the specified name ID to associate with
     * @param p_value is the value parameter to modify through this widget
     * @param p_reset_value is the reset value to set the glm::vec3
     */
    void draw_vec4(const std::string& p_name,
                   glm::vec4& p_value,
                   float p_reset_value = 0.f);

    /**
     * @brief UI render glm::vec4 values to imgui
     *
     * @param p_name is the specified name ID to associate with
     * @param p_value is the value parameter to modify through this widget
     * @param p_reset_value is the reset value to set the glm::vec3
     */
    void draw_float(const std::string& p_tag,
                    float& p_value,
                    float p_reset_value = 0.f);

    /**
     * @brief UI rendering input text
     *
     *
     * @param p_dst is the destination string to be changed
     * @param p_src is the original string that was previously given
     */
    void draw_input_text(std::string& p_dst, std::string& p_src);

    /**
     * @brief For UI rendering text to display and not input
     *
     * @param p_value is specified string for drawing text to imgui's UI
     */
    void draw_text(const std::string& p_value);

    /**
     * @brief Creates a panel that can be used to attach list of properties or
     * other widgets too
     *
     * @tparam UComponent is the type of component or type to assoociate with
     * this panel too
     * @tparam UFunction is the callback that contains an arbitrary task related
     * to the specified UComponent
     * @param p_name is the name to give this panel
     * @param p_entity is the entity that is being modified by this specific
     * panel
     * @param p_callback is a callback that is given an arbitrary task
     *
     *
     * Example Usage:
     *
     * ```C++
     * draw_panel_component<transform>("transform", [](atlas::transform*
     * p_transform){
     *   // do some drawing with that transform
     * });
     * ```
     */
    template<typename UComponent, typename UFunction>
    static void draw_panel_component(const std::string& p_name,
                                     flecs::entity& p_entity,
                                     const UFunction& p_callback) {
        const ImGuiTreeNodeFlags tree_node_flags =
          ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
          ImGuiTreeNodeFlags_SpanAvailWidth |
          ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        ImVec2 content_region = ImGui::GetContentRegionAvail();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });

        float line_height =
          ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImGui::Separator();

        bool opened = ImGui::TreeNodeEx((void*)typeid(UComponent).hash_code(),
                                        tree_node_flags,
                                        "%s",
                                        p_name.c_str());
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
            p_entity.remove<UComponent>();
        }

        if (opened) {
            p_callback(p_entity.get_mut<UComponent>());
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

    /**
     * @brief used for creating a main dockspace for the editor
     *
     * TODO: This should be removed because this will be added into a separate
     * atlas/editor/ toolchain module
     */
    void dockspace_window(GLFWwindow* p_window);

    /**
     * @brief opens up a file dialog that uses imgui's button widget to make
     * this happen
     *
     * This uses ImGui::Button to allow for when a click occurs it opens up the
     * platform-specific file dialog to save or load in a particular file
     *
     * @param p_name is the specified name ID to pass into imgui
     * @param p_filepath is the path of the file
     * @param p_filter is the specific filter for file extensions to allow
     * displaying in the fild dialog
     */
    void button_open_file_dialog(const std::string& p_name,
                                 std::string& p_filepath,
                                 const std::string& p_filter = "obj;glftf;fbx");
};