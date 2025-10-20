#include <drivers/jolt-cpp/jolt_context.hpp>
#include <drivers/jolt-cpp/jolt-imports.hpp>
#include <physics/math_types.hpp>
#include <drivers/jolt-cpp/types.hpp>
#include <core/engine_logger.hpp>

namespace atlas::physics {
    static void trace_impl(const char* p_in_fmt, ...) {
        va_list list;
        va_start(list, p_in_fmt);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), p_in_fmt, list);
        va_end(list);
        console_log_error("{}", buffer);
    }

    [[maybe_unused]] static bool assert_failed_impl(const char* p_in_expression,
                                                    const char* p_in_message,
                                                    const char* p_in_file,
                                                    unsigned int p_in_line) {

        console_log_error("{}:{}: ({}) {}",
                          p_in_file,
                          p_in_line,
                          p_in_expression,
                          (p_in_message != nullptr ? p_in_message : ""));

        return true;
    };

    jolt_context::jolt_context(const jolt_settings& p_settings, event::event_bus& p_bus) : m_contact_listener(p_bus) {
        JPH::RegisterDefaultAllocator();

        JPH::Trace = trace_impl;
        JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = assert_failed_impl;)

        JPH::Factory::sInstance = new JPH::Factory();
        JPH::RegisterTypes();

        m_temp_allocator =
          create_ref<JPH::TempAllocatorImpl>(p_settings.allocation_amount);

        // This just sets up the JoltPhysics system and any listeners
        m_physics_system = create_ref<JPH::PhysicsSystem>();
        m_broad_phase_layer_interface =
          create_ref<broad_phase_layer_interface>();
        m_object_vs_broadphase_filter =
          create_ref<object_vs_broadphase_layer>();
        m_object_layer_pair_filter = create_ref<object_layer_pair_filter>();

        if (p_settings.thread_type == thread_type::default_system) {

            m_thread_system = create_scope<JPH::JobSystemThreadPool>(
              // Max jobs must be a power of 2, otherwise jph crashes.
              // Bianary tree must be fully balanced
              std::pow(2, p_settings.max_jobs_power),
              p_settings.max_barriers,
              p_settings.physics_threads);
        }
        else {
            console_log_error("Unsupported custom job system");
            assert(false);
        }

        m_physics_system->Init(p_settings.max_bodies,
                               0,
                               p_settings.max_body_pairs,
                               p_settings.max_contact_constraints,
                               *m_broad_phase_layer_interface,
                               *m_object_vs_broadphase_filter,
                               *m_object_layer_pair_filter);

        // Default contact listener impl and can change during runtime
        m_physics_system->SetContactListener(&m_contact_listener);
    }

    void jolt_context::emplace_box_collider(uint32_t p_entity_id,
                                            const transform* p_transform,
                                            const physics_body* p_body,
                                            const box_collider* p_collider) {
        using namespace JPH;
        auto& body_interface = m_physics_system->GetBodyInterface();

        // Creating our box shape and specifying half_extent that is a glm::vec3
        // conversion to JPH::Vec3 half_extents must be 0.5f or else it can get
        // an invalid convex radius
        BoxShapeSettings shape_settings(jolt::to_vec3(p_collider->half_extent));
        auto result = shape_settings.Create();

        if (result.HasError()) {
            console_log_error("Box shape creation error: {}",
                              result.GetError());
            return;
        }
        EMotionType motion_type = EMotionType::Static;
        switch (p_body->body_movement_type) {
            case body_type::fixed: {
                motion_type = EMotionType::Static;
                console_log_info("EMotionType::Static!");
            } break;
            case body_type::dynamic: {
                motion_type = EMotionType::Dynamic;
                console_log_info("EMotionType::Dynamic!");
            } break;
            case body_type::kinematic: {
                motion_type = EMotionType::Kinematic;
                console_log_info("EMotionType::Kinematic!");
            } break;
        }

        auto& box = result.Get();
        BodyCreationSettings body_settings(
          box,
          jolt::to_vec3(p_transform->position),
          jolt::to_quat(p_transform->quaternion),
          motion_type,
          p_body->body_layer_type);

        // NOTE TO  SELF ------ This is setting some pointer to the entity ID
        // WE CAN USE THIS TO TELL THE EVENT SYSTEM WHICH FLECS ENTITY COLLIDED
        // WITH EACH OTHER!!!!!!!! Because each contact listener allows you to
        // take a pointer from the physics bodies that are just blocks of
        // data!!!
        body_settings.mUserData = static_cast<uint64_t>(p_entity_id);
        body_settings.mFriction = p_body->friction;
        body_settings.mRestitution = p_body->restitution;
        body_settings.mLinearVelocity = jolt::to_vec3(p_body->linear_velocity);
        body_settings.mAngularVelocity =
          jolt::to_vec3(p_body->angular_velocity);

        Body* body = body_interface.CreateBody(body_settings);
        m_cached_body_ids.emplace(p_entity_id, body->GetID());
    }

    void jolt_context::emplace_sphere_collider(
      uint32_t p_entity_id,
      const transform* p_transform,
      const physics_body* p_body,
      const sphere_collider* p_collider) {
        using namespace JPH;
        auto& body_interface = m_physics_system->GetBodyInterface();
        // ensure that the half_extent of the box shape always matches the
        // object and reference that information through the transform
        SphereShapeSettings shape_settings(p_collider->radius);
        auto result = shape_settings.Create();

        if (result.HasError()) {
            console_log_error("Box shape creation error: {}",
                              result.GetError());
            return;
        }
        EMotionType motion_type = EMotionType::Static;
        switch (p_body->body_movement_type) {
            case body_type::fixed: {
                motion_type = EMotionType::Static;
            } break;
            case body_type::dynamic: {
                motion_type = EMotionType::Dynamic;
            } break;
            case body_type::kinematic: {
                motion_type = EMotionType::Kinematic;
            } break;
        }

        auto& box = result.Get();
        BodyCreationSettings body_settings(
          box,
          jolt::to_vec3(p_transform->position),
          jolt::to_quat(p_transform->quaternion),
          motion_type,
          p_body->body_layer_type);

        // Assigning the entity ID as the user data
        // This can be used when the contact listener is given two entities at
        // collision-levels
        body_settings.mUserData = static_cast<uint64_t>(p_entity_id);
        body_settings.mFriction = p_body->friction;
        body_settings.mRestitution = p_body->restitution;
        body_settings.mLinearVelocity = jolt::to_vec3(p_body->linear_velocity);
        body_settings.mAngularVelocity =
          jolt::to_vec3(p_body->angular_velocity);
        Body* body = body_interface.CreateBody(body_settings);

        // console_log_warn("linear_vel = ({}, {}, {})",
        // p_body->linear_velocity.x, p_body->linear_velocity.y,
        // p_body->linear_velocity.z); console_log_warn("angular_vel = ({}, {},
        // {})", p_body->angular_velocity.x, p_body->angular_velocity.y,
        // p_body->angular_velocity.z);

        // body_interface.AddForce(body->GetID(),
        // jolt::to_vec3(p_body->cumulative_force));
        // TODO: Fix this. Resitution increases when making collision contacts
        // here As this is broken (for now, it works, but this does need a
        // change) For now commenting this out because the issue is: When it
        // bounces, it increases. Which is NOT how that is supposed to work
        // body_interface.SetRestitution(body->GetID(), p_body->restitution);
        m_cached_body_ids.emplace(p_entity_id, body->GetID());
    }

    void jolt_context::emplace_capsule_collider(
      uint32_t p_entity_id,
      const transform* p_transform,
      const physics_body* p_body,
      const capsule_collider* p_collider) {
        using namespace JPH;
        auto& body_interface = m_physics_system->GetBodyInterface();
        // ensure that the half_extent of the box shape always matches the
        // object and reference that information through the transform
        CapsuleShapeSettings shape_settings(p_collider->half_height,
                                            p_collider->radius);
        auto result = shape_settings.Create();

        if (result.HasError()) {
            console_log_error("Box shape creation error: {}",
                              result.GetError());
            return;
        }
        EMotionType motion_type = EMotionType::Static;
        switch (p_body->body_movement_type) {
            case body_type::fixed: {
                motion_type = EMotionType::Static;
            } break;
            case body_type::dynamic: {
                motion_type = EMotionType::Dynamic;
            } break;
            case body_type::kinematic: {
                motion_type = EMotionType::Kinematic;
            } break;
        }

        auto& box = result.Get();
        BodyCreationSettings body_settings(
          box,
          jolt::to_vec3(p_transform->position),
          jolt::to_quat(p_transform->quaternion),
          motion_type,
          p_body->body_layer_type);
        body_settings.mFriction = p_body->friction;
        body_settings.mRestitution = p_body->restitution;
        body_settings.mLinearVelocity = jolt::to_vec3(p_body->linear_velocity);
        body_settings.mAngularVelocity =
          jolt::to_vec3(p_body->angular_velocity);
        body_settings.mUserData = static_cast<uint64_t>(p_entity_id);

        Body* body = body_interface.CreateBody(body_settings);
        m_cached_body_ids.emplace(p_entity_id, body->GetID());
    }

    transform jolt_context::context_read_transform(uint32_t p_id) {
        using namespace JPH;
        transform new_transform{};
        auto& body_interface = m_physics_system->GetBodyInterface();

        BodyID body_id = m_cached_body_ids[p_id];
        JPH::Vec3 pos = body_interface.GetPosition(body_id);
        JPH::Quat rot = body_interface.GetRotation(body_id);
        JPH::Vec3 rot_euler = rot.GetEulerAngles();

        new_transform.position = to_vec3(pos);
        new_transform.quaternion = to_vec4(rot);
        new_transform.rotation = to_vec3(rot_euler);

        return new_transform;
    }

    physics_body jolt_context::context_read_physics_body(uint32_t p_id) {
        using namespace JPH;
        auto& body_interface = m_physics_system->GetBodyInterface();
        // const JPH::BodyID id = JPH::BodyID(body.body_id);
        if (!m_cached_body_ids.contains(p_id)) {
            console_log_warn("Entity ID = {} was not found!!!", p_id);
            return {};
        }
        auto body_id = m_cached_body_ids.at(p_id);

        physics_body body = {
            .linear_velocity =
              to_vec3(body_interface.GetLinearVelocity(body_id)),
            .angular_velocity =
              to_vec3(body_interface.GetAngularVelocity(body_id)),
            .center_mass_position =
              to_vec3(body_interface.GetCenterOfMassPosition(body_id)),
            .gravity_factor = body_interface.GetGravityFactor(body_id),
            .friction = body_interface.GetFriction(body_id),
            .restitution = body_interface.GetRestitution(body_id),
        };

        return body;
    }

    void jolt_context::destroy_bodies() {
        auto& body_interface = m_physics_system->GetBodyInterface();

        // Retrieve all body ID's to ensure that we do proper deactivation and
        // post cleanup for the physics simulation
        JPH::BodyIDVector all_body_ids;
        m_physics_system->GetBodies(all_body_ids);

        if (!all_body_ids.empty()) {

            body_interface.DeactivateBodies(
              all_body_ids.data(), static_cast<int>(all_body_ids.size()));

            body_interface.RemoveBodies(all_body_ids.data(),
                                        static_cast<int>(all_body_ids.size()));

            body_interface.DestroyBodies(all_body_ids.data(),
                                         static_cast<int>(all_body_ids.size()));

            m_cached_body_ids.clear();
        }

        console_log_info("Removed All shapes and bodies...\n");
    }

    void jolt_context::linear_velocity(uint64_t p_id,
                                       const glm::vec3& p_linear_velocity) {
        using namespace JPH;
        auto& body_interface = m_physics_system->GetBodyInterface();

        body_interface.SetLinearVelocity(m_cached_body_ids.at(p_id),
                                         jolt::to_vec3(p_linear_velocity));
    }

    void jolt_context::angular_velocity(uint64_t p_id,
                                        const glm::vec3& p_angular_velocity) {
        using namespace JPH;
        auto& body_interface = m_physics_system->GetBodyInterface();
        BodyID body_id(p_id);

        body_interface.SetAngularVelocity(m_cached_body_ids.at(p_id),
                                          jolt::to_vec3(p_angular_velocity));
    }

    void jolt_context::prepare_and_finalize() {
        using namespace JPH;

        //! @brief We actually do not need to pass in the body ID's into
        //! std::vector. Though we may need ways to store JPH::BodyID for
        //! modifying specific bodies
        JPH::BodyIDVector all_body_ids;
        m_physics_system->GetBodies(all_body_ids);

        auto& body_interface = m_physics_system->GetBodyInterface();
        auto state = body_interface.AddBodiesPrepare(
          all_body_ids.data(), static_cast<int>(all_body_ids.size()));
        body_interface.AddBodiesFinalize(all_body_ids.data(),
                                         static_cast<int>(all_body_ids.size()),
                                         state,
                                         JPH::EActivation::Activate);
        console_log_info("Batch inserted {} bodies successfully.",
                         all_body_ids.size());
    }

    void jolt_context::update_simulation(float p_delta_time) {
        float fixed_time_step = 1.0f / 60.0f;
        int time_step = 1 + (int)(60 * fixed_time_step);
        m_physics_system->Update(p_delta_time,
                                 time_step,
                                 m_temp_allocator.get(),
                                 m_thread_system.get());
    }
}
