#include "drivers/vulkan/helper_functions.hpp"
#include "drivers/vulkan/vulkan_renderer.hpp"
#include "engine_logger.hpp"
#include <drivers/ui/imgui_backend.hpp>
#include <core/application_instance.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace engine3d{
	static std::vector<VkCommandBuffer> s_ImGuiCommandBuffers;
    void ImGuiBackend::Initialize(){
        ConsoleLogInfo("ImGui Vulkan Test: Begin Initialization!");
        //! @note Setting up imgui stuff.
        // Setup Dear ImGui context
		IMGUI_CHECKVERSION();

		ConsoleLogWarn("After setting ImGuiStyle and checking viewports enabled flags!");

		// 1: create descriptor pool for IMGUI
		//  the size of the pool is very oversize, but it's copied from imgui demo
		//  itself.
		VkDescriptorPoolSize pool_sizes[] = { { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo desc_pool_create_info = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
			.maxSets = 1000,
			.poolSizeCount = (uint32_t)std::size(pool_sizes),
			.pPoolSizes = pool_sizes
		};

		VkDescriptorPool imgui_pool;
		vk::vk_check(vkCreateDescriptorPool(vk::VulkanContext::GetDriver(), &desc_pool_create_info, nullptr, &imgui_pool), "vkCreateDescriptorPool", __FILE__, __LINE__, __FUNCTION__);
		ConsoleLogInfo("After creating descriptor sets for IMGUI");

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		// Setup Dear ImGui style

        ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// this initializes imgui for Vulkan
		// ImGui_ImplGlfw_InitForVulkan(ApplicationInstance::GetWindow().GetNativeWindow(), true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = vk::VulkanContext::GetVkInstance();
		init_info.PhysicalDevice = vk::VulkanContext::GetPhysicalDriver();
		init_info.Device = vk::VulkanContext::GetDriver();
		init_info.Queue = vk::VulkanContext::GetDriver().GetGraphicsQueue();
		init_info.RenderPass = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetRenderPass();
		init_info.DescriptorPool = imgui_pool;
		init_info.MinImageCount = 2;
		init_info.ImageCount = 2;
		init_info.UseDynamicRendering = false;

		//dynamic rendering parameters for imgui to use
		init_info.PipelineRenderingCreateInfo = { .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO };
		init_info.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
		init_info.PipelineRenderingCreateInfo.pColorAttachmentFormats = (const VkFormat*)ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetSwapchainFormat();

		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		ConsoleLogWarn("After setting ImGui_ImplVulkan_InitInfo!");

		ImGui_ImplVulkan_Init(&init_info);
		ConsoleLogWarn("ImGui_ImplVulkan_Init afterwards!");
		ImGui_ImplGlfw_InitForVulkan(ApplicationInstance::GetWindow().GetNativeWindow(), true);
		ConsoleLogWarn("After ImGui_ImplGlfw_InitForVulkan called!");
		ImGui_ImplVulkan_CreateFontsTexture();
	}

    void ImGuiBackend::Begin(){
		// ConsoleLogInfo("Begin() from imgui backend called!");
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

    void ImGuiBackend::End(){
		ImGui::Render();

		auto swapchain = ApplicationInstance::GetWindow().GetCurrentSwapchain();
		VkClearValue clearValues[2];
		clearValues[0].color = { {0.1f, 0.1f,0.1f, 1.0f} };
		clearValues[1].depthStencil = { 1.0f, 0 };

		uint32_t current_command_buffer_inedx = vk::VulkanRenderer::GetCurrentCommandBufferIndex();

		uint32_t width = ApplicationInstance::GetWindow().GetWidth();
		uint32_t height = ApplicationInstance::GetWindow().GetHeight();

		VkCommandBufferBeginInfo imgui_draw_command_buffer_info = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = 0
		};

		auto current_cmd_buffer = vk::VulkanRenderer::CurrentCommandBuffer();
		vk::vk_check(vkBeginCommandBuffer(current_cmd_buffer, &imgui_draw_command_buffer_info), "vkBeginCommandBuffer", __FILE__, __LINE__, __FUNCTION__);

		VkRenderPassBeginInfo imgui_render_pass_begin_info = {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.pNext  =nullptr,
			.renderPass = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetRenderPass(),
			.framebuffer = vk::VulkanRenderer::CurrentFramebuffer(),
			.renderArea = {
				.offset = {0, 0},
				.extent = {width, height},
			},
			.clearValueCount = 2,
			.pClearValues = clearValues,
		};

		vkCmdBeginRenderPass(current_cmd_buffer, &imgui_render_pass_begin_info, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

		VkCommandBufferInheritanceInfo imgui_cmd_buffer_inheritance_info = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
			.pNext = nullptr,
			.renderPass = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetRenderPass(),
			.framebuffer = vk::VulkanRenderer::CurrentFramebuffer()
		};

		VkCommandBufferBeginInfo imgui_command_buffer_begin_info = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
			.pInheritanceInfo = &imgui_cmd_buffer_inheritance_info
		};

		vk::vk_check(vkBeginCommandBuffer(s_ImGuiCommandBuffers[current_command_buffer_inedx], &imgui_command_buffer_begin_info), "vkBeginCommandBuffer", __FILE__, __LINE__, __FUNCTION__);

		VkViewport viewport = {
			.x = 0.0f,
			.y = (float)height,
			.width = (float)width,
			.height = -(float)height,
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
		};

		vkCmdSetViewport(s_ImGuiCommandBuffers[current_command_buffer_inedx], 0, 1, &viewport);

		VkRect2D scissor = {
			.offset = {0, 0},
			.extent = {width, height},
		};

		vkCmdSetScissor(s_ImGuiCommandBuffers[current_command_buffer_inedx], 0, 1, &scissor);

		ImDrawData* main_draw_data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(main_draw_data, s_ImGuiCommandBuffers[current_command_buffer_inedx]);

		vk::vk_check(vkEndCommandBuffer(s_ImGuiCommandBuffers[current_command_buffer_inedx]), "vkEndCommandBuffer", __FILE__, __LINE__, __FUNCTION__);

		std::vector<VkCommandBuffer> command_buffers;
		command_buffers.push_back(s_ImGuiCommandBuffers[current_command_buffer_inedx]);

		vkCmdExecuteCommands(current_cmd_buffer, uint32_t(command_buffers.size()), command_buffers.data());

		vkCmdEndRenderPass(current_cmd_buffer);

		vk::vk_check(vkEndCommandBuffer(current_cmd_buffer), "vkEndCommandBuffer", __FILE__, __LINE__, __FUNCTION__);

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}



	}
};