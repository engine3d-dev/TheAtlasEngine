#include <core/application.hpp>
#include <core/engine_logger.hpp>
#include <core/event/event.hpp>
#include <imgui.h>
#include <string>

#include <core/utilities/state.hpp>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>

#include <core/system_framework/system_registry.hpp>
#include <core/system/camera.hpp>

namespace atlas {
    static std::string g_tag = "TheAtlasEngine";
    application* application::s_instance = nullptr;
    static api g_graphics_backend_api = api::vulkan;
    static float g_delta_time = 0.f;
    static float g_physics_step = 0.f; // collision step

    application::application(const application_settings& p_settings) {
        g_tag = p_settings.Name;
        console_log_manager::set_current_logger(g_tag);
        set_current_api(api::vulkan);
        window_settings settings = {
            .width = p_settings.Width,
            .height = p_settings.Height,
            .name = p_settings.Name,
        };
        m_window = create_window(settings);

        m_renderer =
          create_scope<renderer>(m_window->current_swapchain(), "Renderer");
        m_renderer->set_background_color({ 1.f, 0.5f, 0.5f, 1.f });

        m_ui_context = vk::imgui_context(
          *m_window,
          m_window->current_swapchain(),
          m_window->current_swapchain().swapchain_renderpass());
        s_instance = this;
    }

    uint32_t application::current_frame() {
        return s_instance->m_current_frame_index;
    }

    application::~application() {
        destroy();
    }

    void application::set_current_api(api api) {
        g_graphics_backend_api = api;
    }

    VkSwapchainKHR application::get_current_swapchain() {
        return get_window().current_swapchain();
    }

    api application::current_api() {
        return g_graphics_backend_api;
    }

    void application::destroy() {
        s_instance->get_window().close();
    }

    float application::delta_time() {
        return g_delta_time;
    }

    float application::physics_step() {
        return g_physics_step;
    }

    void application::execute() {
        float previous_time = 0.f;

        detail::invoke_start();

        float aspect_ratio = application::aspect_ratio();
        ref<world_scope> current_world =
          system_registry::get_world("Editor World");
        ref<scene_scope> current_scene = current_world->get_scene("LevelScene");

        // This query is for scheduling what viewports the cameras are
        // associated with This means that if we had multiple viewports support,
        // we could assign a camera to those specific viewports and vice versa
        // Assigning one camera to single viewport
        // TODO: Centralizing the responsibility of `camera_system` and this
        // query_builder to contain a centralized operation for both transform
        // of camera updates and scheduling the cameras
        /*
        m_query_camera_objects =
          current_scene
            ->query_builder<projection_view,
                            perspective_camera>().with<flecs::pair<atlas::tag_redo::editor,
        atlas::projection_view>>() .build();
        */

        flecs::world current_world_scope = *current_scene;

        // flecs::system is how your able to schedule changes for given portions
        // of data in this case the projection/view matrices are onyl being
        // changed when flecs::world::progress(g_delta_time) is being invoked
        // within the mainloop
        current_world_scope
          .system<projection_view, transform, perspective_camera>()
          .each([&, aspect_ratio](projection_view& p_proj_view,
                                  transform p_transform,
                                  const perspective_camera& p_camera) {
              if (!p_camera.is_active) {
                  return;
              }

              p_proj_view.projection = glm::mat4(1.f);

              p_proj_view.projection =
                glm::perspective(glm::radians(p_camera.field_of_view),
                                 aspect_ratio,
                                 p_camera.plane.x,
                                 p_camera.plane.y);

              p_proj_view.view = glm::mat4(1.f);
              glm::quat quaternion = glm::quat({ p_transform.quaternion.w,
                                                 p_transform.quaternion.x,
                                                 p_transform.quaternion.y,
                                                 p_transform.quaternion.z });
              p_proj_view.view =
                glm::translate(p_proj_view.view, p_transform.position) *
                glm::toMat4(quaternion);
              p_proj_view.view = glm::inverse(p_proj_view.view);
          });

        // This is querying
        auto query_camera_objects =
          current_scene
            ->query_builder<flecs::pair<tag_redo::editor, projection_view>,
                            perspective_camera>()
            .build();

        while (m_window->available()) {
            float current_time = (float)glfwGetTime();
            g_delta_time = (current_time - previous_time);
            previous_time = current_time;
            event::update_events();

            // Progresses the flecs::world by one tick (or replaced with using
            // the delta time)
            // This also invokes the following system<T...> call  before the
            // mainloop
            current_world_scope.progress(g_delta_time);

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
            vk::vk_command_buffer currently_active =
              m_window->active_command_buffer(m_current_frame_index);

            detail::invoke_physics_update();

            detail::invoke_on_update();

            detail::invoke_defer_update();

            // We want this to be called after late update
            // This queries all camera objects within the camera system
            // Update -- going to be removing camera system in replacement of
            // just simply using flecs::system to keep it simple for the time
            // being m_main_camera_system.query_camera_entities();

            query_camera_objects.each(
              [&](flecs::entity,
                  flecs::pair<tag_redo::editor, projection_view> p_pair,
                  perspective_camera& p_camera) {
                  if (!p_camera.is_active) {
                      return;
                  }

                  if (p_camera.target == screen) {
                      m_proj_view = p_pair->projection * p_pair->view;
                  }
            });

            // TODO: Introduce scene renderer that will make use of the
            // begin/end semantics for setting up tasks during pre-frame
            // operations
            // renderer begin to indicate when a start of the frame to start
            // processing specific tasks that either need to be computed or
            // pre-defined before the renderer does something with it.
            // TODO: Add scene_manager to assist on what things to be processing
            // before the frame preparation
            m_renderer->begin(
              currently_active, m_window->current_swapchain(), m_proj_view);

            // TODO: UI will have its own renderpass, command buffers, and
            // framebuffers specifically for UI-widgets
            m_ui_context.begin(currently_active, m_current_frame_index);

            detail::invoke_ui_update();

            m_ui_context.end();

            m_renderer->end();

            m_window->present(m_current_frame_index);
        }

        // m_query_camera_objects.destruct();
    }

    void application::post_destroy() {
        m_window->close();
    }

    float application::aspect_ratio() {
        return s_instance->m_window->aspect_ratio();
    }

    uint32_t application::image_size() {
        return s_instance->m_window->current_swapchain().image_size();
    }
};