#pragma once
#include <engine3d/Core/internal/Core.h>
#include <engine3d/Core/backend/third_party/imgui/imgui.h>
#include <string>

namespace engine3d{
    /**
     * @name UI
     * @note UI helper functions and UI-related API's for creating effective UI's widgets
     * @note Getting rid of imgui boilerplate. Due to error-prone
     * @note Clients should be able to create a widget rather then manually creating those widgets
    */
    class UI{
    public:
        template<typename UFunction>
        static void UI_RenderViewport(const std::string& name, const UFunction& function){
            if(ImGui::Begin(name.c_str())){
                function();
                ImGui::End();
            }
        }
    };
};