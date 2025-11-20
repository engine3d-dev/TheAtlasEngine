#pragma once
#include <core/scene/components.hpp>
#include <array>

namespace atlas {

    struct point_light_ubo {
        glm::vec4 position; // this is provided by the transform
        glm::vec4 color = { 1.f, 1.f, 1.f, 1.f };
        float attenuation = 1.f;
        float constant = 1.f;
        float linear = 1.f;
        float quadratic = 1.f;

        glm::vec4 ambient = glm::vec4(1.f);
        glm::vec4 diffuse = glm::vec4(1.f);
        glm::vec4 specular = glm::vec4(1.f);
    };

    struct light_scene_ubo {
        alignas(16) uint32_t num_lights;
        // alignas(16) std::array<point_light, 10> light_sources;
        alignas(16) point_light_ubo light_sources[10];
    };
};