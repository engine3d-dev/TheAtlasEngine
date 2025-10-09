#include <drivers/renderer_context.hpp>
#include <core/engine_logger.hpp>
#include <core/application.hpp>
#include <drivers/vulkan-cpp/vk_renderer.hpp>

namespace atlas {

    scope<render_context> initialize_renderer(
      const window_settings& p_window_extent,
      uint32_t p_image_size,
      const std::string& p_tag) {
        switch (application::current_api()) {
            case api::vulkan:
                return create_scope<vk::vk_renderer>(p_window_extent, p_image_size, p_tag);
            default:
                return nullptr;
        }
    }
};
