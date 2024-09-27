// #include <Core/Window.h>
#include <Core/Window.h>
#include <Core/EngineLogger.h>
#include <string>

namespace engine3d{
    static WindowProperties g_Properties;

    Window::Window(uint32_t p_Width, uint32_t p_Height, const std::string& p_Title){
        if(!glfwInit()){
            ConsoleLogError("GLFWInit() called had failed!");
            return;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        g_Properties = {.title = p_Title, .width = p_Width, .height = p_Height};

        m_WindowInstance = glfwCreateWindow((int)p_Width, (int)p_Height, p_Title.c_str(),nullptr, nullptr);

        if(!m_WindowInstance){
            ConsoleLogError("m_WindowInstance could not work because it is nullptr!");
            return;
        }
    }

    void* Window::GetCurrentWindowAPI(){ return m_WindowInstance; }

    bool Window::IsWindowShutdown() const{
        return glfwWindowShouldClose(m_WindowInstance);
    }

    WindowProperties Window::GetProperties(){
        return g_Properties;
    }

    void Window::OnUpdateDisplay(){
        glfwPollEvents();
    }
};