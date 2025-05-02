#pragma once

#include <physics/physics_3d/data/physics_body.hpp>
#include <physics/physics_3d/physics_engine.hpp>
#include <core/scene/scene_object.hpp>

namespace atlas::physics {

    enum physics_backend : uint8_t { JoltBackend };

    ref<physics_engine> initialize_engine(
      const ref<scene_object>& p_physics_object);

    void add_force(glm::vec3 force, physics_body body);

    void add_force_world_point(glm::vec3 force,
                               glm::vec3 position,
                               physics_body body);

    void add_force_on_contact(glm::vec3 force, physics_body body);

    void set_velocity(const glm::vec3& velocity, physics_body body);

    void set_angular_velocity(const glm::vec3& angular_velocity,
                              physics_body body);

    void set_angular_momentum(const glm::vec3& angular_momentum,
                              physics_body body);

};