#pragma once

namespace atlas {
    class ImGuiBackend {
    public:
        static void Initialize();

        static void Begin();
        static void End();
    };
};