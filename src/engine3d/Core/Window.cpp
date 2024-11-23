#include <internal/Vulkan2Showcase/VulkanWindow.hpp>
#include <Core/ApplicationInstance.hpp>
#include <Core/EngineLogger.hpp>
#include <stdexcept>
#include <string>
#include <vulkan/vulkan_core.h>
#include <Core/TimeManagement/GlobalUpdateManager.hpp>

namespace engine3d{
    
    static Window* g_WindowAPI = nullptr;

    Window* Window::Create(uint32_t p_Width, uint32_t p_Height, const std::string &p_Title){
        switch (ApplicationInstance::CurrentAPI()){
        case API::VULKAN:
            return new vk::VulkanWindow(p_Title, p_Width, p_Height);
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

    //! @note Eventually, I'll wanna change the Graphic
    // graphic_swapchain& Window::GetCurrentSwapchain(){
    //     return CurrentSwapchain();
    // }
    Ref<GraphicSwapchain> Window::GetCurrentSwapchain(){
        return Swapchain();
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

    float Window::GetAspectRatio() const{
        return (float)GetWidth() / (float)GetHeight();
    }

    void Window::OnUpdateAllFrames(){

        GlobalUpdateManager::GetInstance()->GlobalOnTickUpdate();
    }
};