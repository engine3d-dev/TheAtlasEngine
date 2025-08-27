#include <core/engine_logger.hpp>
#include <physics/physics_3d/physics_api.hpp>

namespace atlas::physics {
    void physics_api::update_jolt_values() {
        retrieve_values();
    }

    void physics_api::update_atlas_values() {
        return_values();
    }

    void physics_api::add_force_by_body(const glm::vec3& p_force,
                                        const physics_body& p_body) {
        add_force(p_force, p_body.body_id);
    }

    void physics_api::add_linear_velocity_by_body(const glm::vec3& p_velocity,
                                                  const physics_body& p_body) {
        add_linear_velocity(p_velocity, p_body.body_id);
    }

    void physics_api::add_angular_velocity_by_body(const glm::vec3& p_velocity,
                                                   const physics_body& p_body) {
        add_angular_velocity(p_velocity, p_body.body_id);
    }

    void physics_api::set_linear_velocity_by_body(const glm::vec3& p_velocity,
                                                  const physics_body& p_body) {
        set_linear_velocity(p_velocity, p_body.body_id);
    }

    void physics_api::set_angular_velocity_by_body(const glm::vec3& p_velocity,
                                                   const physics_body& p_body) {
        set_angular_velocity(p_velocity, p_body.body_id);
    }
}