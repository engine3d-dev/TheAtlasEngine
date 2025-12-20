#pragma once
#include <core/api.hpp>
#include <core/window.hpp>
#include <string>
#include <renderer/renderer.hpp>
#include <drivers/vulkan-cpp/vk_imgui.hpp>

namespace atlas {

    /**
     * @brief application properties settings for the window
     */
    struct application_settings {
        std::string name = "Undefined";
        uint32_t width = 0;
        uint32_t height = 0;
        glm::vec4 background_color = { 1.f, 0.5f, 0.5f, 1.f };
    };

    /**
     * @brief represents a single application that gets created by the engine
     * internally
     *
     * There is only ever going to be one application tied to the engine's
     * runtime. As the application is given responsibilities of preloading,
     * pre-initialization any sort of utilities required by the engine, and any
     * form of post-cleanup when the user requests the application to close.
     *
     */
    class application {
    public:
        /**
         * @brief constructs a new application
         * @param p_settings is the specific application settings to configure
         * how the application may be setup
         */
        application(const application_settings& p_settings);

        ~application();

        /**
         * @return the delta time as a float for giving you the timestep every
         * frame
         */
        static float delta_time();

        /**
         * TODO: Need to remove this as it is not needed here
         *
         * Originally was used to handle the fixed physics timestep, but that
         * can be handled else where.
         */
        static float physics_step();

        /**
         * @brief Explicitly is used to execute the application's mainloop
         */
        void execute();

        /**
         * @brief Performs any post cleanup when user requests the application
         * to close
         */
        void post_destroy();

        /**
         * @brief we only ever have one window
         *
         * This static function was a means to getting access to the window to
         * perform any operations or request any data the window may have to
         * provide
         */
        static window& get_window() { return *s_instance->m_window; }

        /* Retrieves the current selected graphics API */
        /**
         * @return the currently specified API.
         */
        static api current_api();

        /* Returns the currently selected swapchain */
        /**
         * @brief gives you the current swapchain handle
         *
         * TODO: This is not actually needed, and should be removed
         */
        VkSwapchainKHR get_current_swapchain();

        /**
         * @brief destroys the application completely
         *
         * TODO: Not make this static because you should not allow for this to
         * be a direct calls users can have access to
         */
        static void destroy();

        /**
         * @brief gives you the current aspect ratio based on the dimensions of
         * the window
         *
         * @return a float which is just a static_cast<float>(width / height);
         */
        static float aspect_ratio();

        /**
         * @brief Gives you the current frame index which is used for the Vulkan
         * renderer
         *
         * Provides information such as what is the current frame index to
         * correcly index the commands that get submitted to the GPU for
         * processing commands (tasks)
         *
         * @return uint32_t
         */
        static uint32_t current_frame();

        /**
         * @brief Intended to get the image size so when you use current_frame()
         * to get thje frame index, that you are not making an attempt at
         * accessing anything outside of the frame.
         *
         * @return uint32_t
         */
        static uint32_t image_size();

    protected:
        [[nodiscard]] ref<renderer> renderer_instance() const {
            return m_renderer;
        }

    private:
        void set_current_api(api api);

    private:
        float m_delta_time = 0.f;
        ref<window> m_window;
        ref<renderer> m_renderer = nullptr;
        glm::mat4 m_proj_view;
        uint32_t m_current_frame_index = -1;
        vk::imgui_context m_ui_context;
        static application* s_instance;
    };

    ref<application> initialize_application();
};