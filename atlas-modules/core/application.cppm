module;

#include <cstdint>
// #include <glm/glm.hpp>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <vulkan/vulkan.h>
#include <string>
#include <print>
#include <optional>
#include <GLFW/glfw3.h>
#include <chrono>
#include <flecs.h>

export module atlas.application;

import atlas.graphics_api;
import atlas.logger;
import atlas.core.utilities.types;
import atlas.common;
import atlas.window;
import atlas.drivers;
import atlas.core.event;
import atlas.core.utilities.poll_state;
import atlas.logger;
import atlas.drivers.renderer_system;
import atlas.renderer;
import atlas.drivers.vulkan.instance_context;
import atlas.core.scene;
import atlas.core.scene.world;
import atlas.core.scene.system_registry;
import atlas.core.scene.components;
import atlas.core.math;
import vk;

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
            event::set_window_size(static_cast<GLFWwindow*>(*m_window));

            m_renderer = initialize_renderer(graphics_api::vulkan, params, m_window->current_swapchain().image_size(), "Renderer");
            m_renderer->set_background_color({
            p_params.background_color.x,
            p_params.background_color.y,
            p_params.background_color.z,
            p_params.background_color.w,
            });

            // instance_context::submit_resource_free([this](){
            // });
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
         * @brief Explicitly is used to execute the application's mainloop
         */
        void execute() {
            console_log_info("Executing game mainloop!!!");

            auto start_time = std::chrono::high_resolution_clock::now();
            m_renderer->preload(m_window->current_swapchain().swapchain_renderpass());

            invoke_start();

            ref<world> current_world = system_registry::get_world("Editor World");
            ref<scene> current_scene = current_world->get_scene("LevelScene");
            flecs::world current_world_scope = *current_scene;

            /*
                - flecs::system is how your able to schedule changes for given
                portions of data in this case the projection/view matrices are only
                being changed when flecs::world::progress(g_delta_time) is being
                invoked within the mainloop
                current_world_scope.system<projection_view, transform,
                perspective_camera>()

                - When users do object->add<flecs::pair<tag::editor,
                projection_view>>(), this automatically gets invoked by the
            .system<...> that gets invoked by the mainloop.
            */
            current_world_scope
            .system<flecs::pair<tag::editor, projection_view>,
                    transform,
                    perspective_camera>()
            .each([&](flecs::pair<tag::editor, projection_view> p_pair,
                        transform& p_transform,
                        perspective_camera& p_camera) {
                float aspect_ratio = m_window->aspect_ratio();
                if (!p_camera.is_active) {
                    return;
                }

                p_pair->projection = glm::mat4(1.f);

                p_pair->projection =
                    glm::perspective(glm::radians(p_camera.field_of_view),
                                    aspect_ratio,
                                    p_camera.plane.x,
                                    p_camera.plane.y);
                p_pair->projection[1][1] *= -1;
                p_pair->view = glm::mat4(1.f);

                // This is converting a glm::highp_vec4 to a glm::quat
                glm::quat quaternion = to_quat(p_transform.quaternion);

                p_pair->view =
                    glm::translate(p_pair->view, p_transform.position) *
                    glm::mat4_cast(quaternion);

                p_pair->view = glm::inverse(p_pair->view);
            });

            /*
                - Currently how this works is we query with anything that has a
            flecs::pair<tag::editor, projection_view>
                - This tells the ecs flecs what to do query for in regards to
            specific objects that are a camera
                - in the tag:: namespace, this is to imply components that are empty
            and just represent tags, to specify their uses.
            */
            auto query_camera_objects =
            current_scene
                ->query_builder<flecs::pair<tag::editor, projection_view>,
                                perspective_camera>()
                .build();



            while(m_window->available()) {
                auto current_time = std::chrono::high_resolution_clock::now();
                m_delta_time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();
                start_time = current_time;

                event::flush_events();

                // Progresses the flecs::world by one tick (or replaced with using
                // the delta time)
                // This also invokes the following system<T...> call  before the
                // mainloop
                current_world_scope.progress(m_delta_time);

                m_current_frame_index = m_window->acquired_next_frame();

                // Current commands that are going to be iterated through
                // Prevents things like stalling so the CPU doesnt have to wait for
                // the GPU to fully complete before starting on the next frame
                // Command buffer uses this to track the frames to process its
                // commands currently_active_frame = (m_current_frame_index + 1) %
                // m_window->current_swapchain().settings().frames_in_flight;
                // TODO: Going to need to figure out where to put this
                // Added this here because to ensure the handlers being used by the
                // renderer is in sync when swapchain is resized
                ::vk::command_buffer currently_active = m_window->active_command(m_current_frame_index);

                invoke_on_update();

                invoke_physics_update();

                invoke_defer_update();
                // We want this to be called after late update
                // This queries all camera objects within the camera system
                // Update -- going to be removing camera system in replacement of
                // just simply using flecs::system to keep it simple for the time
                query_camera_objects.each(
                [&](flecs::entity,
                    flecs::pair<tag::editor, projection_view> p_pair,
                    perspective_camera& p_camera) {
                    if (!p_camera.is_active) {
                        return;
                    }

                    m_proj_view = p_pair->projection * p_pair->view;
                });

                // TODO: Introduce scene renderer that will make use of the
                // begin/end semantics for setting up tasks during pre-frame
                // operations
                // renderer begin to indicate when a start of the frame to start
                // processing specific tasks that either need to be computed or
                // pre-defined before the renderer does something with it.
                // TODO: Add scene_manager to coordinate what to process
                // before frame preparation
                auto current_framebuffer =m_window->current_swapchain().active_framebuffer(m_current_frame_index);
                
                m_renderer->begin_frame(
                    currently_active,
                    m_window->current_swapchain().settings(),
                    m_window->current_swapchain().swapchain_renderpass(),
                    current_framebuffer,
                    m_proj_view,
                    m_current_frame_index);
                
                // execute UI logic
                invoke_ui_update();

                m_renderer->end_frame();
                
                /*
                TODO -- have m_window present this to the screen, eventually
                m_renderer should just fetch the images in the order to offload
                to the swapchain for rendering.

                    Where each image has gone through different phases of the
                renderpass onto the final image
                */

                std::array<const VkCommandBuffer, 1> commands = {
                    currently_active,
                };
                m_window->current_swapchain().submit(commands);
                
                // Presents to the swapchain to display to screen
                m_window->present(m_current_frame_index);

            }

        }

        /**
         * @brief Performs any post cleanup when user requests the application
         * to close
         */
        void post_destroy() {
            console_log_info("Executing post cleanup!!!");
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
            return m_window->current_swapchain();
        }

        /**
         * @brief destroys the application completely
         *
         * TODO: Not make this static because you should not allow for this to
         * be a direct calls users can have access to
         */
        static void destroy() {
            s_instance->m_window->close();
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
         * @brief Intended to get the image size so when you use current_frame()
         * to get thje frame index, that you are not making an attempt at
         * accessing anything outside of the frame.
         *
         * @return uint32_t
         */
        static uint32_t image_size() {
            return s_instance->m_window->current_swapchain().image_size();
        }

        static window& get_window() {
            return *s_instance->m_window;
        }

    protected:
        [[nodiscard]] ref<renderer_system> renderer_instance() const {
            return m_renderer;
        }

    private:
        float m_delta_time = 0.f;
        ref<window> m_window;
        // vulkan::instance_context m_instance_handle_test;
        // std::optional<vulkan::instance_context> m_instance_handle_test;
        ref<renderer_system> m_renderer = nullptr;
        glm::mat4 m_proj_view;
        uint32_t m_current_frame_index = -1;
        // vk::imgui_context m_ui_context;
        static application* s_instance;
    };
};


atlas::application* atlas::application::s_instance = nullptr;