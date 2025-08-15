#include <drivers/renderer_context.hpp>
#include <core/engine_logger.hpp>
#include <core/application.hpp>
#include <drivers/vulkan-cpp/vk_renderer.hpp>

namespace atlas {

    scope<render_context> initialize_renderer(
      const atlas::ref<vk::vk_swapchain>& p_swapchain,
      const std::string& p_tag) {
        switch (application::current_api()) {
            case api::vulkan:
                return create_scope<vk::vk_renderer>(p_swapchain, p_tag);
            default:
                return nullptr;
        }
    }
};
