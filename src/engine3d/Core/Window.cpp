// #include <Core/Window.hpp>
#include <internal/Vulkan2Showcase/VulkanWindow.hpp>
#include <Core/ApplicationInstance.hpp>
#include <Core/EngineLogger.hpp>
#include <stdexcept>
#include <string>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    // Window* Window::Create(uint32_t p_Width, uint32_t p_Height, const std::string& p_Title){

    // }
    
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

    VkRenderPass& Window::GetRenderpass(){
        return VkRenderpass();
    }

    VkFramebuffer Window::GetFramebufferAt(uint32_t index){
        return ReadFramebufferAt(index);
    }

    uint32_t Window::AcquireNextImage(){
        return NextImagePerFrame();
    }

    void Window::Submit(VkCommandBuffer* p_CommandBuffer){
        SubmitCommandBufferToSwapchain(p_CommandBuffer);
    }

    uint32_t Window::Tick(){
        return PerFrameTick();
    }

    // Ref<Swapchain> Window::GetCurrentSwapchain(){
    //     return CurrentSwapchain();
    // }

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

    VkSwapchainKHR Window::GetVkSwapchain(){
        return VkSwapchain();
    }

    size_t Window::GetSwapchainImagesSize() const{
        return SwapchainImagesSize();
    }

    // graphic_swapchain& Window::GetCurrentSwapchain(){
    //     return CurrentSwapchain();
    // }


    void Window::OnUpdateAllFrames(){
        Presentation();
    }
};