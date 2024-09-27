#include <Core/EngineLogger.h>
#include <Core/backend/Vulkan/VulkanDevice.h>
#include <Core/backend/Vulkan/VulkanSwapchain.h>
#include <Core/backend/utilities/helper_functions.h>
#include <Core/Renderer/Renderer.h>
#include <Core/UI/UICore.h>
#include <Core/backend/Vulkan/Vulkan.h>
#include <Core/backend/utilities/helper_functions.h>

#include <vulkan/vulkan_core.h>
// #include <imgui/backends/imgui_impl_glfw.h>
// #include <imgui/backends/imgui_impl_vulkan.h>

namespace engine3d{
    static bool g_IsUICoreEnabled = false;
    // static ImGui_ImplVulkanH_Window g_MainWindowData;
    // ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;

    // static ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
    static VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;
    static uint32_t g_minImageCount = 2;

    //! @note Per frame in flight
    static std::vector<std::vector<VkCommandBuffer>> g_AllocateCommandBuffers;
    
    void UICore::InitializeImgui(){
        //! @note Setting up imgui with Vulkan!
        // IMGUI_CHECKVERSION();
        // ImGui::CreateContext();
        // ImGuiIO& io = ImGui::GetIO();
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		// //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		// io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		// ImGui::StyleColorsDark();
        // ImGuiStyle& style = ImGui::GetStyle();
		// if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
		// 	style.WindowRounding = 0.0f;
		// 	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		// }
        // ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;

        // //! @note Setting up descriptor poolings!
        // //! @note Setting up vulkan
        // VkDescriptorPoolSize poolSizes[] = {
        //     { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        //     { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        //     { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        //     { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        //     { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        //     { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        //     { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        //     { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        //     { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        //     { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        //     { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        // };
        
        // VkDescriptorPoolCreateInfo poolInfo  = {};
        // poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        // poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        // poolInfo.maxSets = 1000 * IM_ARRAYSIZE(poolSizes);
        // poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
        // poolInfo.pPoolSizes = poolSizes;

        // VkResult err = vkCreateDescriptorPool(vk::VulkanDevice::GetVkLogicalDeviceInstance(), &poolInfo, nullptr, &g_DescriptorPool);

        // if(err != VK_SUCCESS){
        //     ConsoleLogError("VkResulted in an error ===== {}", vk::VkResultToString(err));
        // }

        // //! @note Setting up vulkan-defined window with imgui.
        // // ImGuiSetupWindowWithVulkan(wd, vk::VulkanPipeline::GetVkSurface());
        // // SetupImGuiWindow();
        // SetupImGuiWindow();

        // g_AllocateCommandBuffers.resize(wd->ImageCount);

        // //! @note Setting up platform/renderer backends
        // ImGui_ImplGlfw_InitForVulkan(vk::VulkanPipeline::GetCurrentWindow(), true);
		// ImGui_ImplVulkan_InitInfo init_info = {};
		// init_info.Instance = vk::VulkanPipeline::GetVkInstance();
		// init_info.PhysicalDevice = vk::VulkanDevice::GetVkPhysicalDeviceInstance();
		// init_info.Device = vk::VulkanDevice::GetVkLogicalDeviceInstance();
		// init_info.QueueFamily = vk::VulkanDevice::GetLogicalDevice().QueueFamilyVkCount();
		// init_info.Queue = vk::VulkanDevice::GetLogicalDevice().QueueGraphicsFamily();
		// // init_info.PipelineCache = g_pipelineCache;
		// init_info.DescriptorPool = g_DescriptorPool;
		// init_info.Subpass = 0;
		// init_info.MinImageCount = g_minImageCount;
		// init_info.ImageCount = wd->ImageCount;
		// init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		// init_info.Allocator = nullptr;
		// // init_info.CheckVkResultFn = check_vk_result2;
		// ImGui_ImplVulkan_Init(&init_info);

        // ConsoleLogWarn("UICore::InitializeImgui() Successfully Initialized!");
    }

    //! @note Starting imgui frame
    void UICore::BeginImguiFrame(){
        // Starting imgui frame
        // ImGui_ImplVulkan_NewFrame();
        // ImGui_ImplGlfw_NewFrame();
        // ImGui::NewFrame();
    }

    void UICore::EndImguiFrame(){
        // ImGui::Render();
        // ImGuiIO& io = ImGui::GetIO();
        // ImDrawData* draw_data = ImGui::GetDrawData();
        // const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

        // if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
        //     ImGui::UpdatePlatformWindows();
        //     ImGui::RenderPlatformWindowsDefault();
        // }

        // //! @note TODO -- making the rendering tasks happen in renderers.
        // ImGuiRenderFrame(draw_data);
        // ImGuiPresentFrame();
    }

    void UICore::CleanupImgui(){
        // ImGui_ImplVulkan_Shutdown();
        // ImGui_ImplGlfw_Shutdown();
        // ImGui::DestroyContext();
        // ConsoleLogWarn("UICore::CleanupImgui() Successfully called!");
    }

    void UICore::SetupImGuiWindow(){
    }

    void UICore::ImGuiRenderFrame(ImDrawData* draw_data){
    }

