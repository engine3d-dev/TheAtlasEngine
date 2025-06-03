#include <drivers/renderer_context.hpp>
#include <core/engine_logger.hpp>
#include <core/application.hpp>
#include <drivers/vulkan-cpp/vk_renderer.hpp>

namespace atlas {

    scope<render_context> initialize_renderer(
      const vk::vk_swapchain& p_swapchain_handler,
      const std::string& p_tag) {
        switch (application::current_api()) {
            case API::VULKAN:
                return create_scope<vk::vk_renderer>(p_swapchain_handler,
                                                     p_tag);
            default:
                return nullptr;
        }
    }
};