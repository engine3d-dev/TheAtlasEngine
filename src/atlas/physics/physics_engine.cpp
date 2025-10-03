#include <physics/physics_engine.hpp>
#include <drivers/jolt-cpp/types.hpp>
#include <drivers/jolt-cpp/jolt-imports.hpp>
#include <Jolt/Physics/Body/BodyLockMulti.h>
#include <core/application.hpp>
#include <flecs.h>

namespace atlas::physics {

    physics_engine::physics_engine(const jolt_settings& p_settings, flecs::world& p_registry) : m_registry(&p_registry) {
        m_physics_context = initialize_physics_context(p_settings);
        m_query_box_collider = m_registry->query<transform, physics_body, box_collider>();
        m_query_sphere_collider = m_registry->query<transform, physics_body, sphere_collider>();
        m_query_capsule_collider = m_registry->query<transform, physics_body, capsule_collider>();
    };

    void physics_engine::start() {
        m_query_box_collider.each([&](flecs::entity p_entity, transform& p_transform, physics_body& p_body, box_collider& p_collider){
            m_physics_context->add_box_collider(p_entity.id(), &p_transform, &p_body, &p_collider);
        });

        m_query_sphere_collider.each([this](flecs::entity p_entity, transform& p_transform, physics_body& p_body, sphere_collider& p_collider){
            m_physics_context->add_sphere_collider(p_entity.id(), &p_transform, &p_body, &p_collider);
        });

        m_query_capsule_collider.each([this](flecs::entity p_entity, transform& p_transform, physics_body& p_body, capsule_collider& p_collider){
            m_physics_context->add_capsule_collider(p_entity.id(), &p_transform, &p_body, &p_collider);
        });
        m_physics_context->prepare();
    }

    void physics_engine::update(float p_delta_time) {
        using namespace JPH;
        m_physics_context->update(p_delta_time);

        m_query_box_collider.each(
          [&](flecs::entity p_entity, transform& p_transform, physics_body& p_body, box_collider&) {
            // Updating the transform parameter properties
            transform t = m_physics_context->read_transform(p_entity.id());
            p_transform.position = t.position;
            p_transform.rotation = t.rotation;
            p_transform.quaternion = t.quaternion;

            // Handle updating the physics body parameters here
            auto body = m_physics_context->read_physics_body(p_entity.id());
            p_body.linear_damping = body.linear_damping;
            p_body.linear_velocity = body.linear_velocity;
            p_body.angular_velocity = body.angular_velocity;
            p_body.gravity_factor = body.gravity_factor;
            p_body.center_mass_position = body.center_mass_position;
            p_body.friction = body.friction;
            p_body.restitution = body.restitution;
            p_body.body_type = body.body_type;
        });

        m_query_sphere_collider.each([this](flecs::entity p_entity, transform& p_transform, physics_body& p_body, sphere_collider&){
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
            p_body.body_type = body.body_type;
        });

        m_query_capsule_collider.each([this](flecs::entity p_entity, transform& p_transform, physics_body& p_body, capsule_collider&){
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
            p_body.body_type = body.body_type;
        });

        // Execute collisions that happen with the collision manager
        // m_physics_context->update_collision_events();
    }

    void physics_engine::stop() {
        m_physics_context->destroy();
    }

}