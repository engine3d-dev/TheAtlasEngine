#pragma once
#include <Core/Renderer/RendererSettings.hpp>
#include <array>

namespace engine3d{
    class Renderer{
    public:

        static void BeginFrame();
        static void EndFrame();

        template<typename UFunction>
        static void Submit(const UFunction& p_Commands){
            
        }


        static void SetBackgroundColor(const std::array<float, 4>& rgba);

        static RendererSettings& GetSettings();
    };
};