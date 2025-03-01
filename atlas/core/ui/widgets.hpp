#pragma once
#include <imgui.h>
#include <string>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <core/filesystem/file_dialog.hpp>

static const std::filesystem::path g_asset_path = "3d_models";

namespace ImGui{
    //! @note We need to see if this works
    //! @note Because imgui implements this but the header file for some reason does not contain this implementation...
    void PushMultiItemsWidths(int components, float w_full);
};

namespace atlas::ui{

    /* Rendering glm::vec3's as a UI widget */
    void draw_vec3(const std::string& Tag, glm::vec3& Position, float reset_value = 0.f);

    /* Rendering float as a UI widget with default reset values */
    void draw_float(const std::string& Tag, float& value, float reset_value=0.f);

    /*
        draw panel component

        - is used to group widgets into specific panels

        Parameters
        T = is the type the component we want to draw to the UI
        UFunction = callback that defines what data in the component to be displayed in this panel that handles the UI layout of that component

        USAGE:

        DrawPanelCompoent<Component>("Transform", [](){
            DrawVec3("Position", SomePosition);
            // etc....
        });
    */
    template<typename T, typename UFunction>
    static void draw_panel_component(const std::string& Tag, const UFunction& p_callable){
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        ImVec2 contentRegion = ImGui::GetContentRegionAvail();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4,4});

        float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImGui::Separator();

        bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", Tag.c_str());
        ImGui::PopStyleVar();

        ImGui::SameLine(contentRegion.x - lineHeight * 0.05f);

        if(ImGui::Button("+", ImVec2(lineHeight, lineHeight))){
            ImGui::OpenPopup("ComponentSettings");
        }

        // bool isRemovedComponent = false; // @note for deferring when to delete component.
        if(ImGui::BeginPopup("ComponentSettings")){
            if(ImGui::MenuItem("Remove Component")){

            }
            //     isRemovedComponent = true;

            ImGui::EndPopup();
        }

        if(opened){
            p_callable();
            ImGui::TreePop();
        }

    }

    //! @note Sets up dockspaces
    void dockspace_window(GLFWwindow* Window);

    //! @note Sets up UI button for loading files
    void button_open_file_dialog(const std::string Tag, std::string& filename, const std::string& filter = "obj;glftf;fbx");
};