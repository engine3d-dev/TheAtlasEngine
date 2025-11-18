#pragma once
#include <vulkan-cpp/texture.hpp>

namespace atlas::vk {
    /**
     * @brief Represents a vulkan specific mateiral to pass in two textures and
     * offset for handling shininess
     */
    struct material_uniform {
        float shininess = 1.f;
    };
};