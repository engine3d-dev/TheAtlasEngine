#pragma once
#include "api.hpp"
#include <drivers/swapchain.hpp>
#include <string>
#include <core/window.hpp>

namespace atlas{
    struct application_settings {
        std::string Name = "Undefined";
        uint32_t Width = 0;
        uint32_t Height = 0;
    };
    class application_instance{
    public:
        application_instance(const application_settings& p_settings);
        void execute();

        static window& get_window() { return *s_ThisInstance->m_Window; }

        static API current_api();

        ref<swapchain> get_current_swapchain();

        static application_instance* super() { return s_ThisInstance; }

        static void ShutdownApplication();

        static uint32_t get_aspect_ratio();

    private:
        void set_current_api(API api);
    private:
        ref<window> m_Window;
        static application_instance* s_ThisInstance;
    };

    ref<application_instance> initialize_application();
};