#include "UILayer.h"
#include "engine3d/Core/backend/Vulkan/VulkanDevice.h"
#include "engine3d/Core/backend/Vulkan/VulkanSwapchain.h"
// #include "engine3d/Core/backend/Vulkan/VulkanSwapchain.h"
#include <engine3d/Core/backend/Vulkan/Vulkan.h>
#include <engine3d/Core/EngineLogger.h>
#include <engine3d/Core/backend/third_party/imgui/backends/imgui_impl_glfw.h>
#include <engine3d/Core/backend/third_party/imgui/imgui.h>
#include <engine3d/Core/backend/third_party/imgui/backends/imgui_impl_vulkan.h>

namespace engine3d{
    UILayer::UILayer() : Layer("UILayer") {
    }

    void UILayer::InitializeUI(VkQueue queue){
        // IMGUI_CHECKVERSION();
        // ImGui::CreateContext();
        // ImGuiIO& io = ImGui::GetIO(); (void)io;
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        // ImGui::StyleColorsDark();

        // Setup Dear ImGui style
		// ImGui::StyleColorsDark();
		// // SetDarkThemeV2Colors();
        // // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		// ImGuiStyle& style = ImGui::GetStyle();
		// if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		// {
		// 	style.WindowRounding = 0.0f;
		// 	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		// }
		// style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, style.Colors[ImGuiCol_WindowBg].w);

        // ImGui_ImplGlfw_InitForVulkan(vk::VulkanPipeline::GetCurrentWindow(), true);
        // ImGui_ImplVulkan_InitInfo init_info = {};
        // init_info.Instance = vk::VulkanPipeline::GetVkInstance();
        // init_info.PhysicalDevice = vk::VulkanDevice::GetVkPhysicalDeviceInstance();
        // init_info.Device = vk::VulkanDevice::GetVkLogicalDeviceInstance();
        // init_info.QueueFamily = vk::VulkanDevice::GetLogicalDevice().QueueFamily();
        // // init_info.Queue = g_Queue;
        // init_info.Queue = queue,
        // // init_info.PipelineCache = g_PipelineCache;
        // init_info.PipelineCache = nullptr,
        // // init_info.RenderPass = 1,
        // // init_info.DescriptorPool = g_DescriptorPool;
        // // init_info.RenderPass = wd->RenderPass;
        // init_info.Subpass = 1;
        // // init_info.MinImageCount = g_MinImageCount;
        // init_info.MinImageCount = 0,
        // // init_info.ImageCount = wd->ImageCount;
        // // init_info.ImageCount = 1,
        // init_info.ImageCount = vk::VulkanSwapchain::GetImagesSize(),
        // init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        // // init_info.Allocator = g_Allocator;
        // init_info.Allocator = nullptr,
        // // init_info.CheckVkResultFn = check_vk_result;

        // // ImGui_ImplVulkan_Init(&init_info);
        // ImGui_ImplVulkan_Init(&init_info, nullptr);
    }

    void UILayer::BeginPlay(){
        ConsoleLogInfo("UILayer::BeginPlay function called in class {}", Layer::GetLayerString());
        // ImGui_ImplGlfw_NewFrame();
        // IMGUI_CHECKVERSION();
        // ImGui::CreateContext();
        // ImGuiIO& io = ImGui::GetIO(); (void)io;
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        // // ImGui::StyleColorsDark();

        // // Setup Dear ImGui style
		// ImGui::StyleColorsDark();
		// // SetDarkThemeV2Colors();
        // // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		// ImGuiStyle& style = ImGui::GetStyle();
		// if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		// {
		// 	style.WindowRounding = 0.0f;
		// 	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		// }
		// style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, style.Colors[ImGuiCol_WindowBg].w);

        // ImGui_ImplGlfw_InitForVulkan(vk::VulkanPipeline::GetCurrentWindow(), true);
        // ImGui_ImplVulkan_InitInfo init_info = {};
        // init_info.Instance = vk::VulkanPipeline::GetVkInstance();
        // init_info.PhysicalDevice = vk::VulkanDevice::GetVkPhysicalDeviceInstance();
        // init_info.Device = vk::VulkanDevice::GetVkLogicalDeviceInstance();
        // init_info.QueueFamily = vk::VulkanDevice::GetLogicalDevice().QueueFamily();
        // // init_info.Queue = g_Queue;
        // init_info.Queue = nullptr,
        // // init_info.PipelineCache = g_PipelineCache;
        // init_info.PipelineCache = nullptr,
        // // init_info.RenderPass = 0,
        // // init_info.DescriptorPool = g_DescriptorPool;
        // // init_info.RenderPass = wd->RenderPass;
        // init_info.Subpass = 0;
        // // init_info.MinImageCount = g_MinImageCount;
        // init_info.MinImageCount = 0,
        // // init_info.ImageCount = wd->ImageCount;
        // init_info.ImageCount = 1,
        // init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        // // init_info.Allocator = g_Allocator;
        // init_info.Allocator = nullptr,
        // // init_info.CheckVkResultFn = check_vk_result;

        // // ImGui_ImplVulkan_Init(&init_info);
        // ImGui_ImplVulkan_Init(&init_info, nullptr);

        // ImGui_ImplVulkan_NewFrame();
        // ImGui_ImplGlfw_NewFrame();
        // ImGui::NewFrame();
        // ImGui_ImplGlfw_InitForVulkan(vk::VulkanPipeline::GetCurrentWindow(), false);

        // ImGui_ImplVulkan_NewFrame();
        // ImGui_ImplGlfw_NewFrame();
        // ImGui::NewFrame();
    }

    void UILayer::EndPlay(){
        // ConsoleLogInfo("UILayer::EndPlay function called in class {}", Layer::GetLayerString());
        // ImGui_ImplVulkan_Shutdown();
        // ImGui_ImplGlfw_Shutdown();
        // ImGui::DestroyContext();

        // ImGui_ImplVulkan_Shutdown();
        // ImGui_ImplGlfw_Shutdown();
        // ImGui::DestroyContext();
    }

    void UILayer::UpdateFrame(float ts){
        // ImGui::Render();
        // ConsoleLogInfo("UILayer::UpdateFrame(float) function called in class {}", Layer::GetLayerString());
    }

    void UILayer::UpdateEvent(Event& event){
        // ConsoleLogInfo("UILayer::UpdateEvent(Event&) function called in class {}", Layer::GetLayerString());
    }

    void UILayer::UpdateUI(){
        // ImGui::Begin("Settings");
        // ImGui::End();
    }

    std::string UILayer::GetName() const { return Layer::GetLayerString(); }
};