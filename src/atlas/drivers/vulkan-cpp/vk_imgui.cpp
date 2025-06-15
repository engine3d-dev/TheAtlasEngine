#include <drivers/vulkan-cpp/vk_imgui.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <core/engine_logger.hpp>
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>

namespace atlas::vk {

    static void im_gui_layout_color_modification() {
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
        colors[ImGuiCol_TabUnfocusedActive] =
          ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Titles
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.15f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] =
          ImVec4{ 0.1f, 0.150f, 0.951f, 1.0f };
    }

    imgui_context::imgui_context(GLFWwindow* p_window_handler, const vk_swapchain& p_current_swapchain_handler, const VkRenderPass& p_current_renderpass) {
        console_log_warn("vk_imgui Begin Initialization!!!");
        m_instance = vk_context::handler();
        m_physical = vk_context::physical_driver();
        m_driver = vk_context::driver_context();

        if(m_driver == nullptr) {
            console_log_fatal("m_driver == nullptr!");
        }
        else {
            console_log_fatal("m_driver != nullptr!");
        }

        m_current_swapchain_handler = p_current_swapchain_handler;

        // Setting up imgui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        
        io.ConfigFlags |=
          ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable
        // Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        io.ConfigFlags |=
          ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform
                                            // Windows
        
        // io.ConfigViewportsNoAutoMerge = true;
        // io.ConfigViewportsNoAutoMerge = true;
        // io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        // ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();
        im_gui_layout_color_modification();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

         // 1: create descriptor pool for IMGUI
        //  the size of the pool is very oversize, but it's copied from imgui
        //  demo itself.
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
        vk::vk_check(
          vkCreateDescriptorPool(
            m_driver, &desc_pool_create_info, nullptr, &m_desc_pool),
          "vkCreateDescriptorPool",
          __FILE__,
          __LINE__,
          __FUNCTION__);
        
        recreate(p_window_handler, p_current_swapchain_handler.image_size(), p_current_renderpass);
        console_log_warn("vk_imgui End Initialization!!!");
    }

    void imgui_context::recreate(GLFWwindow* p_window_handler, const uint32_t& p_image_size, const VkRenderPass& p_current_renderpass) {
        ImGui_ImplGlfw_InitForVulkan(p_window_handler, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = m_instance;
        init_info.PhysicalDevice = m_physical;
        init_info.Device = m_driver;
        init_info.Queue = m_driver.graphics_queue();
        init_info.RenderPass = p_current_renderpass;
        init_info.PipelineCache = nullptr;
        init_info.DescriptorPool = m_desc_pool;
        init_info.MinImageCount = 2;
        init_info.ImageCount = p_image_size;
        init_info.UseDynamicRendering = false;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        ImGui_ImplVulkan_Init(&init_info);

        vk_context::submit_resource_free([](){
            console_log_warn("vk_imgui freed its resources!!!");
            //! @note This will probably be submitted at construction to be destructed when the application shutsdown
            ImGui_ImplVulkan_Shutdown();
            // vkDestroyDescriptorPool(m_driver, m_desc_pool, nullptr);
            // vkDestroyDescriptorPool(m_driver, m_desc_pool, nullptr);
            // vkDestroyRenderPass(m_driver, m_re, nullptr);
            // vkDestroyCommandPool(m_driver, m_imgui_command_pool, nullptr);

        });
    }

    void imgui_context::begin(const VkCommandBuffer& p_current, [[maybe_unused]] const uint32_t& p_current_frame_idx) {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        m_current_command = p_current;
    }
    
    void imgui_context::end() {
        ImGui::Render();

        ImDrawData* draw_data = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(draw_data, m_current_command);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void imgui_context::draw_hud(const hud_data& p_hud_data, const window_settings& p_settings) {
        ImGuiIO& io = ImGui::GetIO();

        // --- Top-Left HUD (FPS and Weapon) ---
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always); // Fixed position
        ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background

        ImGuiWindowFlags hud_flags =
            ImGuiWindowFlags_NoDecoration | // No title bar, borders, etc.
            ImGuiWindowFlags_NoMove |       // Cannot be moved by user
            ImGuiWindowFlags_NoResize |     // Cannot be resized by user
            ImGuiWindowFlags_NoSavedSettings | // Don't save position/size to .ini
            ImGuiWindowFlags_NoInputs;      // Does not block mouse/keyboard input

        if (ImGui::Begin("TopLeftHUD", nullptr, hud_flags)) {
            ImGui::Text("FPS: %.1f", p_hud_data.fps);
            ImGui::Text("Weapon: %s", p_hud_data.currentWeapon.c_str());
            // ImGui::Text("Health:");
            // Use a progress bar for health
            // ImGui::ProgressBar(p_hud_data.playerHealth / 100.0f, ImVec2(-1, 0), "#Health"); // -1 width fills available space
            ImGui::Text("Health: %.1f", p_hud_data.playerHealth);
            ImGui::ProgressBar(p_hud_data.playerHealth / 100.0f, ImVec2(-1, 0)); // No overlay text here
        }
        ImGui::End();

        // --- Health Bar (Bottom-Left) ---
        ImGui::SetNextWindowPos(ImVec2(10, (float)p_settings.width - 50), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(200, 40), ImGuiCond_Always); // Fixed size
        ImGui::SetNextWindowBgAlpha(0.0f);

        // if (ImGui::Begin("HealthBar", nullptr, hud_flags)) {
        //     ImGui::Text("Health:");
        //     // Use a progress bar for health
        //     ImGui::ProgressBar(p_hud_data.playerHealth / 100.0f, ImVec2(-1, 0), "HP"); // -1 width fills available space
        // }
        // ImGui::End();

        // --- Score (Top-Right) ---
        // Position it relative to the right edge
        ImGui::SetNextWindowPos(ImVec2((float)p_settings.width - 150, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(140, 0), ImGuiCond_Always); // 0 height means auto-size vertically
        ImGui::SetNextWindowBgAlpha(0.0f);

        if (ImGui::Begin("ScoreDisplay", nullptr, hud_flags)) {
            ImGui::SetCursorPosX(ImGui::GetWindowSize().x - ImGui::CalcTextSize(std::to_string(p_hud_data.playerScore).c_str()).x - ImGui::GetStyle().WindowPadding.x);
            ImGui::Text("Score: %d", p_hud_data.playerScore);
        }
        ImGui::End();

        // --- Crosshair (Custom Drawing - Optional) ---
        // If you want a more custom crosshair, you might use ImDrawList.
        // This is more advanced and often handled by your game's rendering pipeline directly,
        // but ImGui can draw simple shapes.
        // To draw on the main viewport, you can get the foreground draw list of the main viewport.
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList(); // Or ImGui::GetForegroundDrawList()
        if (draw_list) {
            ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
            float crosshair_size = 10.0f;
            float gap = 2.0f;
            ImU32 crosshair_color = IM_COL32(255, 255, 255, 255); // White

            // Horizontal lines
            draw_list->AddLine(ImVec2(center.x - crosshair_size - gap, center.y), ImVec2(center.x - gap, center.y), crosshair_color);
            draw_list->AddLine(ImVec2(center.x + gap, center.y), ImVec2(center.x + crosshair_size + gap, center.y), crosshair_color);

            // Vertical lines
            draw_list->AddLine(ImVec2(center.x, center.y - crosshair_size - gap), ImVec2(center.x, center.y - gap), crosshair_color);
            draw_list->AddLine(ImVec2(center.x, center.y + gap), ImVec2(center.x, center.y + crosshair_size + gap), crosshair_color);
        }
    }
};