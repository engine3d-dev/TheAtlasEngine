#pragma once

namespace atlas {
    class imgui_backend {
    public:
        static void initialize();

        static void begin();
        static void end();
    };
};