module;

#include <cstdint>
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
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>

export module atlas.application;

import atlas.core.utilities;
import atlas.core.utilities.poll_state;
import atlas.window;
import atlas.drivers;
import atlas.renderer.context_loader;
import atlas.core.event;
import atlas.drivers.renderer_system;
import atlas.renderer;
import atlas.drivers.vulkan.instance_context;
import atlas.core.scene;
import atlas.core.scene.world;
import atlas.core.scene.components;

import atlas.core.math;
import atlas.drivers.vulkan.imgui_context;
import vk;
import atlas.drivers.graphics_context;

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
        application(ref<graphics_context> p_context,
                    const application_settings& p_params) {
            console_log_info(
              "application(const application_settings&) initialized!!!");

            window_params params = {
                .width = p_params.width,
                .height = p_params.height,
                .name = p_params.name,
            };
            m_window =
              initialize_window(p_context, params, graphics_api::vulkan);
            m_initial_window_params = m_window->data();
            event::set_window_size(static_cast<GLFWwindow*>(*m_window));

            m_renderer =
              initialize_renderer(p_context,
                                  graphics_api::vulkan,
                                  params,
                                  m_window->current_swapchain().image_size(),
                                  "Renderer");
            m_renderer->set_background_color(p_params.background_color);

            m_ui_context = vulkan::imgui_context(
              p_context->handle(), m_window->current_swapchain(), *m_window);

            // vulkan::instance_context::submit_resource_free([this](){
            //     m_ui_context.destroy();
            // });

            p_context->submit_resource_free(
              [this]() { m_ui_context.destroy(); });

            s_instance = this;
        }

        ~application() { m_window->close(); }

        /**
         * @return the delta time as a float for giving you the timestep every
         * frame
         */
        static float delta_time() { return s_instance->m_delta_time; }

        /**
         * @brief Explicitly is used to execute the application's mainloop
         */
        void execute() {
            console_log_info("Executing game mainloop!!!");

            auto start_time = std::chrono::high_resolution_clock::now();
            m_renderer->preload(
              m_window->current_swapchain().swapchain_renderpass());

            invoke_start();

            // ref<world> current_world = system_registry::get_world("Editor World");
            // ref<scene> current_scene = current_world->get_scene("LevelScene");
            ref<scene> current_scene = m_current_world->get_scene("LevelScene");
            flecs::world current_world_scope = *current_scene;

            /*
                - flecs::system is how your able to schedule changes for given
                portions of data in this case the projection/view matrices are
            only being changed when flecs::world::progress(g_delta_time) is
            being invoked within the mainloop
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
                - in the tag:: namespace, this is to imply components that are
            empty and just represent tags, to specify their uses.
            */
            auto query_camera_objects =
              current_scene
                ->query_builder<flecs::pair<tag::editor, projection_view>,
                                perspective_camera>()
                .build();

            while (m_window->available()) {
                auto current_time = std::chrono::high_resolution_clock::now();
                m_delta_time =
                  std::chrono::duration<float, std::chrono::seconds::period>(
                    current_time - start_time)
                    .count();
                start_time = current_time;

                event::flush_events();

                // Progresses the flecs::world by one tick (or replaced with
                // using the delta time) This also invokes the following
                // system<T...> call  before the mainloop
                current_world_scope.progress(m_delta_time);

                m_current_frame_index = m_window->acquired_next_frame();

                // Current commands that are going to be iterated through
                // Use the acquired swapchain image index for the command buffer
                // and swapchain framebuffer so we record and present the same
                // image.
                ::vk::command_buffer currently_active =
                  m_window->active_command(m_current_frame_index);

                invoke_physics_update();

                invoke_on_update(m_delta_time);

                invoke_defer_update();
                // We want this to be called after late update
                // This queries all camera objects within the camera system
                // Update -- going to be removing camera system in replacement
                // of just simply using flecs::system to keep it simple for the
                // time
                query_camera_objects.each(
                  [&](flecs::entity,
                      flecs::pair<tag::editor, projection_view> p_pair,
                      perspective_camera& p_camera) {
                      if (!p_camera.is_active) {
                          return;
                      }

                      m_proj_view = p_pair->projection * p_pair->view;
                  });

                // invalidate imgui context
                if (m_initial_window_params.width != m_window->data().width and
                    m_initial_window_params.height != m_window->data().height) {
                    m_ui_context.invalidate(m_window->current_swapchain());
                    // once we have invalidated the current width/height, we set
                    // that to its new width/height value post-resizing
                    // TODO: Make this into a event::window_resize event that
                    // can be handled!
                    m_initial_window_params = m_window->data();

                    // Make sure to update the imgui window size
                    ImGui::SetNextWindowSize(ImVec2(
                      static_cast<float>(m_initial_window_params.width),
                      static_cast<float>(m_initial_window_params.height)));
                }

                // Prevents things like stalling so the CPU doesnt have to wait
                // for the GPU to fully complete before starting on the next
                // frame Command buffer uses this to track the frames to process
                // its commands. current_frame = (acquired_next_frame + 1) %
                // frames_in_flight; auto current_frame = (m_current_frame_index
                // + 1) % 2;

                // viewport renderpass to render the 3D screen to the offscreen
                // texture
                auto viewport_framebuffer =
                  m_ui_context.active_framebuffer(m_current_frame_index % 2u);
                m_renderer->begin_frame(
                  currently_active,
                  m_window->current_swapchain().settings(),
                  m_ui_context.viewport_renderpass(),
                  viewport_framebuffer,
                  m_proj_view,
                  m_current_frame_index);

                m_renderer->end_frame();

                m_ui_context.begin(currently_active, m_current_frame_index);

                invoke_ui_update();

                // final renderpass for rendering the offscreen information to
                // the final renderpass
                window_params swapchain_extent =
                  m_window->current_swapchain().settings();
                std::array<float, 4> color = { 0.1f, 0.105f, 0.11f, 1.0f };
                vk::renderpass_begin_params begin_renderpass = {
                    .current_command = currently_active,
                    .extent = { swapchain_extent.width,
                                swapchain_extent.height },
                    .current_framebuffer =
                      m_window->current_swapchain().active_framebuffer(
                        m_current_frame_index),
                    .color = color,
                    .subpass = vk::subpass_contents::inline_bit
                };
                m_window->current_swapchain().swapchain_renderpass().begin(
                  begin_renderpass);
                m_ui_context.end();
                m_window->current_swapchain().swapchain_renderpass().end(
                  currently_active);
                currently_active.end();

                std::array<const VkCommandBuffer, 1> commands = {
                    currently_active,
                };
                m_window->current_swapchain().submit(commands);

                m_window->present(m_current_frame_index);
            }
        }

        /**
         * @brief Performs any post cleanup when user requests the application
         * to close
         */
        void post_destroy() { console_log_info("Executing post cleanup!!!"); }

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
        static graphics_api current_api() { return graphics_api::vulkan; }

        /* Returns the currently selected swapchain */
        /**
         * @brief gives you the current swapchain handle
         *
         * TODO: This is not actually needed, and should be removed
         */
        VkSwapchainKHR get_current_swapchain() {
            return m_window->current_swapchain();
        }


        void current_world(ref<world> p_world) {
            m_current_world = p_world;
        }

    protected:
        [[nodiscard]] ref<renderer_system> renderer_instance() const {
            return m_renderer;
        }

    private:
        float m_delta_time = 0.f;
        // world* m_current_world=nullptr;
        ref<world> m_current_world;
        ref<window> m_window;
        window_params m_initial_window_params;
        // vulkan::instance_context m_instance_handle_test;
        // std::optional<vulkan::instance_context> m_instance_handle_test;
        ref<renderer_system> m_renderer = nullptr;
        glm::mat4 m_proj_view;
        uint32_t m_current_frame_index = -1;
        vulkan::imgui_context m_ui_context;
        static application* s_instance;
    };

    application* application::s_instance = nullptr;
};