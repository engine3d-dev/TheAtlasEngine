#pragma once
#include <engine3d/Core/core.h>
#include <engine3d/Core/Window.h>
#include <engine3d/Core/LayerStack.h>
#include <engine3d/ImGui/ImGuiLayer.h>
#include <engine3d/Event/ApplicationEvent.h>
#include <engine3d/Graphics/Framebuffer.h>
#include "glm/glm.hpp"
// #include <fmt/fmt.h>

namespace Engine3D{

    // Application::SetName("Engine3D")
    // Application::SetSize(w, h);
    // Application::SetVSync(false);
    class Application{
    public:
        Application(const std::string& name="Engine3D");
        ~Application();

        static void SetName(const std::string& title);

        // static void ResizeApplicationWindow(uint32_t w, uint32_t h);

        static Application& Get();

        static void* GetNativeWindow();

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        //! @note This checks if the layer we are pushing is an actual kind of client-layer
        //! @note Which is valid.
        template<typename T, typename = std::enable_if_t<std::is_base_of<Layer, T>::value>>
        void PushLayer(){
            T* layer = new T();
            m_LayerStack.PushLayer(layer);
            layer->OnAttach();
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Layer, T>::value>>
        void PushOverlay(T* layer){
            m_LayerStack.PushOverlay(layer);
            layer->OnAttach();
        }

        static glm::vec2 WindowSize();

        //! @note Enabling us to have a way at resizing our frame buffer
        //! @note This may change later on.
        void FramebufferResize(uint32_t w, uint32_t h);

        static void SetApplicationBlockedEvents(bool blocked);

        //! @note For right now client will know what our framebuffer data is
        //! @note TODO --- Though as Engine3D exands, this will also be hidden away from the client
        static const FramebufferMetaData& GetFramebufferData();

        static const Framebuffer& CurrentApplicationFramebuffer();

        static void Close();

        void CloseApplication();

    private:
        Scope<Window> m_Window;
        LayerStack m_LayerStack;
        static Application* instance;
        bool m_IsApplicationRunning=false;
        float m_LastFrameTime = 0.0f;
        bool m_IsAppMinimized;
        ImGuiLayer* m_ImGuiLayer;
        Ref<Framebuffer> m_Framebuffer;
    };

    // Application* CreateApplication(ApplicationCommandLineArgs args);
    Application* CreateApplication();
};