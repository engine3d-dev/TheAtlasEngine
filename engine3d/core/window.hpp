#pragma once
#include <drivers/swapchain.hpp>
#include <core/core.hpp>
#include <string>
#include <GLFW/glfw3.h>

namespace engine3d{

    class Window{
    public:
        virtual ~Window() = default;
        static Ref<Window> Create(uint32_t Width, uint32_t Height, const std::string& Tag="Undefined Window Title");
        
        GLFWwindow* GetNativeWindow() const;

        uint32_t GetWidth() const;
        uint32_t GetHeight() const;

        uint32_t GetFocusedWidth() const { return  FocusedWidth(); }
        uint32_t GetFocusedHeight() const { return FocusedHeight(); }

        bool IsWindowActive() const;

        float GetAspectRatio() const;

        Ref<Swapchain> GetCurrentSwapchain();

        //! @note This treats our window class to be able to get called like a "GLFWwindow*"
        operator GLFWwindow*(){
            return GetNativeWindow();
        }

        void Close();

    private:
        virtual uint32_t FocusedWidth() const = 0;
        virtual uint32_t FocusedHeight() const = 0;
        virtual uint32_t Width() const = 0;
        virtual uint32_t Height() const = 0;
        virtual GLFWwindow* NativeWindow() const = 0;
        virtual Ref<Swapchain> CurrentSwapchain() = 0; 
    };
};