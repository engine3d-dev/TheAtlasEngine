#include <GLFW/glfw3.h>
#include <core/application_instance.hpp>
#include <core/window.hpp>
#include <core/engine_logger.hpp>
#include <string>
// #include <imgui/backends/imgui_impl_glfw.h>
#include <drivers/vulkan/vulkan_window.hpp>

namespace engine3d{
    static uint32_t g_WindowWidth = -1;
    static uint32_t g_WindowHeight = -1;

    Ref<Window> Window::Create(uint32_t Width, uint32_t Height, const std::string& Tag){
        g_WindowWidth = Width;
        g_WindowHeight = Height;
        switch (ApplicationInstance::CurrentAPI()){
        case VULKAN:
            return CreateRef<vk::VulkanWindow>(Width, Height, Tag);
        default:
            ConsoleLogError("API that was input was not specifiying valid backend!");
            return nullptr;
        }

        return nullptr;
    }

    Ref<Swapchain> Window::GetCurrentSwapchain(){
        return CurrentSwapchain();
    }

    uint32_t Window::GetWidth() const { return g_WindowWidth; }

    uint32_t Window::GetHeight() const { return g_WindowHeight; }

    bool Window::IsWindowActive() const { return !glfwWindowShouldClose(GetNativeWindow()); }

    void Window::Close(){
        // glfwDestroyWindow(GetNativeWindow());
        glfwSetWindowShouldClose(GetNativeWindow(), true);
    }

    float Window::GetAspectRatio() const { return (float)GetWidth() / GetHeight(); }

    GLFWwindow* Window::GetNativeWindow() const{ return NativeWindow(); }
};