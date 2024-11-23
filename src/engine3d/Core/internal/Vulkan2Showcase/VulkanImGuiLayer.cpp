#include <Core/ApplicationInstance.hpp>
#include <Core/EngineLogger.hpp>
#include <Core/internal/Vulkan2Showcase/VulkanContext.hpp>
#include <internal/Vulkan2Showcase/VulkanImGuiLayer.hpp>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    void VulkanImGuiLayer::Initialize() {
        ConsoleLogInfo("Vulkan2Showcase: Begin Initializing VulkanImGuiLayer!!!");
        //! @note Setting up imgui stuff.
        // Setup Dear ImGui context
		IMGUI_CHECKVERSION();
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

		// Setup Platform/Renderer backends
		// ImGui_ImplGlfw_InitForVulkan(ApplicationInstance::GetWindow().GetNativeWindow(), true);
		ImGui_ImplGlfw_InitForVulkan(ApplicationInstance::GetWindow().GetNativeWindow(), true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = VulkanContext::GetVkInstance();
		init_info.PhysicalDevice = VulkanContext::GetPhysicalDriver();
		init_info.Device = VulkanContext::GetDriver();
		init_info.QueueFamily = VulkanContext::GetPhysicalDriver().GetQueueIndices().Graphics;
		init_info.Queue = VK_NULL_HANDLE;
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = VK_NULL_HANDLE;
		init_info.Subpass = 0;
		init_info.MinImageCount = 2;
		init_info.ImageCount = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetImagesSize();
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator = nullptr;
		// init_info.CheckVkResultFn = check_vk_result;
		// ImGui_ImplVulkan_Init(&init_info);
		ImGui_ImplGlfw_InitForVulkan(ApplicationInstance::GetWindow().GetNativeWindow(), false);

		// Load default font
		ImFontConfig fontConfig;
		fontConfig.FontDataOwnedByAtlas = false;
		// ImFont* robotoFont = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoRegular, sizeof(g_RobotoRegular), 20.0f, &fontConfig);
		// io.FontDefault = robotoFont;

        ConsoleLogWarn("Vulkan2Showcase: Finished Initializing VulkanImGuiLayer!!!!");
    }

    void VulkanImGuiLayer::BeginFrame() {

    }

    void VulkanImGuiLayer::EndFrame() {

    }
};