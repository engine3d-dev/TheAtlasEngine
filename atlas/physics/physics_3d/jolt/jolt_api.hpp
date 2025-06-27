#pragma once
#include <core/core.hpp>
#include <flecs.h>
#include <physics/physics_3d/physics_api.hpp>
#include <scene/components.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>

/**
 * @brief This is the how the user is able to call functions relating to jolt.
 * It is the api calls for the user. Most of them running in runtime. With a
 * couple effecting config which is a system wide change. This api is deticated
 * to user interaction, however a few functions will be used by both the engine
 * and the user.
 *
 */
namespace atlas::physics {
    class jolt_api : public physics_api {
    public:
        /**
         * @brief Construct a new jolt api object
         *
         * @param p_config All system wide runtime configurations
         * @param p_physics_system Passed by the engine to allow interaction
         * between the engine and users
         * @param p_registery The world this api is associated with.
         * FIXME: This is a temperary solve until have a funciton like
         * get_active_scenes()
         */
        jolt_api(const jolt_config& p_config,
                 const ref<JPH::PhysicsSystem>& p_physics_system,
                 const flecs::world& p_registery);

    private:
        /**
         * @brief Sends all atlas values to jolt
         *
         */
        void retrieve_values() override;

        /**
         * @brief Sends all jolt values to atlas
         *
         */
        void return_values() override;

        /**
         * @brief Targets a body and adds a force.
         *
         * @param force Direction and strength of force.
         * @param body_id Gets the body in the physics world.
         */
        void add_force(const glm::vec3& p_force,
                       const uint32_t& p_body_id) override;

        /**
         * @brief Adds linear velocity
         *
         * @param p_angular_velocity Direction and strength of velocity
         * @param p_body_id Gets the body in the physics world.
         */

        void add_linear_velocity(const glm::vec3& p_linear_velocity,
                                 const uint32_t& p_body_id) override;
        /**
         * @brief Adds angular valocity
         *
         * @param p_angular_velocity Direction and strength of velocity
         * @param p_body_id Gets the body in the physics world.
         */

        void add_angular_velocity(const glm::vec3& p_angular_velocity,
                                  const uint32_t& p_body_id) override;

        /**
         * @brief Set the linear velocity of an object
         *
         * @param velocity Direction and strength of velocity
         * @param body_id Gets the body in the physics world.
         */
        void set_linear_velocity(const glm::vec3& p_velocity,
                                 const uint32_t& p_body_id) override;

        /**
         * @brief Set the angular velocity of an object
         *
         * @param angular_velocity Direction and strength of angular velocity
         * @param body_id Gets the body in the physics world.
         */
        void set_angular_velocity(const glm::vec3& p_angular_velocity,
                                  const uint32_t& p_body_id) override;

        jolt_config m_config;
        ref<JPH::PhysicsSystem> m_physics_system;
        flecs::world m_registery;

        /**
         * @brief Flecs querys deticated to reading and writing from jolt and
         * atlas. Targeting transforms and colliders.
         *
         */
        flecs::query<transform, collider_body> m_read_transform;
        flecs::query<physics_body> m_read_body;

        /**
         * @brief Flecs querys deticated to reading and writing from jolt and
         * atlas. Targeting physics bodies.
         *
         */
        flecs::query<transform, collider_body> m_query_transform;
        flecs::query<physics_body> m_query_body;
    };
};