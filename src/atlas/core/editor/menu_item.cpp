#include <core/editor/menu_item.hpp>
#include <imgui.h>

namespace atlas::ui {

    void menu_item::begin() {
        if(!ImGui::BeginMenuBar()) {
            throw menu_bar_exception("ImGui::BeginMenuBar failed!");
        }
    }

    void menu_item::end() {
        ImGui::EndMenuBar();
    }

    void menu_item::add_child(const std::string& p_name, const std::function<void()>& p_callback) {
        if(ImGui::MenuItem(p_name.c_str())) {
            p_callback();
        }
        ImGui::Separator();
    }
};