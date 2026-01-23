module;

#include <Jolt/Jolt.h>
#include <flecs.h>

export module atlas.physics.engine;

import atlas.common;
import atlas.core.scene.components;
import atlas.core.event;
import atlas.drivers.physics_context;
import atlas.drivers.jolt_cpp.context;

namespace atlas::physics {

    //! @brief initializes the physics backend. SHOULD have an API associated
    //! with but for now, we assume we only have JoltPhysics as our only physics
    //! backend
    ref<physics_context> initialize_physics_context(event::bus& p_bus) {
        return create_ref<jolt_context>(p_bus);
    }

    /**
     * @brief The manager class for all physics engines. Manages the physics
     * contexts and the collision engines.
     *
     */
    export class engine {
    public:
        // Required by maps but should not be used in anyother circumstance.
        engine() = default;
        engine(flecs::world& p_registry,
                       event::bus& p_bus) : m_registry(&p_registry), m_bus(&p_bus) {
            m_physics_context = initialize_physics_context(*m_bus);

            // This may change, but for now we want to ensure that we only want to
            // create a single physics body with a specific collider Rather then
            // having multiple colliders be associated to a single physics body
            // itself.
            m_query_box_collider =
            m_registry->query_builder<transform, physics_body, box_collider>()
                .without<sphere_collider, capsule_collider>()
                .build();
            m_query_sphere_collider =
            m_registry->query_builder<transform, physics_body, sphere_collider>()
                .without<box_collider, capsule_collider>()
                .build();
            m_query_capsule_collider =
            m_registry->query_builder<transform, physics_body, capsule_collider>()
                .without<sphere_collider, box_collider>()
                .build();

            m_physics_bodies = m_registry->query_builder<physics_body>().build();
        }

        /**
         * @brief The runtime is specifically the way to start the physics,
         * utilizing defined data from level_scene and eventually any scene to
         * create a physics scene and manage physics step.
         *
         */
        void start() {
            // At the start of every simulation we create physics bodies to run
            // simulation on those physics bodies with the specific colliders
            // associated with them
            m_query_box_collider.each([this](flecs::entity p_entity,
                                            transform& p_transform,
                                            physics_body& p_body,
                                            box_collider& p_collider) {
                m_physics_context->add_box_collider(
                p_entity.id(), &p_transform, &p_body, &p_collider);
            });

            m_query_sphere_collider.each([this](flecs::entity p_entity,
                                                transform& p_transform,
                                                physics_body& p_body,
                                                sphere_collider& p_collider) {
                m_physics_context->add_sphere_collider(
                p_entity.id(), &p_transform, &p_body, &p_collider);
            });

            m_query_capsule_collider.each([this](flecs::entity p_entity,
                                                transform& p_transform,
                                                physics_body& p_body,
                                                capsule_collider& p_collider) {
                m_physics_context->add_capsule_collider(
                p_entity.id(), &p_transform, &p_body, &p_collider);
            });
            m_physics_context->prepare();
        }

        /**
         * @brief Runs a single frame of the simulation. Should at minimum be
         * called 60 fps. If below, it must be called twice.
         *
         */
        void update(float p_delta_time) {
            using namespace JPH;

            m_physics_bodies.each(
            [this](flecs::entity p_entity, physics_body& p_body) {
                m_physics_context->set_force_and_torque(
                    p_entity.id(), p_body.force, p_body.torque);
                m_physics_context->set_linear_velocity(p_entity.id(),
                                                        p_body.linear_velocity);
                m_physics_context->set_angular_velocity(p_entity.id(),
                                                        p_body.angular_velocity);
                m_physics_context->set_impulse(p_entity.id(), p_body.impulse);
            });
            // This will ensure all physics bodies with which colliders they are
            // associated with are update with the simulation, and their parameters
            // are modified
            m_physics_context->update(p_delta_time);

            m_query_box_collider.each([&](flecs::entity p_entity,
                                        transform& p_transform,
                                        physics_body& p_body,
                                        box_collider&) {
                // updating transform
                transform t = m_physics_context->read_transform(p_entity.id());
                p_transform.position = t.position;
                p_transform.rotation = t.rotation;
                p_transform.quaternion = t.quaternion;

                // physics bodies parameters
                auto body = m_physics_context->read_physics_body(p_entity.id());
                p_body.linear_damping = body.linear_damping;
                p_body.linear_velocity = body.linear_velocity;
                p_body.angular_velocity = body.angular_velocity;
                p_body.gravity_factor = body.gravity_factor;
                p_body.center_mass_position = body.center_mass_position;
                p_body.friction = body.friction;
                p_body.restitution = body.restitution;
                p_body.angular_velocity = body.angular_velocity;
                p_body.linear_velocity = body.linear_velocity;
            });

            // updating sphere collider
            m_query_sphere_collider.each([this](flecs::entity p_entity,
                                                transform& p_transform,
                                                physics_body& p_body,
                                                sphere_collider&) {
                // updating transform
                transform t = m_physics_context->read_transform(p_entity.id());
                p_transform.position = t.position;
                p_transform.rotation = t.rotation;
                p_transform.quaternion = t.quaternion;

                // updating physics body
                auto body = m_physics_context->read_physics_body(p_entity.id());
                p_body.linear_damping = body.linear_damping;
                p_body.linear_velocity = body.linear_velocity;
                p_body.angular_velocity = body.angular_velocity;
                p_body.gravity_factor = body.gravity_factor;
                p_body.center_mass_position = body.center_mass_position;
                p_body.friction = body.friction;
                p_body.restitution = body.restitution;
            });

            // updating capsule collider
            m_query_capsule_collider.each([this](flecs::entity p_entity,
                                                transform& p_transform,
                                                physics_body& p_body,
                                                capsule_collider&) {
                transform t = m_physics_context->read_transform(p_entity.id());
                p_transform.position = t.position;
                p_transform.rotation = t.rotation;
                p_transform.quaternion = t.quaternion;

                auto body = m_physics_context->read_physics_body(p_entity.id());
                p_body.linear_damping = body.linear_damping;
                p_body.linear_velocity = body.linear_velocity;
                p_body.angular_velocity = body.angular_velocity;
                p_body.gravity_factor = body.gravity_factor;
                p_body.center_mass_position = body.center_mass_position;
                p_body.friction = body.friction;
                p_body.restitution = body.restitution;
            });
        }

        /**
         * @brief Deletes all physics bodies and shapes. Preps itself for
         * runtime to be called again.
         *
         */
        void stop() {
            m_physics_context->destroy();
        }

    private:
        flecs::world* m_registry;
        ref<physics_context> m_physics_context;
        flecs::query<physics_body> m_physics_bodies;
        flecs::query<transform, physics_body, box_collider>
          m_query_box_collider;
        flecs::query<transform, physics_body, sphere_collider>
          m_query_sphere_collider;
        flecs::query<transform, physics_body, capsule_collider>
          m_query_capsule_collider;
        event::bus* m_bus;
    };
};