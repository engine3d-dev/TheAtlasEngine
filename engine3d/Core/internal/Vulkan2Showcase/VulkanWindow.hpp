#pragma once
#include <cstdint>
#include <string>
#include <Core/Window.hpp>
// #include <internal/Vulkan2Showcase/VulkanSwapchain.hpp>
#include <Core/GraphicDrivers/GraphicSwapchain.hpp>
#include <vulkan/vulkan_core.h>

class GLFWwindow;
namespace engine3d::vk{

    class VulkanWindow : public Window{
    public:
        VulkanWindow(const std::string& p_Tag, uint32_t w, uint32_t h);
    private:
        uint32_t Width() const override{ return m_Width; }
        uint32_t Height() const override{return m_Height; }
        std::string Title() const override{return m_Title; }

        bool CurrentWindowActive() const override;
        
        VkSurfaceKHR& VkSurface() override;

        Ref<GraphicSwapchain> Swapchain() override { return m_Swapchain; }

        //! @note Returns our current set native window API.
        GLFWwindow* NativeWindow() override;


        //! @note Update surface rendering every frame.
        void Presentation() override;
    private:
        uint32_t m_Width;
        uint32_t m_Height;
        std::string m_Title;
        GLFWwindow* m_Window;

        // VulkanPhysicalDriver m_PhysicalDriver;
        // VulkanDriver m_Driver;
        // VulkanSwapchain m_Swapchain;
        Ref<GraphicSwapchain> m_Swapchain;
        VkSurfaceKHR m_Surface;
    };
};