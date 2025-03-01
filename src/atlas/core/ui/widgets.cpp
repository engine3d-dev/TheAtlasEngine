#include <core/ui/widgets.hpp>

namespace atlas::ui{
    void draw_vec3(const std::string& Tag, glm::vec3& Position, float reset_value){
        // ImGuiIO& io = ImGui::GetIO();
        ImGui::PushID(Tag.c_str());

        float columnWidth = 100.0f;

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", Tag.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
        
        float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2, 0.1f, 0.2f, 1.0f});

        if(ImGui::Button("X", buttonSize)){
            Position.x = reset_value;
            // ImGui::End();
        }

        // ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &Position.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();


        // Setting up for the Y button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2, 0.1f, 0.2f, 1.0f});

        if(ImGui::Button("Y", buttonSize)){
            Position.y = reset_value;
            // ImGui::End();
        }

        // ImGui::PopFont();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragFloat("##Y", &Position.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        // Setting up for the Z button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1, 0.25f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8, 0.1f, 0.15f, 1.0f});
        if(ImGui::Button("Z", buttonSize)){
            Position.z = reset_value;
            // ImGui::End();
        }

        // ImGui::PopFont();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragFloat("##Z", &Position.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        
        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

    

    void draw_float(const std::string& Tag, float& value, float reset_value){
        ImGui::PushID(Tag.c_str());

        float columnWidth = 100.0f;

        ImGui::Columns(2);

        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", Tag.c_str());
        ImGui::NextColumn();

        ImGui::PushItemWidth(ImGui::CalcItemWidth());
        // ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2, 0.1f, 0.2f, 1.0f});

        if(ImGui::Button(Tag.c_str())){
            value = reset_value;
            // ImGui::End();
        }

        // ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &value, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

    void dockspace_window(GLFWwindow* Window){

        bool dockspace_open = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if(opt_fullscreen){
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode){
            window_flags |= ImGuiWindowFlags_NoBackground;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Dockspace Demo", &dockspace_open, window_flags);
        ImGui::PopStyleVar();

        if(opt_fullscreen){
            ImGui::PopStyleVar(2);
        }

        // Dockspace
        ImGuiIO& io = ImGui::GetIO();
        if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
            ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.f, 0.f), dockspace_flags);
        }

        if(ImGui::BeginMenuBar()){
            if(ImGui::MenuItem("Exit")){
                glfwSetWindowShouldClose(Window, true);
            }

            ImGui::EndMenuBar();
        }
    }

    void button_open_file_dialog(const std::string Tag, std::string& filename, const std::string& filter){
        if(ImGui::Button(Tag.c_str())){
            filename = filesystem::LoadFromFileDialog(filter);
        }
    }
};