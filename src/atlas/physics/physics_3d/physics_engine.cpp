#include <physics/physics_3d/physics_engine.hpp>
#include <drivers/jolt-cpp/jolt_api.hpp>
#include <drivers/jolt-cpp/jolt_helper.hpp>
#include <drivers/jolt-cpp/jolt-imports.hpp>
#include <Jolt/Physics/Body/BodyLockMulti.h>
#include <core/application.hpp>

namespace atlas::physics {

    physics_engine::physics_engine(const jolt_settings& p_settings, const jolt_config& p_config, flecs::world& p_registry) : m_registry(&p_registry), m_jolt_config(p_config) {
        m_physics_context = initialize_physics_context(p_settings);
        m_query_transform = m_registry->query<transform, collider_body>();
        m_query_body = m_registry->query<physics_body>();
        m_query_box_collider = m_registry->query<physics_body, box_collider, transform>();
    };

    void physics_engine::start() {
        // m_physics_context->add_bodies();
        m_query_box_collider.each([&](flecs::entity p_entity, physics_body& p_body, box_collider& p_collider, transform&){
            console_log_warn("Create Bodies!");
            m_physics_context->add_box_collider(p_entity, &p_body, &p_collider);
        });
        m_physics_context->prepare();
    }

    void physics_engine::update(float p_delta_time) {
        using namespace JPH;
        auto& physics_system = m_physics_context->physics_instance();
        physics_system->SetGravity(to_jph(m_jolt_config.gravity));
        m_physics_context->update(p_delta_time);
        // JPH::BodyInterface& interface = physics_system->GetBodyInterface();

        m_query_box_collider.each(
          [&](flecs::entity p_entity, physics_body& p_body, box_collider&, transform& p_transform) {
            // Updating the transform parameter properties
            transform t = m_physics_context->read_transform(p_entity.id());
            p_transform.position = t.position;
            p_transform.rotation = t.rotation;
            p_transform.quaternion = t.quaternion;
            //   const JPH::BodyID id = JPH::BodyID(collider.body_id);

            //   JPH::Vec3 pos = interface.GetPosition(id);
            //   JPH::Quat rot = interface.GetRotation(id);
            //   JPH::Vec3 rot_euler = rot.GetEulerAngles();

            //   transform.position =
            //     glm::vec3(pos.GetX(), pos.GetY(), pos.GetZ());
            //   transform.quaternion =
            //     glm::vec4(rot.GetX(), rot.GetY(), rot.GetZ(), rot.GetW());
            //   transform.rotation =
            //     glm::vec3(rot_euler.GetX(), rot_euler.GetY(), rot_euler.GetZ());

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
            // const JPH::BodyID id = JPH::BodyID(body.body_id);

            // JPH::Vec3 linear_velocity = JPH::Vec3::sZero();
            // JPH::Vec3 angular_velocity = JPH::Vec3::sZero();

            // interface.GetLinearAndAngularVelocity(
            //   id, linear_velocity, angular_velocity);

            // if (linear_velocity.IsClose({ 0, 0, 0 }, 0.0001f))
            //     linear_velocity = { 0, 0, 0 };

            // if (angular_velocity.IsClose({ 0, 0, 0 }, 0.0001f))
            //     angular_velocity = { 0, 0, 0 };
            
            // // TODO: Provide read_linear_velocity and read_angular_velocity(uint32_t p_id);
            // body.linear_velocity = glm::vec3(linear_velocity.GetX(),
            //                                  linear_velocity.GetY(),
            //                                  linear_velocity.GetZ());
            // body.angular_velocity = glm::vec3(angular_velocity.GetX(),
            //                                   angular_velocity.GetY(),
            //                                   angular_velocity.GetZ());

            // JPH::Vec3 center_mass = interface.GetCenterOfMassPosition(id);
            // body.center_mass_position = glm::vec3(
            //   center_mass.GetX(), center_mass.GetY(), center_mass.GetZ());

            // body.gravity_factor = interface.GetGravityFactor(id);
            // body.friction = interface.GetFriction(id);
            // body.restitution = interface.GetRestitution(id);
            // body.body_type = static_cast<uint8_t>(interface.GetMotionType(id));
        });

        // Execute collisions that happen with the collision manager
        m_physics_context->update_collision_events();
    }

    void physics_engine::stop() {
        m_physics_context->destroy();
    }

}