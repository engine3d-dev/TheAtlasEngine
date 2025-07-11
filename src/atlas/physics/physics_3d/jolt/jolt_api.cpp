#include <physics/physics_3d/jolt/jolt_helper.hpp>
#include <physics/physics_3d/jolt/jolt_api.hpp>
#include <Jolt/Physics/Body/BodyLockMulti.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <core/engine_logger.hpp>

namespace atlas::physics {

    jolt_api::jolt_api(const jolt_config& p_config,
                       const ref<JPH::PhysicsSystem>& p_physics_system,
                       const flecs::world& p_registery)
      : m_config(p_config) {

        m_physics_system = p_physics_system;
        m_registery = p_registery;

        m_read_transform = p_registery.query<transform, collider_body>();
        m_query_transform = p_registery.query<transform, collider_body>();

        m_read_body = p_registery.query<physics_body>();
        m_query_body = p_registery.query<physics_body>();
    }

    void jolt_api::retrieve_values() {
        JPH::BodyInterface& interface = m_physics_system->GetBodyInterface();

        m_physics_system->SetGravity(to_jph(m_config.gravity));

        m_read_transform.each([&](flecs::entity,
                                  const transform& transform,
                                  const collider_body& collider_body) {
            const JPH::BodyID id = JPH::BodyID(collider_body.body_id);

            JPH::Vec3 pos = to_jph(transform.position);

            JPH::Quat rot(transform.quaternion.x,
                          transform.quaternion.y,
                          transform.quaternion.z,
                          transform.quaternion.w);

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

        JPH::BodyIDVector locked_body_ids;
        int counter = 0;
        m_query_body.each([&](flecs::entity, physics_body& p_body) {
            auto body = JPH::BodyID(p_body.body_id);
            locked_body_ids.push_back(body);
            p_body.count = counter;
            counter++;
        });
        {
            JPH::BodyLockMultiRead lock(
              m_physics_system->GetBodyLockInterfaceNoLock(),
              &(locked_body_ids[0]),
              (int)locked_body_ids.size());

            m_read_body.each([&](flecs::entity, physics_body& body) {
                const JPH::BodyID id = JPH::BodyID(body.body_id);
                const JPH::Body* locked_body = lock.GetBody(body.count);

                JPH::Vec3 force = locked_body->GetAccumulatedForce();
                JPH::Vec3 torque = locked_body->GetAccumulatedTorque();

                body.cumulative_force =
                  glm::vec3(force.GetX(), force.GetY(), force.GetZ());
                body.cumulative_torque =
                  glm::vec3(torque.GetX(), torque.GetY(), torque.GetZ());

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
    }

    void jolt_api::return_values() {
        JPH::BodyInterface& interface = m_physics_system->GetBodyInterface();

        m_query_transform.each(
          [&](flecs::entity, transform& transform, collider_body& collider) {
              const JPH::BodyID id = JPH::BodyID(collider.body_id);

              JPH::Vec3 pos = interface.GetPosition(id);
              JPH::Quat rot = interface.GetRotation(id);
              JPH::Vec3 rot_euler = rot.GetEulerAngles();

              transform.position =
                glm::vec3(pos.GetX(), pos.GetY(), pos.GetZ());
              transform.quaternion =
                glm::vec4(rot.GetX(), rot.GetY(), rot.GetZ(), rot.GetW());
              transform.rotation =
                glm::vec3(rot_euler.GetX(), rot_euler.GetY(), rot_euler.GetZ());
          });

        m_query_body.each([&](flecs::entity, physics_body& body) {
            const JPH::BodyID id = JPH::BodyID(body.body_id);

            JPH::Vec3 linear_velocity = JPH::Vec3::sZero();
            JPH::Vec3 angular_velocity = JPH::Vec3::sZero();

            interface.GetLinearAndAngularVelocity(
              id, linear_velocity, angular_velocity);

            if (linear_velocity.IsClose({ 0, 0, 0 }, 0.0001f))
                linear_velocity = { 0, 0, 0 };

            if (angular_velocity.IsClose({ 0, 0, 0 }, 0.0001f))
                angular_velocity = { 0, 0, 0 };

            body.linear_velocity = glm::vec3(linear_velocity.GetX(),
                                             linear_velocity.GetY(),
                                             linear_velocity.GetZ());
            body.angular_velocity = glm::vec3(angular_velocity.GetX(),
                                              angular_velocity.GetY(),
                                              angular_velocity.GetZ());

            JPH::Vec3 center_mass = interface.GetCenterOfMassPosition(id);
            body.center_mass_position = glm::vec3(
              center_mass.GetX(), center_mass.GetY(), center_mass.GetZ());

            body.gravity_factor = interface.GetGravityFactor(id);
            body.friction = interface.GetFriction(id);
            body.restitution = interface.GetRestitution(id);
            body.body_type = static_cast<uint8_t>(interface.GetMotionType(id));
        });
    }

    void jolt_api::add_force(const glm::vec3& p_force,
                             const uint32_t& p_body_id) {
        JPH::BodyID body_id(p_body_id);

        m_physics_system->GetBodyInterface().AddForce(body_id, to_jph(p_force));
    }

    void jolt_api::add_linear_velocity(const glm::vec3& p_linear_velocity,
                                       const uint32_t& p_body_id) {
        JPH::BodyID body_id(p_body_id);

        m_physics_system->GetBodyInterface().AddLinearVelocity(
          body_id, to_jph(p_linear_velocity));
    }

    void jolt_api::add_angular_velocity(const glm::vec3& p_angular_velocity,
                                        const uint32_t& p_body_id) {
        JPH::BodyID body_id(p_body_id);

        m_physics_system->GetBodyInterface().AddLinearAndAngularVelocity(
          body_id,
          m_physics_system->GetBodyInterface().GetLinearVelocity(body_id),
          to_jph(p_angular_velocity));
    }

    void jolt_api::set_linear_velocity(const glm::vec3& p_velocity,
                                       const uint32_t& p_body_id) {
        JPH::BodyID body_id(p_body_id);

        m_physics_system->GetBodyInterface().SetLinearVelocity(
          body_id, to_jph(p_velocity));
    }

    void jolt_api::set_angular_velocity(const glm::vec3& p_angular_velocity,
                                        const uint32_t& p_body_id) {
        JPH::BodyID body_id(p_body_id);

        m_physics_system->GetBodyInterface().SetAngularVelocity(
          body_id, to_jph(p_angular_velocity));
    }
}