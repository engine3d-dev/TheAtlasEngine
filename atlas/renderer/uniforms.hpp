#pragma once
#include <core/scene/components.hpp>
#include <array>

namespace atlas {

    struct light_scene_ubo {
        uint32_t num_lights;
        // alignas(16) std::array<point_light, 10> light_sources;
        alignas(16) point_light light_sources[10];
    };
};