#pragma once
#include <physics/jolt-cpp/jolt_components.hpp>

namespace atlas::physics {

    class physics_api {
    public:
        /**
         * @brief Allows for flecs values to sync with jolt without running into
         * collisions.
         * @note Takes all transforms, rigidbodies, and collider bodies
         * that changed and updates jolt.
         *
         * @note should be called either by the user to sync events or
         * just before physics_update()
         */
        void update_jolt_values();

        /**
         * @brief Allows for jolt values to sync with flecs.
         *
         * @note should be called either by user to sync events or
         * just after physics_update()
         */
        void update_atlas_values();

        /**
         * @brief Calls the corresponding virtuals
         *
         */

        void add_force_by_body(const glm::vec3& p_force,
                               const physics_body& p_body);

        /**
         * @brief FIXME: This one is really complicated because it needs to try
         * over multiple frames. And locks are requried. So will be implemented
         * later.
         *
         */
        // void add_force_by_world_point();

        void add_linear_velocity_by_body(const glm::vec3& p_velocity,
                                         const physics_body& p_body);

        void add_angular_velocity_by_body(const glm::vec3& p_angular_velocity,
                                          const physics_body& p_body);

        void set_linear_velocity_by_body(const glm::vec3& p_velocity,
                                         const physics_body& p_body);

        void set_angular_velocity_by_body(const glm::vec3& p_angular_velocity,
                                          const physics_body& p_body);

        virtual ~physics_api() = default;

    private:
        /**
         * @brief Retrieves Flecs values for Jolt
         *
         */
        virtual void retrieve_values() = 0;

        /**
         * @brief Returns jolt values to flecs
         *
         */
        virtual void return_values() = 0;

        /**
         * @brief Adds a force to a single body/entity
         *
         */
        virtual void add_force(const glm::vec3& p_force,
                               const uint32_t& p_body_id) = 0;

        /**
         * @brief Adds a force at a world position
         *
         * @brief FIXME: This one is really complicated because it needs to try
         * over multiple frames. And locks are requried. So will be implemented
         * later.
         *
         */

        // virtual void add_force_world_point(const glm::vec3& force,
        //                                    const glm::vec3& position,
        //                                    const uint32_t& body_id) = 0;

        /**
         * @brief Sets linear velocity of one object
         *
         */
        virtual void add_linear_velocity(const glm::vec3& p_velocity,
                                         const uint32_t& p_body_id) = 0;

        /**
         * @brief Adds angular velocity of one object
         *
         */
        virtual void add_angular_velocity(const glm::vec3& angular_velocity,
                                          const uint32_t& body_id) = 0;

        /**
         * @brief Adds linear velocity of one object
         *
         */
        virtual void set_linear_velocity(const glm::vec3& p_velocity,
                                         const uint32_t& p_body_id) = 0;

        /**
         * @brief Sets angular velocity of one object
         *
         */
        virtual void set_angular_velocity(const glm::vec3& angular_velocity,
                                          const uint32_t& body_id) = 0;
    };
};