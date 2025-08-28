#include <core/ui/widgets.hpp>
#include <imgui_internal.h> // Used to include "PushMultiItemsWidths"

namespace atlas::ui {
    bool begin_popup_context_window(const char* str_id,
                                    ImGuiMouseButton mb,
                                    bool over_items) {
        return ImGui::BeginPopupContextWindow(
          str_id, mb | (over_items ? 0 : ImGuiPopupFlags_NoOpenOverItems));
    }

    void draw_vec3(const std::string& p_tag,
                   glm::vec3& p_position,
                   float p_reset_value) {
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

    void draw_vec4(const std::string& p_tag,
                   glm::vec4& p_value,
                   float p_reset_value) {
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

    void draw_float(const std::string& p_tag,
                    float& p_value,
                    float reset_value) {
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

    void draw_input_text(std::string& p_value) {
        // @context Camera0
        char buffer[256];
        
        // To make null terminated.
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, p_value.c_str());

        if(ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
            p_value = std::string(buffer);
        }
    }

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

    void button_open_file_dialog(const std::string& p_tag,
                                 std::string& p_filename,
                                 const std::string& p_filter) {
        if (ImGui::Button(p_tag.c_str())) {
            p_filename = filesystem::load_from_file_dialog(p_filter);
        }
    }
};