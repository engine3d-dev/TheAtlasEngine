module;

#include <imgui_internal.h> // Used to include "PushMultiItemsWidths"
#include <glm/glm.hpp>
#include <string>
#include <GLFW/glfw3.h>
#include <flecs.h>
#include <type_traits>
#include <typeinfo>

export module atlas.core.ui.widgets;

import atlas.core.filesystem.file_dialog;
import atlas.core.scene.components;

export namespace atlas::ui {
    using ::ImGui::InputText;

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
    bool begin_popup_context_window(const char* str_id,
                                    ImGuiMouseButton mb,
                                    bool over_items) {
        return ImGui::BeginPopupContextWindow(
          str_id, mb | (over_items ? 0 : ImGuiPopupFlags_NoOpenOverItems));
    }

    /**
     * @brief UI render glm::vec3 values to imgui
     *
     * @param p_name is the specified name ID to associate with
     * @param p_value is the value parameter to modify through this widget
     * @param p_reset_value is the reset value to set the glm::vec3
     */
    void draw_vec3(const std::string& p_tag,
                   glm::vec3& p_position,
                   float p_reset_value = 0.f) {
        // ImGuiIO& io = ImGui::GetIO();
        ImGui::PushID(p_tag.c_str());

        float column_width = 125.0f;

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, column_width);
        ImGui::Text("%s", p_tag.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float line_height =
          ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImVec2 button_size = { line_height + 3.0f, line_height };

        ImGui::PushStyleColor(ImGuiCol_Button,
                              ImVec4{ 0.8, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4{ 0.2, 0.1f, 0.2f, 1.0f });

        if (ImGui::Button("X", button_size)) {
            p_position.x = p_reset_value;
            // ImGui::End();
        }

        // ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &p_position.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        // Setting up for the Y button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4{ 0.2, 0.1f, 0.2f, 1.0f });

        if (ImGui::Button("Y", button_size)) {
            p_position.y = p_reset_value;
            // ImGui::End();
        }

        // ImGui::PopFont();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragFloat("##Y", &p_position.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        // Setting up for the Z button
        ImGui::PushStyleColor(ImGuiCol_Button,
                              ImVec4{ 0.1, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4{ 0.8, 0.1f, 0.15f, 1.0f });
        if (ImGui::Button("Z", button_size)) {
            p_position.z = p_reset_value;
            // ImGui::End();
        }

        // ImGui::PopFont();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragFloat("##Z", &p_position.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

    /**
     * @brief UI render glm::vec4 values to imgui
     *
     * @param p_name is the specified name ID to associate with
     * @param p_value is the value parameter to modify through this widget
     * @param p_reset_value is the reset value to set the glm::vec3
     */
    void draw_vec4(const std::string& p_tag,
                   glm::vec4& p_value,
                   float p_reset_value = 0.f) {
        // ImGuiIO& io = ImGui::GetIO();
        ImGui::PushID(p_tag.c_str());

        float column_width = 100.0f;

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, column_width);
        ImGui::Text("%s", p_tag.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float line_height =
          ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImVec2 button_size = { line_height + 3.0f, line_height };

        ImGui::PushStyleColor(ImGuiCol_Button,
                              ImVec4{ 0.8, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4{ 0.2, 0.1f, 0.2f, 1.0f });

        if (ImGui::Button("X", button_size)) {
            p_value.x = p_reset_value;
            // ImGui::End();
        }

        // ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &p_value.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        // Setting up for the Y button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4{ 0.2, 0.1f, 0.2f, 1.0f });

        if (ImGui::Button("Y", button_size)) {
            p_value.y = p_reset_value;
            // ImGui::End();
        }

        // ImGui::PopFont();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragFloat("##Y", &p_value.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        // Setting up for the Z button
        ImGui::PushStyleColor(ImGuiCol_Button,
                              ImVec4{ 0.1, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4{ 0.8, 0.1f, 0.15f, 1.0f });
        if (ImGui::Button("Z", button_size)) {
            p_value.z = p_reset_value;
            // ImGui::End();
        }

        // ImGui::PopFont();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragFloat("##Z", &p_value.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

    /**
     * @brief UI render glm::vec4 values to imgui
     *
     * @param p_name is the specified name ID to associate with
     * @param p_value is the value parameter to modify through this widget
     * @param p_reset_value is the reset value to set the glm::vec3
     */
    void draw_float(const std::string& p_tag,
                    float& p_value,
                    float reset_value = 0.f) {
        ImGui::PushID(p_tag.c_str());

        float column_width = 100.0f;

        ImGui::Columns(2);

        ImGui::SetColumnWidth(0, column_width);
        ImGui::Text("%s", p_tag.c_str());
        ImGui::NextColumn();

        ImGui::PushItemWidth(ImGui::CalcItemWidth());
        // ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
        ImGui::PushStyleColor(ImGuiCol_Button,
                              ImVec4{ 0.8, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4{ 0.2, 0.1f, 0.2f, 1.0f });

        if (ImGui::Button(p_tag.c_str())) {
            p_value = reset_value;
            // ImGui::End();
        }

        // ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &p_value, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

    /**
     * @brief UI rendering input text
     *
     *
     * @param p_dst is the destination string to be changed
     * @param p_src is the original string that was previously given
     */
    void draw_input_text(std::string& p_dst, std::string& p_src) {
        std::string input_buffer = p_src;

        ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
        input_buffer.resize(255); // resize to allocate for 255 in the char
                                  // array since this should be long enough

        if (ImGui::InputText("##Name",
                             (char*)input_buffer.c_str(),
                             input_buffer.size() + 1,
                             flags)) {
            p_dst = input_buffer;
        }

        if (p_dst.empty()) {
            p_dst = p_src;
            return;
        }

        if (p_dst == p_src) {
            p_dst = p_src;
            return;
        }
    }

    /**
     * @brief For UI rendering text to display and not input
     *
     * @param p_value is specified string for drawing text to imgui's UI
     */
    void draw_text(const std::string& p_value) {
        ImGui::Text("%s", p_value.data());
    }

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
    void draw_panel_component(const std::string& p_name,
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
     * @brief used for creating a main dockspace for the editor
     *
     * TODO: This should be removed because this will be added into a separate
     * atlas/editor/ toolchain module
     */
    void dockspace_window(GLFWwindow* p_window) {

        bool dockspace_open = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags =
          ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if (opt_fullscreen) {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            window_flags |= ImGuiWindowFlags_NoTitleBar |
                            ImGuiWindowFlags_NoCollapse |
                            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus |
                            ImGuiWindowFlags_NoNavFocus;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
            window_flags |= ImGuiWindowFlags_NoBackground;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Dockspace Demo", &dockspace_open, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen) {
            ImGui::PopStyleVar(2);
        }

        // Dockspace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.f, 0.f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar()) {
            if (ImGui::MenuItem("Exit")) {
                glfwSetWindowShouldClose(p_window, true);
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();
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
    void draw_component(const std::string& p_tag,
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
    void button_open_file_dialog(
      const std::string& p_tag,
      std::string& p_filename,
      const std::string& p_filter = "obj;glftf;fbx") {
        if (ImGui::Button(p_tag.c_str())) {
            p_filename = filesystem::load_from_file_dialog(p_filter);
        }
    }
};