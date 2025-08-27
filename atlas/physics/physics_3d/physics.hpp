#pragma once

#include <physics/physics_3d/physics_engine.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>
#include <core/scene/scene_object.hpp>

namespace atlas::physics {
    /**
     * @brief This is the manager of all three api for physics. It contians the
     * scope for all physics engine related functions. It passes the jolt_vlaues
     * to engine and gives them to the user api.
     *
     */
    enum physics_backend : uint8_t { jolt_backend };

    /**
     * @brief Creates the engine and all three api - collision, user, and engine
     * api.
     *
     * @param p_physics_object
     * @param p_registery
     * @return ref<physics_engine>
     */
    strong_ref<physics_engine> initialize_engine(
      std::pmr::polymorphic_allocator<>& p_object_allocator,
      const strong_ref<scene_object>& p_physics_object,
      flecs::world& p_registery);

    /**
     * @brief These functions are the generic functions implmented in jolt but
     * should be able to switch to any engine.
     *
     */
    void add_force(const glm::vec3& force, const physics_body& body);

    /**
     * @brief FIXME: This is a difficult funciton that requires locks and
     * waiting multiple frames before running. So it is later.
     *
     */
    // void add_force_world_point(glm::vec3 force,
    //   glm::vec3 position,
    //   physics_body body);

    void add_velocity(const glm::vec3& velocity, const physics_body& body);

    void add_angular_velocity(const glm::vec3& angular_velocity,
                              const physics_body& body);

    void set_velocity(const glm::vec3& velocity, const physics_body& body);

    void set_angular_velocity(const glm::vec3& angular_velocity,
                              const physics_body& body);

};