#pragma once
#include <Core/GraphicDrivers/GraphicSwapchain.hpp>
#include <string>
#include <vulkan/vulkan_core.h>

class GLFWwindow;
namespace engine3d{
    // namespace vk{
    //     class VulkanSwapchain;
    // };
    /**
     * @name
     * @note Window-agnostic implementation
     * @note Having a window abstracted interface for having users define their own windows.
     * @param GetVkSurface returns the current vulkan surface of the currently active window
     * @param GetNativeWindow returns a pointer to the actual glfw window
     * @param IsActive returns a boolean that is used for checking our window is active before use.
     * @param OnUpdateAllFrames is used for displaying in the last stages of the swapchain of images ready for to be on the actual screen.
    */

    class Window{
    public:
        static Window* Create(uint32_t p_Width=900, uint32_t p_Height=600, const std::string& p_Title="Engine3D");

        /**
         * @name As(typename input);
         * @param UWindowSpecified represents the windowing system to convert to
         * @param IWidnowSpecified is what the user defines as their window.
         * @note TODO --- Probably would have this be in helper_functions.h as this is an exposed to the API that uses may used to get the right API.
        */
        // template<typename T>
        // static T As(){
        //     return static_cast<T>(GetNativeWindow());
        // }

        bool IsActive() const;

        //! @note TODO --- Replace VkSurface with something like DisplaySurface or something with this naming.
        //! @note Reason is because we may have one window, but multiple surfaces that may be rendered to.
        VkSurfaceKHR& GetVkSurface();
        
        //! @TODO: Modify vk::VulkanSwapchain into RenderSwapchain or to return an interface.
        // Like this: graphic_swapchain& GetCurrentSwapchain();
        Ref<GraphicSwapchain> GetCurrentSwapchain();
        
        GLFWwindow* GetNativeWindow();

        uint32_t GetWidth() const;
        uint32_t GetHeight() const;
        std::string GetTitle() const;

        float GetAspectRatio() const;
        void OnUpdateAllFrames();

    protected:
        //! @note These are all implementation details not exposed to the public API's
        //! @note Internal implementations the developer of the window should know about.
        virtual uint32_t Width() const = 0;
        virtual uint32_t Height() const = 0;
        virtual std::string Title() const = 0;
        // virtual uint32_t SwapchainImagesSize() const = 0;
        
        virtual bool CurrentWindowActive() const = 0;
        

        virtual VkSurfaceKHR& VkSurface() = 0;

        virtual Ref<GraphicSwapchain> Swapchain() = 0;

        //! @note Returns our current set native window API.
        virtual GLFWwindow* NativeWindow() = 0;

        //! @note Update surface rendering every frame.
        virtual void Presentation() = 0;
    };
};