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

#include <array>
#include <memory>
#include <memory_resource>
#include <optional>
#include <span>

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
            m_extent = {
                .width = p_params.width,
                .height = p_params.height,
            };
            // m_window = std::allocate_shared<window>(, m_context->instance_handle(), params);
            m_window = std::make_shared<window>(p_context, params);

            // Requesting depth format
            std::array<vk::format, 3> format_support = {
                vk::format::d32_sfloat,
                vk::format::d32_sfloat_s8_uint,
                vk::format::d24_unorm_s8_uint
            };

            // We provide a selection of format support that we want to check is
            // supported on current hardware device.
            m_depth_format = m_physical->request_depth_format(format_support);

            // Initializing command buffers
            std::span<const VkImage> images = m_window->request_images();

            m_images.resize(images.size());
            m_depth_images.resize(images.size());

            for(uint32_t i = 0; i < m_images.size(); i++) {
                vk::image_params color_img_params = {
                    .extent = {
                        .width = p_params.width,
                        .height = p_params.height,
                    },
                    .format = m_window->surface_properties().format.format,
                    .memory_mask = m_physical->memory_properties(
                    vk::memory_property::device_local_bit),
                    .aspect = vk::image_aspect_flags::color_bit,
                    .usage = vk::image_usage::color_attachment_bit,
                    .mip_levels = 1,
                    .layer_count = 1,
                };
                m_images[i] = vk::sample_image(*m_device, images[i], color_img_params);

                vk::image_params depth_img_params = {
                    .extent = {
                        .width = p_params.width,
                        .height = p_params.height,
                    },
                    .format = m_depth_format,
                    .memory_mask = m_physical->memory_properties(
                    vk::memory_property::device_local_bit),
                    .aspect = vk::image_aspect_flags::depth_bit,
                    .usage = vk::image_usage::depth_stencil_bit,
                    .mip_levels = 1,
                    .layer_count = 1,
                };

                m_depth_images[i] = vk::sample_image(*m_device, depth_img_params);
            }

            m_command_buffers.resize(images.size());

            for(uint32_t i = 0; i < m_command_buffers.size(); i++) {
                vk::command_params command_params = {
                    .levels = vk::command_levels::primary,
                    .queue_index = 0,
                    .flags = vk::command_pool_flags::reset,
                };
                m_command_buffers[i] = vk::command_buffer(*m_device, command_params);
            }

            std::println("images.size() = {}", images.size());

            std::println("After constructing atlas::window");
        }

        void execute() {
            std::println("Executing mainloop");

            VkClearValue clear_color = {
                { 0.f, 0.5f, 0.5f, 1.f },
            };

            VkClearValue depth_value = {
                .depthStencil = { .depth = 1.f, .stencil = 0 },
            };

            while(m_window->available()) {
                event::flush_events();

                m_next_image_frame_idx = m_window->acquire_next_frame();
                const auto current_extent = m_window->surface_properties().capabilities.currentExtent;

                vk::command_buffer current = m_command_buffers[m_next_image_frame_idx];

                current.begin(vk::command_usage::simulatneous_use_bit);

                m_images[m_next_image_frame_idx].memory_barrier(
                    current,
                    m_window->surface_properties().format.format,
                    VK_IMAGE_LAYOUT_UNDEFINED,
                    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
                
                m_depth_images[m_next_image_frame_idx].memory_barrier(
                    current,
                    m_depth_format,
                    VK_IMAGE_LAYOUT_UNDEFINED,
                    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                    VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT);
                
                vk::rendering_attachment color_render_attachment = {
                    .image_view = m_images[m_next_image_frame_idx].image_view(),
                    .layout = vk::image_layout::color_optimal,
                    .resolve_mode = vk::resolved_mode_flags::none,
                    .resolve_image_view = nullptr,
                    .resolve_image_layout = vk::image_layout::undefined,
                    .load = vk::attachment_load::clear,
                    .store = vk::attachment_store::store,
                    .clear_values = clear_color
                };

                vk::rendering_attachment depth_stencil_attachment = {
                    .image_view = m_depth_images[m_next_image_frame_idx].image_view(),
                    .layout = vk::image_layout::depth_stencil_optimal,
                    .resolve_mode = vk::resolved_mode_flags::none,
                    .resolve_image_view = nullptr,
                    .resolve_image_layout = vk::image_layout::undefined,
                    .load = vk::attachment_load::clear,
                    .store = vk::attachment_store::store,
                    .depth_values = depth_value
                };

                vk::rendering_begin_parameters begin_params = {
                    .render_area = { { 0, 0 },
                                    {
                                    current_extent.width,
                                    current_extent.height,
                                    }, },
                    .layer_count = 1,
                    .color_attachments = std::span<const vk::rendering_attachment>(
                    &color_render_attachment, 1),
                    .depth_attachment = depth_stencil_attachment,
                    .stencil_attachment = depth_stencil_attachment,
                };

                vk::viewport_params viewport = {
                    .x = 0.0f,
                    .y = 0.0f,
                    .width = static_cast<float>(current_extent.width),
                    .height = static_cast<float>(current_extent.height),
                    .min_depth = 0.0f,
                    .max_depth = 1.0f,
                };
                current.set_viewport(0, 1, std::span<const vk::viewport_params>(&viewport, 1));

                vk::scissor_params scissor = {
                    .offset = { 0, 0 },
                    .extent = current_extent,
                };

                current.set_scissor(0, 1, std::span<const vk::scissor_params>(&scissor, 1));

                current.begin_rendering(begin_params);

                // Do rendering stuff...

                current.end_rendering();

                m_images[m_next_image_frame_idx].memory_barrier(
                    current,
                    m_window->surface_properties().format.format,
                    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
                current.end();

                std::array<const VkCommandBuffer, 1> commands = {current};
                m_window->submit(commands);
                m_window->present(m_next_image_frame_idx);
            }
        }

        void post_destroy() {
            std::println("Post destroy!");

            for(auto& command : m_command_buffers) {
                command.destruct();
            }

            for(auto& color_image : m_images) {
                color_image.destruct();
            }

            for(auto& depth_image : m_depth_images) {
                depth_image.destruct();
            }
            m_window->destruct();
            m_device->destruct();
        }

    private:
        uint32_t m_next_image_frame_idx=0;
        VkFormat m_depth_format;
        vk::instance m_instance;
        std::optional<vk::physical_device> m_physical;
        std::shared_ptr<vk::device> m_device;
        std::shared_ptr<graphics_context> m_context;
        std::shared_ptr<window> m_window=nullptr;
        vk::image_extent m_extent{};
        std::vector<vk::sample_image> m_images;
        std::vector<vk::sample_image> m_depth_images;
        std::vector<vk::command_buffer> m_command_buffers;
        event::bus* m_bus = nullptr;
        static application* s_instance;
    };

    application* application::s_instance = nullptr;
};