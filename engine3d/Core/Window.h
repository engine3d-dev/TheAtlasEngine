#pragma once
#include <Core/Core.h>
#include <string>
#include <vulkan/vulkan_core.h>

class GLFWwindow;
namespace engine3d{
    class Window{
    public:
        // Window(uint32_t p_Width, uint32_t p_Height, const std::string& p_Title);
        static Window* Create(uint32_t p_Width=900, uint32_t p_Height=600, const std::string& p_Title="Engine3D");

        /**
         * @name As(typename input);
         * @param UWindowSpecified represents the windowing system to convert to
         * @param IWidnowSpecified is what the user defines as their window.
         * @note TODO --- Probably would have this be in helper_functions.h as this is an exposed to the API that uses may used to get the right API.
        */
        template<typename UWindowSpecified, typename IWindowSpecified>
        static UWindowSpecified* As(IWindowSpecified){
            return static_cast<UWindowSpecified *>(GetCurrentWindowAPI());
        }

        bool IsActive() const;

        VkSurfaceKHR& GetVkSurface();
        GLFWwindow* GetNativeWindow();

        uint32_t GetWidth() const;
        uint32_t GetHeight() const;
        std::string GetTitle() const;

        void OnUpdatePerTick();

        void UpdateFrames();

    protected:
        virtual uint32_t Width() const = 0;
        virtual uint32_t Height() const = 0;
        virtual std::string Title() const = 0;
        
        virtual bool CurrentWindowActive() const = 0;
        
        virtual VkSurfaceKHR& VkSurface() = 0;

        //! @note Returns our current set native window API.
        virtual GLFWwindow* NativeWindow() = 0;

        //! @note Update surface rendering every frame.
        virtual void UpdatePerTick() = 0;

        virtual void Presentation() = 0;
    };
};