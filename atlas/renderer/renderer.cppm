module;

#include <string>
#include <cstdint>

export module atlas.renderer;

import atlas.common;
import atlas.drivers.renderer_system;
import atlas.drivers.vulkan.render_system;
import atlas.common;
import atlas.graphics_api;
import atlas.drivers.graphics_context;
import atlas.core.utilities.types;

export namespace atlas {
    ref<renderer_system> initialize_renderer(
      ref<graphics_context> p_context,
      graphics_api p_api,
      const window_params& p_window_extent,
      uint32_t p_image_size,
      const std::string& p_name) {
        switch (p_api) {
            case graphics_api::vulkan:
                return create_ref<vulkan::render_system>(
                  p_context, p_window_extent, p_image_size, p_name);
            default:
                return nullptr;
        }
    }
};