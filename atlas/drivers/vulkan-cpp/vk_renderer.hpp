#pragma once
#include <string>
#include <drivers/renderer_context.hpp>
#include <drivers/vulkan-cpp/vk_swapchain.hpp>
#include <drivers/vulkan-cpp/vk_command_buffer.hpp>

namespace atlas::vk {

    class vk_renderer : public render_context {
    public:
        vk_renderer(const std::string& p_tag);
        vk_renderer(const vk_swapchain& p_swapchain_handler, const std::string& p_tag);
        
        ~vk_renderer() override;

    private:
        void start_frame(const uint32_t& p_current_frame_index) override;
        void post_frame() override;
        void background_color(const std::array<float, 4>& p_color) override;

    private:
        vk_driver m_driver{};
        vk_swapchain m_swapchain_handler;
        vk_command_buffer m_current_command_buffer{};
        VkClearColorValue m_color;

        VkRenderPass m_main_renderpass=nullptr;
    };
};