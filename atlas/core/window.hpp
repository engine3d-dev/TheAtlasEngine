#pragma once
#include <drivers/swapchain.hpp>
#include <core/core.hpp>
#include <string>
#include <GLFW/glfw3.h>

namespace atlas{

    class window{
    public:
        virtual ~window() = default;
        static ref<window> create(uint32_t Width, uint32_t Height, const std::string& Tag="Undefined Window Title");
        
        GLFWwindow* get_native_window() const;

        uint32_t get_width() const;
        uint32_t get_height() const;

        bool is_active() const;

        float get_aspect_ratio() const;

        ref<swapchain> get_current_swapchain();

        //! @note This treats our window class to be able to get called like a "GLFWwindow*"
        operator GLFWwindow*(){
            return get_native_window();
        }

        void close();

    private:
        virtual uint32_t read_width() const = 0;
        virtual uint32_t read_height() const = 0;
        virtual GLFWwindow* native_window() const = 0;
        virtual ref<swapchain> current_swapchain() = 0; 
    };
};