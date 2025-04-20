#pragma once

#include <core/scene/scene_object.hpp>
#include <physics/physics_3d/data/physics_body.hpp>
#include <physics/physics_3d/physics_context.hpp>

namespace atlas::physics {
    enum physics_api : uint8_t { Jolt };

    physics_api test_api = Jolt;

    ref<physics_context> initialize_engine(
      const ref<scene_object>& p_physics_object) 
      {
        switch (test_api) {
            case physics_api::Jolt:
                return create_ref<physics_context>(
                  *p_physics_object->get<physics_settings>());
            default:
                return nullptr;
        }
        return nullptr;
    }

    void add_force(glm::vec3 force, physics_body);

    void add_force_world_point(glm::vec3 force, glm::vec3 position, );

    void add_force_on_contact(glm::vec3 force, );
};