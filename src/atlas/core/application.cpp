#include <core/application.hpp>
#include <core/common.hpp>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>

namespace atlas {
    application* application::s_instance = nullptr;
    static api g_graphics_backend_api = api::vulkan;

    application::application(const application_settings& p_settings) {
        console_log_manager::set_current_logger(p_settings.name);
        set_current_api(api::vulkan);
        window_settings settings = {
            .width = p_settings.width,
            .height = p_settings.height,
            .name = p_settings.name,
        };
        m_window = create_window(settings);

        m_renderer =
          create_scope<renderer>(m_window->current_swapchain(), "Renderer");
        m_renderer->set_background_color({ 1.f, 0.5f, 0.5f, 1.f });

        // TODO: Imgui context will need to be refactored
        // to use shared swapchain ref...
        m_ui_context = vk::imgui_context(m_window);
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

    // NOTE: only good for immediate usage,
    //  this will not work for long-term storage due to the likelyhood
    //  of the handle being invalidated
    VkSwapchainKHR application::get_current_swapchain() {
        return m_window->current_swapchain();
    }

    api application::current_api() {
        return g_graphics_backend_api;
    }

    void application::destroy() {
        s_instance->get_window().close();
    }

    float application::delta_time() {
        return s_instance->m_delta_time;
    }

    float application::physics_step() {
        // return g_physics_step;
        return 0.f;
    }

    void application::execute() {
        auto start_time = std::chrono::high_resolution_clock::now();

        detail::invoke_start();

        ref<world_scope> current_world =
          system_registry::get_world("Editor World");
        ref<scene_scope> current_scene = current_world->get_scene("LevelScene");

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
                    transform p_transform,
                    perspective_camera& p_camera) {
              float aspect_ratio = application::aspect_ratio();
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
              // (quaternion)
              glm::quat quaternion = glm::quat({ p_transform.quaternion.w,
                                                 p_transform.quaternion.x,
                                                 p_transform.quaternion.y,
                                                 p_transform.quaternion.z });
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

        while (m_window->available()) {
            auto current_time = std::chrono::high_resolution_clock::now();
            m_delta_time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();
            start_time = current_time;
            event::update_events();

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
            vk::vk_command_buffer currently_active =
              m_window->active_command_buffer(m_current_frame_index);

            detail::invoke_physics_update();

            detail::invoke_on_update();

            detail::invoke_defer_update();

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

                  // Removing this because not needed for now, we can assume the
                  // single viewport is going to be if (p_camera.target ==
                  // screen) {
                  m_proj_view = p_pair->projection * p_pair->view;
                  // }
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

            // renderer would need to share a reference with the windows
            // swapchain otherwise invalidation detection on presenting doesn't
            // get properly propogated to the renderer's swapchain
            // m_window->present(m_current_frame_index);
            m_renderer->present(m_current_frame_index);
        }
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
