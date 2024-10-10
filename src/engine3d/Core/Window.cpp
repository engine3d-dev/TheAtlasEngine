// #include <Core/Window.hpp>
#include <internal/VulkanCpp/VulkanWindow.hpp>
#include "ApplicationInstance.hpp"
#include "TimeManagement/GlobalUpdateManager.hpp"
#include <Core/EngineLogger.hpp>
#include <stdexcept>
#include <string>

namespace engine3d{
    // Window* Window::Create(uint32_t p_Width, uint32_t p_Height, const std::string& p_Title){

    // }
    
    static Window* g_WindowAPI = nullptr;

    Window* Window::Create(uint32_t p_Width, uint32_t p_Height, const std::string &p_Title){
        switch (ApplicationInstance::CurrentAPI()){
        case API::VULKAN:
            return new vk::VulkanWindow(p_Width, p_Height, p_Title);
        default:
            throw std::runtime_error("API was unspecified!");
        }
        return nullptr;
    }

    bool Window::IsActive() const{
        return CurrentWindowActive();
    }

    VkSurfaceKHR& Window::GetVkSurface(){
        return VkSurface();
    }

    GLFWwindow* Window::GetNativeWindow(){
        return NativeWindow();
    }

    uint32_t Window::GetWidth() const{
        return Width();
    }

    uint32_t Window::GetHeight() const{
        return Height();
    }

    std::string Window::GetTitle() const{
        return Title();
    }


    void Window::OnUpdateAllFrames(){
        Presentation();
    }
};