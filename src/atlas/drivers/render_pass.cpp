#include "application.hpp"
#include <drivers/render_pass.hpp>
#include <drivers/vulkan/vulkan_renderpass.hpp>

namespace atlas {
    ref<renderpass> renderpass::create(
      const renderpass_properties& p_propeties) {
        switch (application::current_api()) {
            case API::VULKAN:
                create_ref<vk::VulkanRenderPass>(p_propeties);
            default:
                break;
        }

        return nullptr;
    }
};