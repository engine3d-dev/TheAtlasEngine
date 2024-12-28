#pragma once

namespace engine3d{
    class ImGuiBackend{
    public:
        static void Initialize();

        static void Begin();
        static void End();
    };
};