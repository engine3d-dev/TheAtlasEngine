module;

#include <cstdint>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <vulkan/vulkan.h>
#include <string>
#include <print>
#include <chrono>
#include <utility>
#include <flecs.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <memory>
#include <memory_resource>
#include <optional>

export module atlas.application;

import atlas.core.utilities;
import atlas.core.utilities.poll_state;
import atlas.core.event;
import vk;
import atlas.drivers.graphics_context;
import atlas.window;

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
    // class application {
    // public:
    //     /**
    //      * @brief constructs a new application
    //      * @param p_settings is the specific application settings to
    //      configure
    //      * how the application may be setup
    //      */
    //     application(/*NOLINT*/ ref<graphics_context> p_context,
    //                 const application_settings& p_params,
    //                 event::bus& p_bus)
    //       : m_bus(&p_bus) {
    //         console_log_info(
    //           "application(const application_settings&) initialized!!!");

    //         window_params params = {
    //             .width = p_params.width,
    //             .height = p_params.height,
    //             .name = p_params.name,
    //         };
    //         m_window =
    //           initialize_window(p_context, params, graphics_api::vulkan);
    //         // m_initial_window_params = m_window->data();
    //         event::set_window_size(static_cast<GLFWwindow*>(*m_window));

    //         // m_renderer =
    //         //   initialize_renderer(p_context,
    //         //                       graphics_api::vulkan,
    //         //                       params,
    //         // m_window->current_swapchain().image_size(),
    //         //                       "Renderer");
    //         // m_renderer->set_background_color(p_params.background_color);

    //         // m_ui_context = vulkan::imgui_context(
    //         //   p_context->handle(), m_window->current_swapchain(),
    //         *m_window);

    //         // p_context->submit_resource_free(
    //         //   [this]() { m_ui_context.destroy(); });

    //         s_instance = this;

    //         // Setting internal-level listeners for specific events
    //         m_bus->create_listener<atlas::event::collision_enter>();
    //         m_bus->create_listener<atlas::event::collision_persisted>();
    //         m_bus->create_listener<atlas::event::collision_exit>();
    //         m_bus->create_immediate_listener<atlas::event::mesh_reload>();
    //         m_bus->create_immediate_listener<atlas::event::material_reload>();

    //         m_bus->trigger<event::mesh_reload>(this,
    //         &application::reload_mesh);
    //         m_bus->trigger<event::material_reload>(
    //           this, &application::reload_material);
    //     }

    //     ~application() { m_window->close(); }

    //     /**
    //      * @return the delta time as a float for giving you the timestep
    //      every
    //      * frame
    //      */
    //     static float delta_time() { return s_instance->m_delta_time; }

    //     /**
    //      * @brief Explicitly is used to execute the application's mainloop
    //      */
    //     void execute() {
    //         console_log_info("Executing game mainloop!!!");

    //         auto start_time = std::chrono::high_resolution_clock::now();

    //         ref<scene> current_scene = m_current_world->current();

    //         invoke_start(current_scene.get());

    //         while (m_window->available()) {
    //             auto current_time =
    //             std::chrono::high_resolution_clock::now(); m_delta_time =
    //               std::chrono::duration<float, std::chrono::seconds::period>(
    //                 current_time - start_time)
    //                 .count();
    //             start_time = current_time;

    //             event::flush_events();

    //             // Progresses the flecs::world by one tick (or replaced with
    //             // using the delta time) This also invokes the following
    //             // system<T...> call  before the mainloop
    //             current_scene->progress(m_delta_time);

    //             m_current_frame_index = m_window->acquired_next_frame();

    //             // Current commands that are going to be iterated through
    //             // Use the acquired swapchain image index for the command
    //             buffer
    //             // and swapchain framebuffer so we record and present the
    //             same
    //             // image.
    //             vk::command_buffer currently_active =
    //               m_window->active_command(m_current_frame_index);

    //             // invoke_physics_update(current_scene.get());

    //             // invoke_on_update(current_scene.get(), m_delta_time);

    //             // invoke_defer_update(current_scene.get());

    //             // m_ui_context.begin(currently_active,
    //             m_current_frame_index);

    //             // invoke_ui_update(current_scene.get());

    //             std::array<const VkCommandBuffer, 1> commands = {
    //                 currently_active,
    //             };
    //             m_window->current_swapchain().submit(commands);

    //             m_window->present(m_current_frame_index);
    //         }
    //     }

    //     /**
    //      * @brief Performs any post cleanup when user requests the
    //      application
    //      * to close
    //      */
    //     void post_destroy() { console_log_info("Executing post cleanup!!!");
    //     }

    //     /**
    //      * @brief we only ever have one window
    //      *
    //      * This static function was a means to getting access to the window
    //      to
    //      * perform any operations or request any data the window may have to
    //      * provide
    //      */
    //     // static window& get_window() { return *s_instance->m_window; }

    //     /* Retrieves the current selected graphics API */
    //     /**
    //      * @return the currently specified API.
    //      */
    //     static graphics_api current_api() { return graphics_api::vulkan; }

    //     /* Returns the currently selected swapchain */
    //     /**
    //      * @brief gives you the current swapchain handle
    //      *
    //      * TODO: This is not actually needed, and should be removed
    //      */
    //     VkSwapchainKHR get_current_swapchain() {
    //         return m_window->current_swapchain();
    //     }

    //     void current_world(ref<world> p_world) { m_current_world = p_world; }

    //     void reload_mesh(event::mesh_reload&) {
    //         console_log_info(
    //           "application::trigger<UEvent> invoked from core/application!");
    //     }

    //     void reload_material(event::material_reload&) {
    //         console_log_info(
    //           "application::trigger<material> invoked from
    //           core/application!");
    //     }

    // protected:
    //     [[nodiscard]] ref<renderer_system> renderer_instance() const {
    //         // return m_renderer;
    //         return nullptr;
    //     }

    // private:
    //     float m_delta_time = 0.f;
    //     ref<world> m_current_world;
    //     ref<window> m_window;
    //     // window_params m_initial_window_params;
    //     // ref<renderer_system> m_renderer = nullptr;
    //     glm::mat4 m_projection;
    //     glm::mat4 m_view;
    //     uint32_t m_current_frame_index = -1;
    //     // vulkan::imgui_context m_ui_context;
    //     event::bus* m_bus = nullptr;
    //     static application* s_instance;
    // };

    class application {
    public:
        application() = default;
        application(std::shared_ptr<graphics_context> p_context,
                    const application_settings& p_params,
                    event::bus& p_bus)
          : m_context(p_context), m_bus(&p_bus) {
            m_instance = p_context->instance_handle();
            m_physical = p_context->physical_device();
            m_device = p_context->logical_device();

            // Constructing the application
            std::println("Constructing application");
            window_params params = {
                .width = p_params.width,
                .height = p_params.height,
                .name = p_params.name,
            };
            // m_window = std::allocate_shared<window>(, m_context->instance_handle(), params);
            m_window = std::make_shared<window>(p_context, params);

            std::println("After constructing atlas::window");
        }

        void execute() {
            std::println("Executing mainloop");

            while(m_window->available()) {
                event::flush_events();
            }
        }

        void post_destroy() {
            std::println("Post destroy!");
        }

    private:
        vk::instance m_instance;
        std::optional<vk::physical_device> m_physical;
        std::shared_ptr<vk::device> m_device;
        std::shared_ptr<graphics_context> m_context;
        std::shared_ptr<window> m_window=nullptr;
        event::bus* m_bus = nullptr;
        static application* s_instance;
    };

    application* application::s_instance = nullptr;
};