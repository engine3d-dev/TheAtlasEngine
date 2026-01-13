module;

#include <string>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>

export module atlas.drivers.vulkan.imgui_context;

import atlas.common;
import vk;
import atlas.window;

import atlas.drivers.vulkan.instance_context;
import atlas.drivers.vulkan.physical_device;
import atlas.drivers.vulkan.device;

namespace atlas::vulkan {
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

    export class imgui_context {
    public:
        imgui_context() = default;

        imgui_context(const ref<window>& p_window_ctx) {
            m_instance = instance_context::handle();
            m_physical = instance_context::physical_driver();
            m_driver = instance_context::logical_device();

            m_current_swapchain_handler = p_window_ctx->current_swapchain();

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

            m_viewport_command_buffers.resize(
            p_window_ctx->current_swapchain().image_size());

            for (size_t i = 0; i < m_viewport_command_buffers.size(); i++) {
                ::vk::command_params settings = {
                    .levels = ::vk::command_levels::primary,
                    // .queue_index = enumerate_swapchain_settings.present_index,
                    .queue_index = 0,
                    .flags = ::vk::command_pool_flags::reset,
                };
                m_viewport_command_buffers[i] =
                ::vk::command_buffer(m_driver, settings);
            }

            // ::vk::descriptor_res
            // m_imgui_descriptor = ::vk::descriptor_resource(m_driver, {});
            // 1: create descriptor pool for IMGUI
            //  the size of the pool is very oversize, but it's copied from imgui
            //  demo itself.
            std::array<VkDescriptorPoolSize, 11> pool_sizes = {
                VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
                VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                    100 },
                VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
                VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 },
                VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                                    100 },
                VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
                                    100 },
                VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
                VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100 },
                VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                                    100 },
                VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
                                    100 },
                VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100 }
            };

            VkDescriptorPoolCreateInfo desc_pool_create_info = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
                .pNext = nullptr,
                .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
                .maxSets = static_cast<uint32_t>(1000 * pool_sizes.size()),
                // .poolSizeCount = (uint32_t)std::size(pool_sizes),
                .poolSizeCount = static_cast<uint32_t>(pool_sizes.size()),
                .pPoolSizes = pool_sizes.data()
            };

            // VkDescriptorPool imgui_pool;
            vk::vk_check(vkCreateDescriptorPool(
                        m_driver, &desc_pool_create_info, nullptr, &m_desc_pool),
                        "vkCreateDescriptorPool");

            create(*p_window_ctx,
                p_window_ctx->current_swapchain().image_size(),
                p_window_ctx->current_swapchain().swapchain_renderpass());
        }

        void create(GLFWwindow* p_window_handler, const uint32_t& p_image_size, const VkRenderPass& p_current_renderpass) {
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
        }

        void begin(const VkCommandBuffer& p_current, const uint32_t& p_frame_index) {
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            m_current_frame_index = p_frame_index;
            m_current = p_current;
        }

        void end() {
            ImGui::Render();

            ImDrawData* draw_data = ImGui::GetDrawData();
            ImGui_ImplVulkan_RenderDrawData(draw_data, m_current);

            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
        }

        [[nodiscard]] ::vk::command_buffer imgui_active_command() const {
            return m_viewport_command_buffers[m_current_frame_index];
        }

        void destroy() {
            ImGui_ImplVulkan_Shutdown();
            vkDestroyDescriptorPool(m_driver, m_desc_pool, nullptr);

            for (auto& command_buffer : m_viewport_command_buffers) {
                command_buffer.destroy();
            }

            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

    private:
        VkInstance m_instance = nullptr;
        VkPhysicalDevice m_physical = nullptr;
        device m_driver{};
        uint32_t m_current_frame_index = 0;
        VkSwapchainKHR m_current_swapchain_handler = nullptr;
        VkDescriptorPool m_desc_pool = nullptr;
        VkCommandBuffer m_current = nullptr;
        std::vector<::vk::command_buffer> m_viewport_command_buffers;
    };
};