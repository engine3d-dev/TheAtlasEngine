#include "drivers/vulkan/helper_functions.hpp"
#include "drivers/vulkan/vulkan_core.hpp"
#include "drivers/vulkan/vulkan_renderer.hpp"
#include "engine_logger.hpp"
#include <drivers/ui/imgui_backend.hpp>
#include <core/application.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <fmt/format.h>

// constexpr void assert_expression(bool expression, char* message){
// 	// static constexpr auto* name = "";
// 	static_assert(expression, "name");
// }

// constexpr void assert_expression(){
// 	constexpr std::string_view message = "This is my error message";

// 	static_assert(false, message);
// }

namespace atlas{
	static void ImGuiLayoutColorModification(){
		auto& colors = ImGui::GetStyle().Colors; // @note Colors is ImVec4
			
			colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

			// Headers
			colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
			colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
			colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
			
			
			// Buttons
			colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
			colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
			colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };


			// Frame BG
			colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
			colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
			colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };


			// Tabs
			colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
			colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
			colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
			colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.15f, 1.0f };
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
			
			// Titles
			colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
			colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.15f, 1.0f };
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.1f, 0.150f, 0.951f, 1.0f };
	}
	
	static std::vector<VkCommandBuffer> s_ImGuiCommandBuffers;
	// static VkCommandPool imgui_command_pool;
	static VkDescriptorPool s_DescriptorPool;
	// static VkRenderPass imgui_renderpass;
	// static std::vector<VkFramebuffer> s_ImGuiFramebuffers;
	// static std::vector<VkImageView> s_ImGuiViewportImageViews;
	// static std::vector<VkImage> s_ImGuiViewportImages;
	static std::vector<VkFramebuffer> s_ViewportFramebuffers;
	VkCommandPool s_ViewportCommandPool;
	VkRenderPass s_ViewportRenderpass;
	static vk::vk_driver g_Driver;

	struct ImGuiImage{
		VkImage Image;
		VkImageView ImageView;
		VkDeviceMemory ImageDeviceMemory;
	};

	static std::vector<ImGuiImage> s_ImGuiViewportImages;

	// static std::vector<
	// static VkCommandPool cmd_pool;

	static void updateViewport(const VkCommandBuffer& p_command_buffer, int p_width, int p_height){
		// 1. Get the ImGui viewport's position and size
		ImGuiViewport* viewport = ImGui::GetMainViewport(); // Or a specific viewport if you're using multiple
		ImVec2 pos = viewport->Pos;
		// ImVec2 size = viewport->Size;
	
		// 2. Create the Vulkan viewport struct
		VkViewport vkViewport = {};
		vkViewport.x = pos.x;
		vkViewport.y = pos.y;
		vkViewport.width = p_width;
		vkViewport.height = p_height;
		vkViewport.minDepth = 0.0f;
		vkViewport.maxDepth = 1.0f;
	
		// 3. Create a scissor rectangle (optional, but recommended)
		VkRect2D scissorRect = {};
		scissorRect.offset = {static_cast<int32_t>(pos.x), static_cast<int32_t>(pos.y)};
		scissorRect.extent = {static_cast<uint32_t>(p_width), static_cast<uint32_t>(p_height)};
	
		// 4. Set the viewport and scissor in your Vulkan command buffer
		vkCmdSetViewport(p_command_buffer, 0, 1, &vkViewport);
		vkCmdSetScissor(p_command_buffer, 0, 1, &scissorRect);
	}

    void ImGuiBackend::Initialize(){
        console_log_info("ImGui Vulkan Test: Begin Initialization!");
		g_Driver = vk::vk_context::get_current_driver();

		// 1: create descriptor pool for IMGUI
		//  the size of the pool is very oversize, but it's copied from imgui demo
		//  itself.
		VkDescriptorPoolSize pool_sizes[] = { 
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 100 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 100 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 100 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100 }
		};

		VkDescriptorPoolCreateInfo desc_pool_create_info = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
			.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes),
			// .poolSizeCount = (uint32_t)std::size(pool_sizes),
			.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes),
			.pPoolSizes = pool_sizes
		};

		// VkDescriptorPool imgui_pool;
		vk::vk_check(vkCreateDescriptorPool(g_Driver, &desc_pool_create_info, nullptr, &s_DescriptorPool), "vkCreateDescriptorPool", __FILE__, __LINE__, __FUNCTION__);
		console_log_info("After creating descriptor sets for IMGUI");



		//! @note Creating separate render pass for imgui
		VkAttachmentDescription attachment_description = {
			.format = application::get_window().get_current_swapchain()->get_swapchain_format(),
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
		};

		VkAttachmentReference color_attachment = {
			.attachment = 0,
			.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		};

		VkSubpassDescription subpass = {
			.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.colorAttachmentCount = 1,
			.pColorAttachments = &color_attachment
		};

		VkSubpassDependency subpass_dependency = {
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = 0,
			.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			.srcAccessMask = 0, // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
			.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
		};

		VkRenderPassCreateInfo imgui_renderpass_ci = {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			.attachmentCount = 1,
			.pAttachments = &attachment_description,
			.subpassCount = 1,
			.pSubpasses = &subpass,
			.dependencyCount = 1,
			.pDependencies = &subpass_dependency
		};

		vk::vk_check(vkCreateRenderPass(g_Driver, &imgui_renderpass_ci, nullptr, &s_ViewportRenderpass), "vkCreateRenderPass", __FILE__, __LINE__, __FUNCTION__);

		console_log_warn("After Creating renderpass!");

		// static_assert(false, "Error from static_assert!");
		// assert(true);
		// auto swapchain_images_size = application::get_window().get_current_swapchain()->get_images_size();
		
		auto swapchain = application::get_window().get_current_swapchain();
		auto swapchain_images_size = swapchain->get_images_size();

		// Creating viewport framebuffers
		s_ViewportFramebuffers.resize(swapchain_images_size);
		console_log_warn("Resize viewport framebuffers = {}", s_ViewportFramebuffers.size());

		for(size_t i = 0; i < s_ViewportFramebuffers.size(); i++){
			VkImageView attachment[1];
			//! @note For Framebuffer abstraction layer have the specifications
			//! @note Each framebuffer abstraction will define the renderpass themselves
			VkFramebufferCreateInfo fb_ci = {
				.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				.renderPass = s_ViewportRenderpass,
				.attachmentCount = 1,
				.pAttachments = attachment,
				.width = application::get_window().get_current_swapchain()->get_extent().width,
				.height = application::get_window().get_current_swapchain()->get_extent().height,
				.layers = 1,
			};

			vk::vk_check(vkCreateFramebuffer(g_Driver, &fb_ci, nullptr, &s_ViewportFramebuffers[i]), "vkCreateFramebuffer", __FILE__, __LINE__, __FUNCTION__);
		}

		console_log_warn("After setting our framebuffers information. Framebuffer.size() = {}", s_ViewportFramebuffers.size());

		//! @note Creating images for the viewport
		s_ImGuiViewportImages.resize(swapchain_images_size);


		console_log_trace("After setting viewport images.size() = {}", s_ImGuiViewportImages.size());
		//! @note Viewport-specific images
		for(size_t i = 0; i <  s_ImGuiViewportImages.size(); i++){
			s_ImGuiViewportImages[i].Image = vk::create_image(VK_FORMAT_B8G8R8A8_SRGB, swapchain->get_extent().width, swapchain->get_extent().height);

			// Create memory to backup image
			VkMemoryRequirements memory_requirements;
			vkGetImageMemoryRequirements(g_Driver, s_ImGuiViewportImages[i].Image, &memory_requirements);
			
			// Allocate memory for these images
			VkMemoryAllocateInfo mem_alloc_info = {
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO
			};
			vkAllocateMemory(g_Driver, &mem_alloc_info, nullptr, &s_ImGuiViewportImages[i].ImageDeviceMemory);
			vkBindImageMemory(g_Driver, s_ImGuiViewportImages[i].Image, s_ImGuiViewportImages[i].ImageDeviceMemory, 0);
		}



		//! @note Setting up imgui stuff.
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		// io.ConfigViewportsNoAutoMerge = true;
		// io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setting custom dark themed imgui layout
		ImGuiLayoutColorModification();

		// Setup Dear ImGui style
		// ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		// Setup Dear ImGui style

        ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// this initializes imgui for Vulkan
		ImGui_ImplGlfw_InitForVulkan(application::get_window().get_native_window(), true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = vk::vk_context::get_vk_instance();
		init_info.PhysicalDevice = vk::vk_context::get_current_selected_physical_driver();
		init_info.Device = g_Driver;
		init_info.Queue = g_Driver.get_graphics_queue();
		init_info.RenderPass = application::get_window().get_current_swapchain()->get_renderpass();
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = s_DescriptorPool;
		init_info.MinImageCount = 2;
		init_info.ImageCount = application::get_window().get_current_swapchain()->get_images_size();
		init_info.UseDynamicRendering = false;




		//dynamic rendering parameters for imgui to use
		// init_info.PipelineRenderingCreateInfo = { .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO };
		// init_info.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
		// init_info.PipelineRenderingCreateInfo.pColorAttachmentFormats = (const VkFormat*)application::get_window().get_current_swapchain()->get_swapchain_format();

		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		ImGui_ImplVulkan_Init(&init_info);
		console_log_warn("After ImGui_ImplGlfw_InitForVulkan called!");

		ImGui_ImplVulkan_CreateFontsTexture();

	}

	//! TODO: Get scene into imgui window
    void ImGuiBackend::Begin(){
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

    void ImGuiBackend::End(){
		ImGui::Render();

		auto current_cmd_buffer = vk::vk_renderer::current_command_buffer();

		int width, height;
		glfwGetFramebufferSize(application::get_window(), &width, &height); // Or use glfwget_windowSize
		updateViewport(current_cmd_buffer, width, height); // Pass window width and height for initial viewport setup


		
		//! @note This works, dont modify.
		ImDrawData* draw_data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(draw_data, current_cmd_buffer);

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
};