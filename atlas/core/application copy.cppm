module;

#include <cstdint>
#include <string>
#include <print>
#include <chrono>
#include <utility>

#include <array>
#include <memory>
#include <memory_resource>
#include <optional>
#include <span>

#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <flecs.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <vulkan/vulkan.h>

export module atlas.application;

import atlas.core.utilities;
import atlas.core.utilities.poll_state;
import atlas.core.event;
import vk;
import atlas.drivers.vulkan;

import atlas.core.scene;
import atlas.core.scene.world;
import atlas.core.scene.components;
import atlas.core.math;

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

    class application {
    public:
        application() = default;
        application(/*NOLINT*/ std::shared_ptr<graphics_context> p_context,
                    const application_settings& p_params,
                    event::bus& p_bus)
          : m_context(p_context)
          , m_bus(&p_bus) {
            m_instance = p_context->instance_handle();
            m_physical = p_context->physical_device();
            m_device = p_context->logical_device();

            // Constructing the application
            window_params params = {
                .width = p_params.width,
                .height = p_params.height,
                .name = p_params.name,
            };
            
            // m_window = std::allocate_shared<window>(,
            // m_context->instance_handle(), params);
            m_window = std::make_shared<window>(p_context, params);

            m_aspect_ratio = static_cast<float>(params.width) / static_cast<float>(params.height);
            event::set_window_size(m_window->glfw_window());

            m_bus->create_listener<atlas::event::collision_enter>();
            m_bus->create_listener<atlas::event::collision_persisted>();
            m_bus->create_listener<atlas::event::collision_exit>();
            // m_bus->create_immediate_listener<atlas::event::scene_transition>();
            // m_bus->create_immediate_listener<atlas::event::mesh_reload>();
            // m_bus->create_immediate_listener<atlas::event::material_reload>();

            // Requesting depth format
            std::array<vk::format, 3> format_support = {
                vk::format::d32_sfloat,
                vk::format::d32_sfloat_s8_uint,
                vk::format::d24_unorm_s8_uint
            };

            m_color_format = m_window->surface_properties().format.format;
            // We provide a selection of format support that we want to check is
            // supported on current hardware device.
            m_depth_format = m_physical->request_depth_format(format_support);

            m_imgui_context = std::make_shared<imgui_context>(p_context,
                                m_window->glfw_window(),
                                m_window->swapchain_handle(),
                                m_window->request_images().size(),
                                m_window->present_queue(),
                                m_color_format,
                                m_depth_format,
                                params);

            // Initializing command buffers
            std::span<const VkImage> images = m_window->request_images();

            m_images.resize(images.size());
            m_depth_images.resize(images.size());

            for (uint32_t i = 0; i < m_images.size(); i++) {
                vk::image_params color_img_params = {
                    .extent = {
                        .width = m_window->extent().width,
                        .height = m_window->extent().height,
                    },
                    .format = m_window->surface_properties().format.format,
                    .memory_mask = m_physical->memory_properties(
                    vk::memory_property::device_local_bit),
                    .aspect = vk::image_aspect_flags::color_bit,
                    .usage = vk::image_usage::color_attachment_bit,
                    .mip_levels = 1,
                    .layer_count = 1,
                };
                m_images[i] =
                  vk::sample_image(*m_device, images[i], color_img_params);

                vk::image_params depth_img_params = {
                    .extent = {
                        .width = m_window->extent().width,
                        .height = m_window->extent().height,
                    },
                    .format = m_depth_format,
                    .memory_mask = m_physical->memory_properties(
                    vk::memory_property::device_local_bit),
                    .aspect = vk::image_aspect_flags::depth_bit,
                    .usage = vk::image_usage::depth_stencil_bit,
                    .mip_levels = 1,
                    .layer_count = 1,
                };

                m_depth_images[i] =
                  vk::sample_image(*m_device, depth_img_params);
            }

            m_command_buffers.resize(images.size());

            for (uint32_t i = 0; i < m_command_buffers.size(); i++) {
                vk::command_params command_params = {
                    .levels = vk::command_levels::primary,
                    .queue_index = 0,
                    .flags = vk::command_pool_flags::reset,
                };
                m_command_buffers[i] =
                  vk::command_buffer(*m_device, command_params);
            }

            m_render_context =
              render_context(m_context, m_color_format, m_depth_format);

            std::println("images.size() = {}", images.size());

            m_window->center_window();

            // m_bus->trigger<event::scene_transition>(this,
            // &application::on_scene_transition);
        }

        void execute() {
            VkClearValue clear_color = {
                { { 0.f, 0.5f, 0.5f, 1.f } },
            };

            VkClearValue depth_value = {
                .depthStencil = { .depth = 1.f, .stencil = 0 },
            };

            m_current_scene = m_world->current();

            // Handling camera system execution
            m_current_scene
              ->system<flecs::pair<tag::editor, projection_view>,
                       transform,
                       perspective_camera>()
              .each([this](flecs::pair<tag::editor, projection_view> p_pair,
                        transform& p_transform,
                        perspective_camera& p_camera) {
                  if (!p_camera.is_active) {
                      return;
                  }

                  p_pair->projection = glm::mat4(1.f);

                  p_pair->projection =
                    glm::perspective(glm::radians(p_camera.field_of_view),
                                     m_aspect_ratio,
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

            // Setting the current scene for the renderer to start rendering the
            // objects
            m_render_context.current_scene(*m_current_scene);

            auto start_time = std::chrono::high_resolution_clock::now();
            invoke_start(m_current_scene.get());

            // Querying editor cameras specific objects
            // Then using this to execute specific main cameras.
            auto query_camera_objects =
              m_current_scene
                ->query_builder<flecs::pair<tag::editor, projection_view>,
                                perspective_camera>()
                .build();

            m_render_context.prebake();

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
                m_current_scene->progress(m_delta_time);

                m_next_image_frame_idx = m_window->acquire_next_frame();

                vk::command_buffer current =
                  m_command_buffers[m_next_image_frame_idx];

                invoke_physics_update(m_current_scene.get());

                invoke_on_update(m_current_scene.get(), m_delta_time);

                // We want this to be called after late update
                // This queries all camera objects within the camera system
                // TODO: Should consider changing this from
                // using tags in flecs for specifying active cameras.
                query_camera_objects.each(
                  [&](flecs::entity,
                      flecs::pair<tag::editor, projection_view> p_pair,
                      perspective_camera& p_camera) {
                      if (!p_camera.is_active) {
                          return;
                      }

                      m_projection = p_pair->projection;
                      m_view = p_pair->view;
                  });

                current.begin(vk::command_usage::simulatneous_use_bit);

                m_render_context.set_command(current);

                m_imgui_context->set_current_command(current);

                /**
                 * NOTE: For getting imgui_context
                 * 
                 * Using viewport images instead of swapchain images.
                 * 
                 * These swapchain images will be used as soon the 3D scene is rendered to the imgui viewport images.
                */

                // m_images[m_next_image_frame_idx].memory_barrier(
                m_imgui_context->image_memory_barrier(
                  current,
                  m_color_format,
                  VK_IMAGE_LAYOUT_UNDEFINED, 
                  VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
                  VK_IMAGE_ASPECT_COLOR_BIT);

                // m_depth_images[m_next_image_frame_idx].memory_barrier(
                m_imgui_context->depth_image_memory_barrier(
                  current,
                  m_depth_format,
                  VK_IMAGE_LAYOUT_UNDEFINED,
                  VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                  VK_IMAGE_ASPECT_DEPTH_BIT);

                vk::rendering_attachment ui_color_render_attachment = {
                    .image_view = m_imgui_context->color_image_view(),
                    .layout = vk::image_layout::color_optimal,
                    .resolve_mode = vk::resolved_mode_flags::none,
                    .resolve_image_view = nullptr,
                    .resolve_image_layout = vk::image_layout::undefined,
                    .load = vk::attachment_load::clear,
                    .store = vk::attachment_store::store,
                    .clear_values = clear_color
                };

                vk::rendering_attachment ui_depth_stencil_attachment = {
                    .image_view = m_imgui_context->depth_image_view(),
                    .layout = vk::image_layout::depth_stencil_optimal,
                    .resolve_mode = vk::resolved_mode_flags::none,
                    .resolve_image_view = nullptr,
                    .resolve_image_layout = vk::image_layout::undefined,
                    .load = vk::attachment_load::clear,
                    .store = vk::attachment_store::store,
                    .depth_values = depth_value
                };

                // presenting rendering attachments
                vk::rendering_attachment present_color_render_attachment = {
                    .image_view = m_images[m_next_image_frame_idx].image_view(),
                    .layout = vk::image_layout::color_optimal,
                    .resolve_mode = vk::resolved_mode_flags::none,
                    .resolve_image_view = nullptr,
                    .resolve_image_layout = vk::image_layout::undefined,
                    .load = vk::attachment_load::clear,
                    .store = vk::attachment_store::store,
                    .clear_values = clear_color
                };

                vk::rendering_attachment present_depth_stencil_attachment = {
                    .image_view =
                      m_depth_images[m_next_image_frame_idx].image_view(),
                    .layout = vk::image_layout::depth_stencil_optimal,
                    .resolve_mode = vk::resolved_mode_flags::none,
                    .resolve_image_view = nullptr,
                    .resolve_image_layout = vk::image_layout::undefined,
                    .load = vk::attachment_load::clear,
                    .store = vk::attachment_store::store,
                    .depth_values = depth_value
                };

                vk::rendering_begin_parameters ui_begin_params = {
                    .render_area = { { 0, 0 }, { m_window->extent().width, m_window->extent().height }, },
                    .layer_count = 1,
                    .color_attachments = std::span<const vk::rendering_attachment>(
                    &ui_color_render_attachment, 1),
                    .depth_attachment = ui_depth_stencil_attachment,
                    .stencil_attachment = ui_depth_stencil_attachment,
                };

                vk::viewport_params viewport = {
                    .x = 0.0f,
                    .y = 0.0f,
                    .width = static_cast<float>(m_window->extent().width),
                    .height = static_cast<float>(m_window->extent().height),
                    .min_depth = 0.0f,
                    .max_depth = 1.0f,
                };
                current.set_viewport(0, 1, std::span<const vk::viewport_params>(&viewport, 1));

                vk::scissor_params scissor = {
                    .offset = { 0, 0 },
                    .extent = {static_cast<uint32_t>(m_window->extent().width), static_cast<uint32_t>(m_window->extent().height)},
                };
                current.set_scissor(0, 1, std::span<const vk::scissor_params>(&scissor, 1));

                current.begin_rendering(ui_begin_params);

                m_render_context.begin(m_projection, m_view);
                
                m_render_context.end();
                current.end_rendering();

                m_imgui_context->image_memory_barrier(
                    current,
                    m_color_format,
                    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
                    VK_IMAGE_ASPECT_COLOR_BIT
                );

                // Performing Present Context Rendering

                m_images[m_next_image_frame_idx].memory_barrier(
                  current,
                //   m_window->surface_properties().format.format,
                m_color_format,
                  VK_IMAGE_LAYOUT_UNDEFINED,
                  VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

                m_depth_images[m_next_image_frame_idx].memory_barrier(
                  current,
                  m_depth_format,
                  VK_IMAGE_LAYOUT_UNDEFINED,
                  VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                  VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT);

                vk::rendering_begin_parameters begin_params = {
                    .render_area = { { 0, 0 }, { m_window->extent().width, m_window->extent().height }, },
                    .layer_count = 1,
                    .color_attachments = std::span<const vk::rendering_attachment>(
                    &present_color_render_attachment, 1),
                    .depth_attachment = present_depth_stencil_attachment,
                    .stencil_attachment = present_depth_stencil_attachment,
                };
                current.begin_rendering(begin_params);
                m_imgui_context->begin();
                invoke_ui_update(m_current_scene.get());
                m_imgui_context->end();

                current.end_rendering();
                m_images[m_next_image_frame_idx].memory_barrier(
                  current,
                  m_window->surface_properties().format.format,
                  VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                  VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

                // m_images[m_next_image_frame_idx].memory_barrier(
                current.end();

                const VkCommandBuffer command = current;
                m_window->submit(std::span<const VkCommandBuffer>(&command, 1));
                m_window->present(m_next_image_frame_idx);
            }
        }

        // Experimental: Will look into later once we dive into scene
        // transitioning void on_scene_transition(event::scene_transition&
        // p_scene_transition) {
        //     m_world->current(p_scene_transition.next_scene);
        //     std::println("Attempting to switch to the scene: {}",
        //     p_scene_transition.next_scene);

        //     // We only want to set the current scene if that specific scene
        //     is valid if(m_world->current() != nullptr) {
        //         m_current_scene = m_world->current();
        //         m_render_context.current_scene(*m_current_scene);
        //     }
        // }

        void post_destroy() {
            m_imgui_context->destruct();
            m_render_context.destruct();

            for (auto& command : m_command_buffers) {
                command.destruct();
            }

            for (auto& color_image : m_images) {
                color_image.destruct();
            }

            for (auto& depth_image : m_depth_images) {
                depth_image.destruct();
            }
            m_window->destruct();
            m_device->destruct();
        }

        void current_world(std::shared_ptr<world> p_world) {
            m_world = std::move(p_world);
        }

    private:
        float m_aspect_ratio=0.f;
        uint32_t m_next_image_frame_idx = 0;
        VkFormat m_color_format;
        VkFormat m_depth_format;
        std::shared_ptr<graphics_context> m_context;
        std::shared_ptr<window> m_window = nullptr;
        event::bus* m_bus = nullptr;
        glm::mat4 m_view = glm::mat4(1.f);
        glm::mat4 m_projection = glm::mat4(1.f);

        render_context m_render_context;

        std::shared_ptr<world> m_world;

        std::shared_ptr<imgui_context> m_imgui_context;
        std::shared_ptr<scene> m_current_scene = nullptr;

        // vulkan-cpp specific handles
        vk::instance m_instance;
        std::optional<vk::physical_device> m_physical;
        std::shared_ptr<vk::device> m_device;
        std::vector<vk::sample_image> m_images;
        std::vector<vk::sample_image> m_depth_images;
        std::vector<vk::command_buffer> m_command_buffers;
        float m_delta_time = 0.f;
        static application* s_instance;
    };

    application* application::s_instance = nullptr;
};