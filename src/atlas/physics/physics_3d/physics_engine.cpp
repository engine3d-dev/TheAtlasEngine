#include <physics/physics_3d/physics_engine.hpp>
#include <drivers/jolt-cpp/jolt_api.hpp>
#include <drivers/jolt-cpp/jolt_helper.hpp>
#include <drivers/jolt-cpp/jolt-imports.hpp>
#include <Jolt/Physics/Body/BodyLockMulti.h>
#include <core/application.hpp>

namespace atlas::physics {

    physics_engine::physics_engine(const jolt_settings& p_settings, const jolt_config& p_config, flecs::world* p_registry) : m_registry(p_registry), m_jolt_config(p_config) {
        // ref<jolt_context> engine_access = create_ref<jolt_context>(p_settings);
        // m_engine_api = create_ref<jolt_context>(p_settings);
        // m_engine_api = engine_access;
        m_physics_context = initialize_physics_context(p_settings);

        jolt_api user_api(p_config, m_physics_context->physics_instance(), *m_registry);

        m_backend_api = create_ref<jolt_api>(user_api);
        m_query_transform = p_registry->query<transform, collider_body>();
        m_query_body = p_registry->query<physics_body>();
    };

    void physics_engine::start() {
        m_physics_context->create_bodies();
    }

    void physics_engine::update(float p_delta_time) {
        using namespace JPH;
        auto& physics_system = m_physics_context->physics_instance();
        // JPH::BodyInterface& interface = physics_system->GetBodyInterface();
        physics_system->SetGravity(to_jph(m_jolt_config.gravity));
        // m_backend_api->update_jolt_values();

        // m_physics_context->run_physics_step();
        m_physics_context->update(p_delta_time);
        // float fixed_time_step = 1.0f / 60.0f;
        // int time_step = 1 + (int)(60 * fixed_time_step);
        // m_physics_context->update(application::delta_time());
        // m_physics_system->Update(application::delta_time(),
        //                             time_step,
        //                             m_temp_allocator.get(),
        //                             m_thread_system.get());
        // m_backend_api->update_atlas_values();
        JPH::BodyInterface& interface = physics_system->GetBodyInterface();

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

        // Execute collisions that happen with the collision manager
        m_physics_context->contact_added_event();
    }

    void physics_engine::stop() {
        m_physics_context->clean_bodies();
    }

}