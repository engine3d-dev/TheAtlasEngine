#include <drivers/vulkan-cpp/vk_renderer.hpp>
#include <core/engine_logger.hpp>
#include <array>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <core/application.hpp>

#include <glm/glm.hpp>

namespace atlas::vk {

    vk_renderer::vk_renderer(const vk_swapchain& p_swapchain, const std::string& p_tag){
        console_log_manager::create_new_logger(p_tag);
        m_driver = vk_context::driver_context();
        console_log_info("vk_renderer Begin construction!!!");
        m_main_swapchain = p_swapchain;
        m_image_count = m_main_swapchain.image_size();

        m_shader_group = vk_shader_group({
            {"experimental-shaders/test.vert", shader_stage::Vertex},
            {"experimental-shaders/test.frag", shader_stage::Fragment}
        });

        m_shader_group.set_vertex_attributes({
            {.location = 0, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(vk::vertex, position)},
		{.location = 1, .binding = 0, .format = VK_FORMAT_R32G32B32A32_SFLOAT, .offset = offsetof(vk::vertex, color)},
        {.location = 2, .binding = 0, .format = VK_FORMAT_R32G32B32A32_SFLOAT, .offset = offsetof(vk::vertex, normals)},
		{.location = 3, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = offsetof(vk::vertex, uv)}
        });

        m_shader_group.set_vertex_bind_attributes({
            {.binding = 0, .stride = sizeof(vk::vertex), .inputRate = VK_VERTEX_INPUT_RATE_VERTEX}
        });

        console_log_info("vk_renderer End construction!!!");

        console_log_info("image_size = {}", m_image_count);

        m_global_uniforms = std::vector<vk_uniform_buffer>(p_swapchain.image_size());

        for(size_t i = 0; i < m_global_uniforms.size(); i++) {
            m_global_uniforms[i] = vk_uniform_buffer(sizeof(camera_ubo));
        }

        m_descriptor_set_test = descriptor_set(m_image_count, {
            {.binding = 0, .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = 1, .stageFlags = VK_SHADER_STAGE_VERTEX_BIT, .pImmutableSamplers  = nullptr},
		{.binding = 1, .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = 1, .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT, .pImmutableSamplers  = nullptr}
        });

        m_main_pipeline = vk_pipeline(m_main_swapchain.swapchain_renderpass(), m_shader_group, m_descriptor_set_test.get_layout());


        // m_test_texture = texture(std::filesystem::path("assets/models/viking_room.png"));

        // if(m_test_texture.loaded()) {
        //     console_log_info("viking_room.png is loaded!!!");
        // }
        // else {
        //     console_log_info("viking_room.png not loaded!!!");
        // }

        m_test_mesh = mesh("assets/models/viking_room.obj");
        m_test_mesh.set_texture(0, "assets/models/viking_room.png");

        m_descriptor_set_test.update_test_descriptors(m_global_uniforms, m_test_mesh.get_texture(0));


        vk_context::submit_resource_free([this](){
            m_shader_group.destroy();
            m_descriptor_set_test.destroy();
            m_main_pipeline.destroy();
            m_test_mesh.destroy();

            for(size_t i = 0; i < m_global_uniforms.size(); i++) {
                m_global_uniforms[i].destroy();
            }
        });
    }

    vk_renderer::~vk_renderer() = default;

    void vk_renderer::background_color(const std::array<float, 4>& p_color) {
        m_color = {{p_color.at(0), p_color.at(1), p_color.at(2), p_color.at(3)}};
    }

    void vk_renderer::start_frame(const vk_command_buffer& p_current, const vk::vk_swapchain& p_swapchain_handler) {
        m_main_swapchain = p_swapchain_handler;
        // m_main_pipeline = vk_pipeline(p_swapchain_handler.swapchain_renderpass(), m_shader_group);
        uint32_t current_frame = application::current_frame();

        std::array<VkClearValue, 2> clear_values = {};

        clear_values[0].color = m_color;
        clear_values[1].depthStencil = {1.f, 0};
        window_settings settings = m_main_swapchain.settings();

        VkRenderPassBeginInfo renderpass_begin_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.pNext = nullptr,
			.renderPass = m_main_swapchain.swapchain_renderpass(),
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
        renderpass_begin_info.framebuffer = m_main_swapchain.active_framebuffer(current_frame);

		vkCmdBeginRenderPass(m_current_command_buffer,&renderpass_begin_info,VK_SUBPASS_CONTENTS_INLINE);

        // camera uniforms
        static auto start_time = std::chrono::high_resolution_clock::now();

        auto current_time = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

        camera_ubo ubo{};
        ubo.Model = glm::translate(ubo.Model, glm::vec3(0.f, 0.f, 0.f));
        ubo.Model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.Projection = glm::perspective(glm::radians(45.0f), (float)settings.width / (float) settings.height, 0.1f, 10.0f);
        ubo.Projection[1][1] *= -1;

        glm::mat4 mvp = ubo.Projection * ubo.View * ubo.Model;
        m_global_uniforms[current_frame].update(&mvp, sizeof(mvp));

        // Draw stuff
        m_main_pipeline.bind(p_current);
        m_descriptor_set_test.bind(p_current, current_frame, m_main_pipeline.get_layout());
        m_test_mesh.draw(p_current);

    }

    void vk_renderer::post_frame() {
        vkCmdEndRenderPass(m_current_command_buffer);
        m_current_command_buffer.end();

        m_main_swapchain.submit(m_current_command_buffer);
    }
};