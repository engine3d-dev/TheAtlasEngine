#include <drivers/vulkan-cpp/vk_renderer.hpp>
#include <core/engine_logger.hpp>
#include <array>
#include <drivers/vulkan/helper_functions.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <core/application.hpp>

namespace atlas::vk {

    vk_renderer::vk_renderer(const std::string& p_tag) {
        console_log_manager::create_new_logger(p_tag);
        m_driver = vk_context::driver_context();
    }

    vk_renderer::vk_renderer(const vk_swapchain& p_swapchain_handler, const std::string& p_tag) : m_swapchain_handler(p_swapchain_handler) {
        console_log_manager::create_new_logger(p_tag);
        m_driver = vk_context::driver_context();

        // Creating main renderpass
        // m_main_renderpass = create_simple_renderpass(m_driver, m_swapchain_handler.data().surface_format);

        // vk_context::submit_resource_free([this](){
        //     vkDestroyRenderPass(m_driver, m_main_renderpass, nullptr);
        // });
    }

    vk_renderer::~vk_renderer() = default;

    void vk_renderer::background_color(const std::array<float, 4>& p_color) {
        m_color = {{p_color.at(0), p_color.at(1), p_color.at(2), p_color.at(3)}};
    }

    void vk_renderer::start_frame(const vk_command_buffer& p_current, const VkFramebuffer& p_current_fb, const VkRenderPass& p_current_renderpass) {
        std::array<VkClearValue, 2> clear_values = {};

        clear_values[0].color = m_color;
        clear_values[1].depthStencil = {1.f, 0};
        window_settings settings = m_swapchain_handler.settings();

        VkRenderPassBeginInfo renderpass_begin_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.pNext = nullptr,
			.renderPass = p_current_renderpass,
			.renderArea = {
				.offset = {
					.x = 0,
					.y = 0
				},
				.extent = {
					.width = settings.width,
					.height = settings.height
				},
			},
			.clearValueCount = static_cast<uint32_t>(clear_values.size()),
			.pClearValues = clear_values.data()
        };

        m_current_command_buffer = p_current;
        m_current_command_buffer.begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

        VkViewport viewport = {
            .x = 0.0f,
			.y = 0.0f,
			.width = static_cast<float>(settings.width),
			.height = static_cast<float>(settings.height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
        };

        vkCmdSetViewport(m_current_command_buffer, 0, 1, &viewport);

		VkRect2D scissor = {
			.offset = { 0, 0 },
			.extent = {settings.width, settings.height},
		};

		vkCmdSetScissor(m_current_command_buffer, 0, 1, &scissor);

        // renderpass_begin_info.framebuffer = m_swapchain_framebuffers[p_current_frame_idx];
        // renderpass_begin_info.framebuffer = m_swapchain_handler.active_framebuffer(application::current_frame());
        renderpass_begin_info.framebuffer = p_current_fb;

		vkCmdBeginRenderPass(m_current_command_buffer,&renderpass_begin_info,VK_SUBPASS_CONTENTS_INLINE);

    }

    void vk_renderer::post_frame() {
        vkCmdEndRenderPass(m_current_command_buffer);
        m_current_command_buffer.end();

        m_swapchain_handler.submit(m_current_command_buffer);
    }
};