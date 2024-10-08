#pragma once
#include <Core/Renderer/RendererSettings.hpp>

namespace engine3d{
    class Renderer{
    public:
        static RendererSettings& GetSettings();
    };
};