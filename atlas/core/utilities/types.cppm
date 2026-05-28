module;

#include <cstdint>
#include <string>
#include <vulkan/vulkan.h>

export module atlas.core.utilities.types;

import vk;

export namespace atlas {
    struct window_params {
        uint32_t width;
        uint32_t height;
        std::string name = "";
        vk::surface_params surface_properties{};
    };

    // TODO: Move this into atlas.drivers.vulkan.utilities
    struct surface_properties {
        VkSurfaceCapabilitiesKHR surface_capabilities;
        VkSurfaceFormatKHR surface_format;
    };
};