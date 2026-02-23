module;

#include <string>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <array>

export module atlas.drivers.vulkan.imgui_context;

import atlas.common;
import vk;

import atlas.drivers.vulkan.instance_context;
import atlas.drivers.vulkan.physical_device;
import atlas.drivers.vulkan.device;
import atlas.drivers.vulkan.swapchain;
import atlas.core.utilities;

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

    void transition_image_layout(VkDevice p_device, vk::sample_image& p_image, VkFormat p_format, VkImageLayout p_old, VkImageLayout p_new) {
        vk::command_params copy_command_params = {
            .levels = vk::command_levels::primary,
            .queue_index = 0,
            .flags = vk::command_pool_flags::reset,
        };
        vk::command_buffer temp_command_buffer = vk::command_buffer(p_device, copy_command_params);
        
        temp_command_buffer.begin(vk::command_usage::one_time_submit);

        p_image.memory_barrier(temp_command_buffer, p_format, p_old, p_new);

        temp_command_buffer.end();

        VkCommandBuffer handle = temp_command_buffer;
        VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .commandBufferCount = 1,
            .pCommandBuffers = &handle,
        };

        uint32_t queue_family_index = 0;
        uint32_t queue_index = 0;
        VkQueue temp_graphics_queue;
        vkGetDeviceQueue(p_device, queue_family_index, queue_index, &temp_graphics_queue);

        vkQueueSubmit(temp_graphics_queue, 1, &submit_info, nullptr);
        vkQueueWaitIdle(temp_graphics_queue);

        temp_command_buffer.destroy();
    }

    export ImTextureID g_viewport_image_id = nullptr;
    export class imgui_context {
    public:
        imgui_context() = default;

        imgui_context(const VkInstance& p_instance, const swapchain& p_swapchain_ctx, GLFWwindow* p_window_ctx) {
            m_instance = p_instance;
            m_physical = instance_context::physical_driver();
            VkPhysicalDeviceMemoryProperties memory_properties = instance_context::physical_driver().memory_properties();
            m_driver = instance_context::logical_device();
            // vk::device device_temp = instance_context::physical_driver();

            m_glfw_window = p_window_ctx;
            m_current_swapchain_handler = p_swapchain_ctx;
            m_extent = {.width = p_swapchain_ctx.settings().width, .height = p_swapchain_ctx.settings().height};

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
            p_swapchain_ctx.image_size());

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

            create(p_window_ctx,
                p_swapchain_ctx.image_size(),
                p_swapchain_ctx.swapchain_renderpass());

            vk::image_params config_image = {
                .extent = { .width = p_swapchain_ctx.settings().width, .height = p_swapchain_ctx.settings().height },
                .format = VK_FORMAT_B8G8R8A8_UNORM,
                .property = vk::memory_property::device_local_bit,
                .aspect = vk::image_aspect_flags::color_bit,
                .usage = vk::image_usage::color_attachment_bit | vk::image_usage::transfer_dst_bit | vk::image_usage::sampled_bit,
                // .usage = vk::image_usage::color_attachment_bit | vk::image_usage::transfer_dst_bit,
                .phsyical_memory_properties = memory_properties,
                .address_mode_u = vk::sampler_address_mode::clamp_to_edge,
                .addrses_mode_v = vk::sampler_address_mode::clamp_to_edge,
                .addrses_mode_w = vk::sampler_address_mode::clamp_to_edge,
            };
            m_viewport_image = vk::sample_image(m_driver, config_image);

            // transition image layout uses image memory barrier
            transition_image_layout(m_driver, m_viewport_image, VK_FORMAT_B8G8R8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

            // Viewport-specific render pass: color attachment ends in SHADER_READ_ONLY_OPTIMAL
            // so the offscreen texture can be sampled by ImGui. (Swapchain pass uses PRESENT_SRC.)
            VkFormat depth_format = m_driver.depth_format();
            std::array<::vk::attachment, 2> viewport_attachments = {
                ::vk::attachment{
                    .format = VK_FORMAT_B8G8R8A8_UNORM,
                    .layout = ::vk::image_layout::color_optimal,
                    .samples = ::vk::sample_bit::count_1,
                    .load = ::vk::attachment_load::clear,
                    .store = ::vk::attachment_store::store,
                    .stencil_load = ::vk::attachment_load::clear,
                    .stencil_store = ::vk::attachment_store::dont_care,
                    .initial_layout = ::vk::image_layout::undefined,
                    .final_layout = ::vk::image_layout::shader_read_only_optimal,
                },
                ::vk::attachment{
                    .format = depth_format,
                    .layout = ::vk::image_layout::depth_stencil_optimal,
                    .samples = ::vk::sample_bit::count_1,
                    .load = ::vk::attachment_load::clear,
                    .store = ::vk::attachment_store::dont_care,
                    .stencil_load = ::vk::attachment_load::dont_care,
                    .stencil_store = ::vk::attachment_store::dont_care,
                    .initial_layout = ::vk::image_layout::undefined,
                    .final_layout = ::vk::image_layout::depth_stencil_optimal,
                },
            };
            m_viewport_renderpass = ::vk::renderpass(m_driver, viewport_attachments);

            vk::image_params config_depth_image = {
                .extent = { .width = p_swapchain_ctx.settings().width, .height = p_swapchain_ctx.settings().height },
                .format = m_driver.depth_format(),
                .property = vk::memory_property::device_local_bit,
                .aspect = vk::image_aspect_flags::depth_bit,
                .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                .phsyical_memory_properties = memory_properties,
            };

            m_depth_viewport_image = vk::sample_image(m_driver, config_depth_image);

            for (uint32_t i = 0; i < m_viewport_framebuffers.size(); i++) {
                std::array<VkImageView, 2> image_view_attachments = { m_viewport_image.image_view(), m_depth_viewport_image.image_view() };

                vk::framebuffer_params framebuffer_info = {
                    .renderpass = m_viewport_renderpass,
                    .views = image_view_attachments,
                    .extent = {p_swapchain_ctx.settings().width, p_swapchain_ctx.settings().height}
                };
                m_viewport_framebuffers[i] = vk::framebuffer(m_driver, framebuffer_info);
            }

            // g_viewport_image_id = (ImTextureID)ImGui_ImplVulkan_AddTexture(
            g_viewport_image_id = static_cast<ImTextureID>(ImGui_ImplVulkan_AddTexture(
                m_viewport_image.sampler(), 
                m_viewport_image.image_view(), 
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            ));
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

            // Sync ImGui display size and main viewport with the host GLFW window so the
            // dockspace and widgets resize correctly when the window is resized.
            if (m_glfw_window != nullptr) {
                int fb_width = 0, fb_height = 0;
                glfwGetFramebufferSize(m_glfw_window, &fb_width, &fb_height);
                if (fb_width > 0 && fb_height > 0) {
                    ImGuiIO& io = ImGui::GetIO();
                    io.DisplaySize = ImVec2(static_cast<float>(fb_width), static_cast<float>(fb_height));
                    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
                    if (main_viewport != nullptr) {
                        main_viewport->Pos = ImVec2(0.f, 0.f);
                        main_viewport->Size = io.DisplaySize;
                    }
                }
            }

            m_current_frame_index = p_frame_index;
            m_current = p_current;
        }

        void invalidate(const swapchain& p_swapchain) {
            // Wait for device to finish all operations before recreating resources
            vkDeviceWaitIdle(m_driver);

            for(auto& fb : m_viewport_framebuffers) {
                fb.destroy();
            }

            // Remove old texture from ImGui if needed
            // ImTextureID in ImGui Vulkan backend is VkDescriptorSet cast to void*
            if (g_viewport_image_id != nullptr) {
                VkDescriptorSet old_descriptor_set = reinterpret_cast<VkDescriptorSet>(g_viewport_image_id);
                ImGui_ImplVulkan_RemoveTexture(old_descriptor_set);
                g_viewport_image_id = nullptr;
            }
            
            // Destroy old images
            m_viewport_image.destroy();
            m_depth_viewport_image.destroy();
            
            // Recreate viewport images with new swapchain size
            VkPhysicalDeviceMemoryProperties memory_properties = instance_context::physical_driver().memory_properties();
            
            vk::image_params config_image = {
                .extent = { .width = p_swapchain.settings().width, .height = p_swapchain.settings().height },
                .format = VK_FORMAT_B8G8R8A8_UNORM,
                .property = vk::memory_property::device_local_bit,
                .aspect = vk::image_aspect_flags::color_bit,
                .usage = vk::image_usage::color_attachment_bit | vk::image_usage::transfer_dst_bit | vk::image_usage::sampled_bit,
                .phsyical_memory_properties = memory_properties,
                .address_mode_u = vk::sampler_address_mode::clamp_to_edge,
                .addrses_mode_v = vk::sampler_address_mode::clamp_to_edge,
                .addrses_mode_w = vk::sampler_address_mode::clamp_to_edge,
            };
            m_viewport_image = vk::sample_image(m_driver, config_image);
            
            // Transition to shader read-only layout
            transition_image_layout(m_driver, m_viewport_image, VK_FORMAT_B8G8R8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            
            // Recreate depth image
            vk::image_params config_depth_image = {
                .extent = { .width = p_swapchain.settings().width, .height = p_swapchain.settings().height },
                .format = m_driver.depth_format(),
                .property = vk::memory_property::device_local_bit,
                .aspect = vk::image_aspect_flags::depth_bit,
                .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                .phsyical_memory_properties = memory_properties,
            };
            m_depth_viewport_image = vk::sample_image(m_driver, config_depth_image);
            
            // Recreate framebuffers with new images
            for (uint32_t i = 0; i < m_viewport_framebuffers.size(); i++) {
                std::array<VkImageView, 2> image_view_attachments = { 
                    m_viewport_image.image_view(), 
                    m_depth_viewport_image.image_view() 
                };
                
                vk::framebuffer_params framebuffer_info = {
                    .renderpass = m_viewport_renderpass,
                    .views = image_view_attachments,
                    .extent = {p_swapchain.settings().width, p_swapchain.settings().height}
                };
                m_viewport_framebuffers[i] = vk::framebuffer(m_driver, framebuffer_info);
            }
            
            // Update ImGui texture ID with new image
            g_viewport_image_id = (ImTextureID)ImGui_ImplVulkan_AddTexture(
                m_viewport_image.sampler(), 
                m_viewport_image.image_view(), 
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            );
            
            // Update extent
            m_extent = {.width = p_swapchain.settings().width, .height = p_swapchain.settings().height};

            ImGuiIO& io = ImGui::GetIO();
            // io.Display
            io.DisplaySize.x = static_cast<float>(m_extent.width);
            io.DisplaySize.y = static_cast<float>(m_extent.height);
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

        vk::framebuffer active_framebuffer(uint32_t p_frame) const {
            return m_viewport_framebuffers[p_frame];
        }

        [[nodiscard]] vk::renderpass viewport_renderpass() const {
            return m_viewport_renderpass;
        }

        void destroy() {
            ImGui_ImplVulkan_Shutdown();
            vkDestroyDescriptorPool(m_driver, m_desc_pool, nullptr);

            for (auto& command_buffer : m_viewport_command_buffers) {
                command_buffer.destroy();
            }

            for(auto& fb : m_viewport_framebuffers) {
                fb.destroy();
            }

            m_viewport_renderpass.destroy();
            m_viewport_image.destroy();
            m_depth_viewport_image.destroy();

            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

    private:
        VkInstance m_instance = nullptr;
        VkPhysicalDevice m_physical = nullptr;
        device m_driver{};
        GLFWwindow* m_glfw_window = nullptr;
        uint32_t m_current_frame_index = 0;
        VkSwapchainKHR m_current_swapchain_handler = nullptr;
        VkDescriptorPool m_desc_pool = nullptr;
        VkCommandBuffer m_current = nullptr;
        std::vector<::vk::command_buffer> m_viewport_command_buffers;
        std::array<vk::framebuffer, 2> m_viewport_framebuffers;
        ::vk::renderpass m_viewport_renderpass;
        vk::sample_image m_viewport_image;
        vk::sample_image m_depth_viewport_image;
        ImTextureID m_viewport_image_id=nullptr;
        window_params m_extent;
    };
};