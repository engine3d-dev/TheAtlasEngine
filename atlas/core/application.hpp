#pragma once
#include <core/api.hpp>
#include <core/window.hpp>
#include <drivers/swapchain.hpp>
#include <string>
#include <thread_utils/thread.hpp>

namespace atlas {
    struct application_settings {
        std::string Name = "Undefined";
        uint32_t Width = 0;
        uint32_t Height = 0;
    };
    class application {
    public:
        application(const application_settings& p_settings);
        ~application();

        static float delta_time();

        /* executes the application's mainloop */
        void execute();

        /* Returns the current window selected in the application */
        static window& get_window() { return *s_instance->m_window; }

        /* Retrieves the current selected graphics API */
        static API current_api();

        /* Returns the currently selected swapchain */
        ref<swapchain> get_current_swapchain();

        /* Destroys the application */
        static void destroy();

        static uint32_t get_aspect_ratio();

    private:
        void set_current_api(API api);

    private:
        ref<window> m_window;
        thread m_sync_update_thread;
        static application* s_instance;
    };

    ref<application> initialize_application();
};