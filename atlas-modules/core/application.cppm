module;

#include <cstdint>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <string>
#include <print>
#include <optional>

export module core:application;

import atlas.graphics_api;
import atlas.logger;
import atlas.common;
// import atlas.drivers.vulkan.window_context;
import atlas.window;
import atlas.drivers;

export namespace atlas {


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
        application(const application_settings& p_params) {
            console_log_info("application(const application_settings&) initialized!!!");

            window_params params = {
                .width = p_params.width,
                .height = p_params.height,
                .name = p_params.name,
            };
            m_window = initialize_window(params, graphics_api::vulkan);

            s_instance = this;
        }

        ~application() {
            destroy();
        }

        /**
         * @return the delta time as a float for giving you the timestep every
         * frame
         */
        static float delta_time() {
            return s_instance->m_delta_time;
        }

        /**
         * TODO: Need to remove this as it is not needed here
         *
         * Originally was used to handle the fixed physics timestep, but that
         * can be handled else where.
         */
        static float physics_step() {
            return 0.f;
        }

        /**
         * @brief Explicitly is used to execute the application's mainloop
         */
        void execute() {
            // execute
            std::println("Executing game mainloop!!!");
        }

        /**
         * @brief Performs any post cleanup when user requests the application
         * to close
         */
        void post_destroy() {
            std::println("Executing post cleanup!!!");
        }

        /**
         * @brief we only ever have one window
         *
         * This static function was a means to getting access to the window to
         * perform any operations or request any data the window may have to
         * provide
         */
        // static window& get_window() { return *s_instance->m_window; }

        /* Retrieves the current selected graphics API */
        /**
         * @return the currently specified API.
         */
        static graphics_api current_api() {
            return graphics_api::vulkan;
        }

        /* Returns the currently selected swapchain */
        /**
         * @brief gives you the current swapchain handle
         *
         * TODO: This is not actually needed, and should be removed
         */
        VkSwapchainKHR get_current_swapchain() {
            return nullptr;
        }

        /**
         * @brief destroys the application completely
         *
         * TODO: Not make this static because you should not allow for this to
         * be a direct calls users can have access to
         */
        static void destroy() {
        }

        /**
         * @brief gives you the current aspect ratio based on the dimensions of
         * the window
         *
         * @return a float which is just a static_cast<float>(width / height);
         */
        static float aspect_ratio() {
            return 0.f;
        }

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
        static uint32_t current_frame() {
            return 0;
        }

        /**
         * @brief Intended to get the image size so when you use current_frame()
         * to get thje frame index, that you are not making an attempt at
         * accessing anything outside of the frame.
         *
         * @return uint32_t
         */
        static uint32_t image_size() {
            return 0;
        }

    protected:
        // [[nodiscard]] ref<renderer> renderer_instance() const {
        //     return m_renderer;
        // }

    private:
        void set_current_api(graphics_api api) {
        }

    private:
        float m_delta_time = 0.f;
        ref<window> m_window;
        // vulkan::instance_context m_instance_handle_test;
        // std::optional<vulkan::instance_context> m_instance_handle_test;
        // ref<renderer> m_renderer = nullptr;
        glm::mat4 m_proj_view;
        uint32_t m_current_frame_index = -1;
        // vk::imgui_context m_ui_context;
        static application* s_instance;
    };
};


atlas::application* atlas::application::s_instance = nullptr;