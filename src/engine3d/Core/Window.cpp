#include <Core/Window.h>
#include <Core/EngineLogger.h>
#include <string>

namespace engine3d{
    static WindowProperties g_Properties;

    Window::Window(const std::string& title, uint32_t w, uint32_t h){
        if(!glfwInit()){
            CoreLogError("GLFWInit() called had failed!");
            return;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        g_Properties = {.title = title, .width = w, .height = h};

        m_WindowInstance = glfwCreateWindow((int)w, (int)h, title.c_str(),nullptr, nullptr);

        if(!m_WindowInstance){
            CoreLogError("m_WindowInstance could not work because it is nullptr!");
            return;
        }
    }

    GLFWwindow* Window::NativeWindow(){ return m_WindowInstance; }

    bool Window::IsWindowShutdown() const{
        return glfwWindowShouldClose(m_WindowInstance);
    }

    WindowProperties Window::GetProperties(){
        return g_Properties;
    }

    void Window::UpdateEvent(){
        glfwPollEvents();
    }
};