    void UICore::ImGuiPresentFrame(){
    }


    //! @note Apply vulkan windows to imgui
    /*
    // void UICore::SetupImGuiWindow(){
    //     wd->Surface = vk::VulkanPipeline::GetVkSurface();
    //     VkSurfaceFormatKHR surfaceFormatProperties = vk::SelectSurfaceFormatAndColorspace(vk::VulkanPhysicalDevice::GetSurfaceFormats());
    //     const VkFormat* format = &surfaceFormatProperties.format;
    //     const VkColorSpaceKHR colorspace = surfaceFormatProperties.colorSpace;

    //     wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(vk::VulkanDevice::GetVkPhysicalDeviceInstance(), wd->Surface, format, sizeof(format), colorspace);
    //     #ifdef APP_USE_UNLIMITED_FRAME_RATE
    //         VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
    //     #else
    //         VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
    //     #endif
        
    //     wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(vk::VulkanDevice::GetVkPhysicalDeviceInstance(), wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));

    //     ImGui_ImplVulkanH_CreateOrResizeWindow(vk::VulkanPipeline::GetVkInstance(), vk::VulkanDevice::GetVkPhysicalDeviceInstance(), vk::VulkanDevice::GetVkLogicalDeviceInstance(), wd, vk::VulkanDevice::GetLogicalDevice().QueueFamilyVkCount(), nullptr, vk::VulkanPipeline::GetWidth(), vk::VulkanPipeline::GetHeight(), 2);
    // }
    
    // void UICore::ImGuiRenderFrame(ImDrawData* draw_data){
    //     VkSemaphore image_acquired_semaphore  = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
    //     VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
    //     VkSwapchainKHR swapchain = vk::VulkanSwapchain::GetVkSwapchainInstance();

    //     VkResult err;
    //     err = vkAcquireNextImageKHR(vk::VulkanDevice::GetVkLogicalDeviceInstance(), wd->Swapchain, std::numeric_limits<uint32_t>::max(), image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
    //     if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR){
    //         return;
    //     }
        
    //     ////////////////////////////////////////////////////////
    //     ////////////////////////////////////////////////////////
    //     ////////////////////////////////////////////////////////

    //     ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
    //     err = vkWaitForFences(vk::VulkanDevice::GetVkLogicalDeviceInstance(), 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
        // check_vk_result(err);

    //     err = vkResetFences(vk::VulkanDevice::GetVkLogicalDeviceInstance(), 1, &fd->Fence);
    //     // check_vk_result(err);

    //     ////////////////////////////////////////////////////////
    //     ////////////////////////////////////////////////////////
    //     ////////////////////////////////////////////////////////

    //     err = vkResetCommandPool(vk::VulkanDevice::GetVkLogicalDeviceInstance(), fd->CommandPool, 0);
    //     // check_vk_result(err);

    //     VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
    //     cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    //     cmdBufferBeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    //     err = vkBeginCommandBuffer(fd->CommandBuffer, &cmdBufferBeginInfo);
    //     // check_vk_result(err);

    //     VkRenderPassBeginInfo info = {};
    //     info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    //     info.renderPass = wd->RenderPass;
    //     info.framebuffer = fd->Framebuffer;
    //     // info.framebuffer = vk::VulkanSwapchain::ReadFramebuffer(wd->FrameIndex);
    //     info.renderArea.extent.width = vk::VulkanPipeline::GetWidth();
    //     info.renderArea.extent.height = vk::VulkanPipeline::GetHeight();
    //     info.clearValueCount = 1;
    //     info.pClearValues = &wd->ClearValue;
    //     vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);

    //     // Record dear imgui primitives into command buffer
    //     ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

    //     // Submit command buffer
    //     vkCmdEndRenderPass(fd->CommandBuffer);

    //     VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    //     VkSubmitInfo submitInfo = {};
    //     submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    //     submitInfo.waitSemaphoreCount = 1;
    //     submitInfo.pWaitSemaphores = &image_acquired_semaphore;
    //     submitInfo.pWaitDstStageMask = &wait_stage;
    //     submitInfo.commandBufferCount = 1;
    //     submitInfo.pCommandBuffers = &fd->CommandBuffer;
    //     submitInfo.signalSemaphoreCount = 1;
    //     submitInfo.pSignalSemaphores = &render_complete_semaphore;

    //     err = vkEndCommandBuffer(fd->CommandBuffer);
    //     // check_vk_result(err);
    //     err = vkQueueSubmit(vk::VulkanDevice::GetLogicalDevice().QueueGraphicsFamily(), 1, &submitInfo, fd->Fence);
    //     // check_vk_result(err);
    // }
    */
    /*void UICore::ImGuiPresentFrame(){
        VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
        VkPresentInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &render_complete_semaphore;
        info.swapchainCount = 1;
        info.pSwapchains = &wd->Swapchain;
        info.pImageIndices = &wd->FrameIndex;
        VkResult err = vkQueuePresentKHR(vk::VulkanDevice::GetLogicalDevice().QueueGraphicsFamily(), &info);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR){
            return;
        }
        // check_vk_result(err);
        // wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->SemaphoreIndex; // Now we can use the next set of semaphores
    }*/
};
