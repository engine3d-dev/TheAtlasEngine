#pragma once
#include <Core/core.h>
#include <string>
#include <GLFW/glfw3.h>

namespace engine3d{
    struct WindowProperties{
        std::string title="Default";
        uint32_t width=0, height=0;
    };

    class Window{
    public:
        Window(const std::string& title, uint32_t w, uint32_t h);
        GLFWwindow* NativeWindow();

        bool IsWindowShutdown() const;

        void UpdateEvent();

        static WindowProperties GetProperties();
    public:
        GLFWwindow* m_WindowInstance;
    };
};