#include "drivers/vulkan/helper_functions.hpp"
#include "engine_logger.hpp"
#include <drivers/ui/imgui_backend.hpp>
#include <core/application_instance.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <vulkan/vulkan_core.h>

namespace engine3d{
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

        ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
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
		init_info.MinImageCount = 3;
		init_info.ImageCount = 3;
		init_info.UseDynamicRendering = true;

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
		ConsoleLogInfo("Begin() from imgui backend called!");
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

    void ImGuiBackend::End(){
		ImGui::Render();
	}
};