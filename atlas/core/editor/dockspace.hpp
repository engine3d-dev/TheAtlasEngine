#pragma once
#include <GLFW/glfw3.h>

namespace atlas::ui {

    /**
     * @brief dockspace window using imgui
    */
    class dockspace {
    public:
        dockspace() = default;

        void fullscreen(bool p_fullscreen) { m_fullscreen_enabled = p_fullscreen; }

        void dockspace_open(bool p_dockspace_open) { m_is_dockspace_open = p_dockspace_open; }

        void begin();

        void end();

    private:
        bool m_fullscreen_enabled=false;
        bool m_is_dockspace_open=false;
    };
};