module;

#include <string>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <array>
#include <GLFW/glfw3.h>
#include <print>
#include <memory>
#include <optional>
#include <span>

export module atlas.drivers.vulkan:imgui_context;

import :graphics_context;

import atlas.common;
import vk;

import atlas.core.utilities;

namespace atlas {
    void imgui_layout_color_modification() {
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

    VkPipelineRenderingCreateInfo pipeline_rendering_info(
      std::span<const uint32_t> p_color_attachment_formats,
      const uint32_t p_depth_format,
      const uint32_t p_stencil_format) {
        VkPipelineRenderingCreateInfo rendering_ci = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR,
            .pNext = nullptr,
            .colorAttachmentCount =
              static_cast<uint32_t>(p_color_attachment_formats.size()),
            .pColorAttachmentFormats = reinterpret_cast<const VkFormat*>(
              p_color_attachment_formats.data()),
            .depthAttachmentFormat = static_cast<VkFormat>(p_depth_format),
            .stencilAttachmentFormat = static_cast<VkFormat>(p_stencil_format),
        };

        return rendering_ci;
    }

    export class imgui_context {
    public:
        imgui_context() = delete;
        imgui_context(/*NOLINT*/ std::shared_ptr<graphics_context> p_context,
                      GLFWwindow* p_window,
                      /*NOLINT*/ std::shared_ptr<vk::swapchain> p_swapchain,
                      uint32_t p_image_count,
                      const vk::device_present_queue& p_queue,
                      /*NOLINT*/ const VkFormat& p_color_format,
                      /*NOLINT*/ const VkFormat& p_depth_format) {
            m_instance = p_context->instance_handle();
            m_device = p_context->logical_device();
            m_physical = p_context->physical_device();

            std::println("Constructing imgui_context");

            // Common setup for imgui
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();

            io.ConfigFlags |=
              ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
            // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      //
            // Enable Gamepad Controls
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
            io.ConfigFlags |=
              ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport /
                                                // Platform Windows

            // Additional configurations
            imgui_layout_color_modification();

            ImGuiStyle& style = ImGui::GetStyle();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                style.WindowRounding = 0.0f;
                style.Colors[ImGuiCol_WindowBg].w = 1.0f;
            }

            // Configurnig descriptor pool
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
            vk::vk_check(
              vkCreateDescriptorPool(
                *m_device, &desc_pool_create_info, nullptr, &m_descriptor_pool),
              "vkCreateDescriptorPool");

            construct(p_window, /*NOLINT*/ p_swapchain, p_image_count, p_queue, p_color_format, p_depth_format);
        }

        void construct(GLFWwindow* p_window,
                       /*NOLINT*/ std::shared_ptr<vk::swapchain> p_swapchain,
                       uint32_t p_image_count,
                       const vk::device_present_queue& p_queue,
                       /*NOLINT*/ const VkFormat& p_color_format,
                       /*NOLINT*/ const VkFormat& p_depth_format) {

            ImGui_ImplGlfw_InitForVulkan(p_window, true);
            ImGui_ImplVulkan_InitInfo init_info = {};
            init_info.Instance = m_instance.value();
            init_info.PhysicalDevice = m_physical.value();
            init_info.Device = *m_device;
            init_info.Queue = p_queue;
            init_info.RenderPass = nullptr;
            init_info.PipelineCache = nullptr;
            init_info.DescriptorPool = m_descriptor_pool;
            init_info.MinImageCount = 2;
            init_info.ImageCount = p_image_count;
            init_info.UseDynamicRendering = true;
            init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

            const uint32_t color_format = static_cast<uint32_t>(p_color_format);
            const uint32_t depth_format = static_cast<uint32_t>(p_depth_format);
            init_info.PipelineRenderingCreateInfo = pipeline_rendering_info(
              std::span<const uint32_t>(&color_format, 1),
              depth_format,
              depth_format);
            ImGui_ImplVulkan_Init(&init_info);
        }

        void set_current_command(vk::command_buffer& p_command) {
            m_current_command = &p_command;
        }

        void begin() {
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        void end() {
            ImGui::Render();

            ImDrawData* draw_data = ImGui::GetDrawData();
            ImGui_ImplVulkan_RenderDrawData(draw_data, *m_current_command);

            ImGuiIO& io = ImGui::GetIO();
            if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
        }

        void destruct() {
            ImGui_ImplVulkan_Shutdown();
            vkDestroyDescriptorPool(*m_device, m_descriptor_pool, nullptr);

            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

    private:
        VkDescriptorPool m_descriptor_pool = nullptr;
        vk::command_buffer* m_current_command;
        std::optional<vk::instance> m_instance;
        std::optional<vk::physical_device> m_physical;
        std::shared_ptr<vk::device> m_device = nullptr;
        vk::surface_params m_surface_properties;
    };
};