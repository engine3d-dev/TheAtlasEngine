#pragma once
// #include <Core/backend/third_party/imgui/backends/imgui_impl_vulkan.h>
// #include <imgui/backends/imgui_impl_vulkan.h>

class ImDrawData;
namespace engine3d{
    /**
     * @name UICore
     * @note Core UI Wrapper for ImGui operations
     * @note Initiating ImGui that is graphics API agnostic
    */
    class UICore{
    public:
        //! @note Initializes ImGui
        static void InitializeImgui();

        static void BeginImguiFrame();

        //! @note At the end of each frame we render using the data we collected from imgui.
        static void EndImguiFrame();

        //! @note Cleansup ImGui from Engine3D.
        static void CleanupImgui();
    private:
        static void SetupImGuiWindow();

        static void ImGuiRenderFrame(ImDrawData* draw_data);

        static void ImGuiPresentFrame();
        // static void ImGuiSetupWindowWithVulkan(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface);

        /**
         * @name FrameRender
         * @note Submitting our window data to our vulkan abstraction layer
        */
        // static void ImGuiFrameRender(ImGui_ImplVulkanH_Window* window, ImDrawData* drawData);

        /**
         * @name VulkanPresentation
         * @note Presenting all of our window and imgui data to our vulkan API
         * @note We pass imgui data to our swapchain that will get presented!
        */
        // static void ImGuiVulkanPresentation(ImGui_ImplVulkanH_Window* window);
    };
};