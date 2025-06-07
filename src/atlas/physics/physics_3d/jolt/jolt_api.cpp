#include <physics/physics_3d/jolt/jolt_helper.hpp>
#include <physics/physics_3d/jolt/jolt_api.hpp>
#include <core/engine_logger.hpp>

namespace atlas::physics {

    jolt_api::jolt_api(jolt_config& p_config,
                       ref<JPH::PhysicsSystem>& p_physics_system,
                       flecs::world& p_registery)
      : m_config(p_config)
      , m_physics_system(p_physics_system) {
        m_registery = p_registery;

        m_read_transform = p_registery.query<transform, collider_body>();
        m_query_transform = p_registery.query<transform, collider_body>();

        m_read_body = p_registery.query<physics_body>();
        m_query_body = p_registery.query<physics_body>();
    }

    void jolt_api::retrieve_values() {
        JPH::BodyInterface& interface = m_physics_system->GetBodyInterface();

        m_physics_system->SetGravity( to_jph(m_config.gravity));

        m_read_transform.each([&](flecs::entity,
                                  const transform& transform,
                                  const collider_body& collider_body) {
            const JPH::BodyID id = JPH::BodyID(collider_body.body_id);

            JPH::Vec3 pos = to_jph(transform.Position);

            glm::quat quaternion = glm::quat(transform.Rotation);
            JPH::Quat rot(
              quaternion.x, quaternion.y, quaternion.z, quaternion.w);

            interface.SetPositionAndRotation(
              id, pos, rot, JPH::EActivation::Activate);


            if (!collider_body.collision_enabled && interface.IsAdded(id)) {
                interface.RemoveBody(id);
            }
            else if (collider_body.collision_enabled &&
                     !interface.IsAdded(id)) {
                interface.AddBody(id, JPH::EActivation::Activate);
            }
        });

        m_read_body.each([&](flecs::entity, const physics_body& body) {
            const JPH::BodyID id = JPH::BodyID(body.body_id);

            interface.SetGravityFactor(
              id, body.use_gravity ? body.gravity_factor : 0.0f);
            interface.SetFriction(id, body.friction);
            interface.SetRestitution(id, body.restitution);
            interface.SetMotionType(
              id,
              static_cast<JPH::EMotionType>(body.body_type),
              JPH::EActivation::Activate);
        });
    }

    void jolt_api::return_values() {
        JPH::BodyInterface& interface = m_physics_system->GetBodyInterface();

        m_query_transform.each([&](flecs::entity,
                                   transform& transform,
                                   collider_body& collider) {
            const JPH::BodyID id = JPH::BodyID(collider.body_id);
             JPH::BodyIDVector vector;
        m_physics_system->GetBodies(vector);
      

            const JPH::Vec3 pos = interface.GetPosition(id);
            const JPH::Quat rot = interface.GetRotation(id);
            const JPH::Vec3 rot_euler = rot.GetEulerAngles();


            transform.Position = glm::vec3(pos.GetX(), pos.GetY(), pos.GetZ());
            transform.QuaternionRotation =
              glm::vec4(rot.GetX(), rot.GetY(), rot.GetZ(), rot.GetW());
            transform.Rotation =
              glm::vec3(rot_euler.GetX(), rot_euler.GetY(), rot_euler.GetZ());

        });

        m_query_body.each([&](flecs::entity, physics_body& body) {
            const JPH::BodyID id = JPH::BodyID(body.body_id);

            JPH::Vec3 linear_velocity = JPH::Vec3::sZero();
            JPH::Vec3 angular_velocity = JPH::Vec3::sZero();
            interface.GetLinearAndAngularVelocity(
              id, linear_velocity, angular_velocity);

            body.linear_velocity = glm::vec3(linear_velocity.GetX(),
                                             linear_velocity.GetY(),
                                             linear_velocity.GetZ());
            body.angular_velocity = glm::vec3(angular_velocity.GetX(),
                                              angular_velocity.GetY(),
                                              angular_velocity.GetZ());

            JPH::Vec3 center_mass = interface.GetCenterOfMassPosition(id);
            body.center_mass_position = glm::vec3(
              center_mass.GetX(), center_mass.GetY(), center_mass.GetZ());

            body.friction = interface.GetFriction(id);
            body.restitution = interface.GetRestitution(id);
            body.body_type = static_cast<uint8_t>(interface.GetMotionType(id));

        });
    }
}