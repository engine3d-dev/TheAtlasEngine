#pragma once
#include <engine3d/Core/core.h>
#include <string>
#include <cstdint>
#include <engine3d/Event/Event.h>
#include <functional>

class GLFWwindow;
namespace engine3d{

    struct WindowProperties{
        WindowProperties(const std::string& t="Engine3D", uint32_t w = 1600, uint32_t h = 900) : title(t), width(w), height(h) {}

        std::string title;
        uint32_t width, height;
        bool vsyncEnabled;
        std::function<void(Event& event)> callback;
    };

    //! @note This will act as our main viewport window
    //! @note Client can modify the window name by doing something like:
    class ENGINE_API Window{
        Window(const WindowProperties& properties = WindowProperties());
        using EventCallbackFn = std::function<void(Event& event)>;
    public:
        ~Window();

        void OnUpdate();

        void Shutdown();
        
        void SetName(const std::string& title);
        uint32_t GetWidth() const;
        uint32_t GetHeight() const;

        void SetEventCallback(const EventCallbackFn& callback);
        void SetVSync(bool enabled);
        bool IsVSyncEnabled() const;

        void* GetNativeWindow() const;

        static Window* Create(const WindowProperties& props = WindowProperties());

    private:
        GLFWwindow* m_windowHandler;
        WindowProperties m_properties;
    };
};