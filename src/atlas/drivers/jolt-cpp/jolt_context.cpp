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

    jolt_context::jolt_context(const jolt_settings& p_settings) {
        JPH::RegisterDefaultAllocator();

        JPH::Trace = trace_impl;
        JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = assert_failed_impl;)

        JPH::Factory::sInstance = new JPH::Factory();
        JPH::RegisterTypes();

        m_temp_allocator =
          create_ref<JPH::TempAllocatorImpl>(p_settings.allocation_amount);

        // Sets up basic settings for physics engine
        m_physics_system = create_ref<JPH::PhysicsSystem>();
        m_broad_phase_layer_interface =
          create_ref<broad_phase_layer_interface>();
        m_object_vs_broadphase_filter =
          create_ref<object_vs_broadphase_layer>();
        m_object_layer_pair_filter = create_ref<object_layer_pair_filter>();

        if (p_settings.thread_type == thread_type::default_system){

            m_thread_system = create_scope<JPH::JobSystemThreadPool>(
              // Max jobs must be a power of 2, otherwise jph crashes.
              // Bianary tree must be fully balanced
              std::pow(2, p_settings.max_jobs_power),
              p_settings.max_barriers,
              p_settings.physics_threads);
        }
        else {
            console_log_error("Unsupported thread system: {}",
                              (int)p_settings.thread_type);
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


    void jolt_context::emplace_box_collider(uint32_t p_entity_id, const transform* p_transform, const physics_body* p_body, const box_collider* p_collider) {
        using namespace JPH;
        auto& body_interface = m_physics_system->GetBodyInterface();
        // ensure that the half_extent of the box shape always matches the object and reference that information through the transform
        BoxShapeSettings shape_settings(to_jph(p_collider->half_extent));
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
            }
            break;
        case body_type::dynamic: {
            motion_type = EMotionType::Dynamic;
            console_log_info("EMotionType::Dynamic!");
            }
            break;
        case body_type::kinematic: {
            motion_type = EMotionType::Kinematic;
            console_log_info("EMotionType::Kinematic!");
            }
            break;
        }

        auto& box = result.Get();
        BodyCreationSettings body_settings(box, to_jph(p_transform->position), to_jph(p_transform->quaternion), motion_type, p_body->body_layer_type);
        
        // NOTE TO  SELF ------ This is setting some pointer to the entity ID
        // WE CAN USE THIS TO TELL THE EVENT SYSTEM WHICH FLECS ENTITY COLLIDED WITH EACH OTHER!!!!!!!!
        // Because each contact listener allows you to take a pointer from the physics bodies that are just blocks of data!!!
        body_settings.mUserData = static_cast<uint64_t>(p_entity_id);
        console_log_info("Entity ID = {}", p_entity_id);

        Body* body = body_interface.CreateBody(body_settings);
        m_cached_body_ids.emplace(p_entity_id, body->GetID());
    }


    void jolt_context::emplace_sphere_collider(uint32_t p_entity_id, const transform* p_transform, const physics_body* p_body, const sphere_collider* p_collider) {
        using namespace JPH;
        auto& body_interface = m_physics_system->GetBodyInterface();
        // ensure that the half_extent of the box shape always matches the object and reference that information through the transform
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
            console_log_info("EMotionType::Static!");
            }
            break;
        case body_type::dynamic: {
            motion_type = EMotionType::Dynamic;
            console_log_info("EMotionType::Dynamic!");
            }
            break;
        case body_type::kinematic: {
            motion_type = EMotionType::Kinematic;
            console_log_info("EMotionType::Kinematic!");
            }
            break;
        }

        auto& box = result.Get();
        BodyCreationSettings body_settings(box, to_jph(p_transform->position), to_jph(p_transform->quaternion), motion_type, p_body->body_layer_type);
        body_settings.mUserData = static_cast<uint64_t>(p_entity_id);

        Body* body = body_interface.CreateBody(body_settings);
        m_cached_body_ids.emplace(p_entity_id, body->GetID());

        // TODO: Fix this. Resitution increases when making collision contacts here
        // As this is broken (for now, it works, but this does need a change)
        // For now commenting this out because the issue is: When it bounces, it increases. Which is NOT how that is supposed to work
        // body_interface.SetRestitution(body->GetID(), p_body->restitution);
    }

    void jolt_context::emplace_capsule_collider(uint32_t p_entity_id, const transform* p_transform, const physics_body* p_body, const capsule_collider* p_collider) {
        using namespace JPH;
        auto& body_interface = m_physics_system->GetBodyInterface();
        // ensure that the half_extent of the box shape always matches the object and reference that information through the transform
        CapsuleShapeSettings shape_settings(p_collider->half_height, p_collider->radius);
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
            }
            break;
        case body_type::dynamic: {
            motion_type = EMotionType::Dynamic;
            console_log_info("EMotionType::Dynamic!");
            }
            break;
        case body_type::kinematic: {
            motion_type = EMotionType::Kinematic;
            console_log_info("EMotionType::Kinematic!");
            }
            break;
        }

        auto& box = result.Get();
        BodyCreationSettings body_settings(box, to_jph(p_transform->position), to_jph(p_transform->quaternion), motion_type, p_body->body_layer_type);
        body_settings.mUserData = static_cast<uint64_t>(p_entity_id);

        Body* body = body_interface.CreateBody(body_settings);
        m_cached_body_ids.emplace(p_entity_id, body->GetID());

        // TODO: Fix this. Resitution increases when making collision contacts here
        // As this is broken (for now, it works, but this does need a change)
        // For now commenting this out because the issue is: When it bounces, it increases. Which is NOT how that is supposed to work
        // body_interface.SetRestitution(body->GetID(), p_body->restitution);
    }

    transform jolt_context::context_read_transform(uint32_t p_id) {
        using namespace JPH;
        transform new_transform{};
        auto& body_interface = m_physics_system->GetBodyInterface();
        // if(!m_cached_body_ids.contains(p_entity.id())) {
        //     // console_log_warn("Entity ID Not Found in Cache!!!");
        //     return {};
        // }
        // BodyID body_id = m_cached_body_ids.at(p_entity.id());
        BodyID body_id = m_cached_body_ids[p_id];
        JPH::Vec3 pos = body_interface.GetPosition(body_id);
        JPH::Quat rot = body_interface.GetRotation(body_id);
        JPH::Vec3 rot_euler = rot.GetEulerAngles();

        new_transform.position = glm::vec3(pos.GetX(), pos.GetY(), pos.GetZ());
        new_transform.quaternion = glm::vec4(rot.GetX(), rot.GetY(), rot.GetZ(), rot.GetW());
        new_transform.rotation = glm::vec3(rot_euler.GetX(), rot_euler.GetY(), rot_euler.GetZ());

        return new_transform;
    }

    physics_body jolt_context::context_read_physics_body(uint32_t p_id) {
        using namespace JPH;
        auto& body_interface = m_physics_system->GetBodyInterface();
        // const JPH::BodyID id = JPH::BodyID(body.body_id);
        if(!m_cached_body_ids.contains(p_id)) {
            console_log_warn("Entity ID = {} was not found!!!", p_id);
            return {};
        }
        auto body_id = m_cached_body_ids.at(p_id);

        physics_body body{};

        JPH::Vec3 linear_velocity = JPH::Vec3::sZero();
        JPH::Vec3 angular_velocity = JPH::Vec3::sZero();

        body_interface.GetLinearAndAngularVelocity(
            body_id, linear_velocity, angular_velocity);

        if (linear_velocity.IsClose({ 0, 0, 0 }, 0.0001f)) {
            linear_velocity = { 0, 0, 0 };
        }

        if (angular_velocity.IsClose({ 0, 0, 0 }, 0.0001f)) {
            angular_velocity = { 0, 0, 0 };
        }
        
        // TODO: Provide read_linear_velocity and read_angular_velocity(uint32_t p_id);
        body.linear_velocity = glm::vec3(linear_velocity.GetX(),
                                            linear_velocity.GetY(),
                                            linear_velocity.GetZ());
        body.angular_velocity = glm::vec3(angular_velocity.GetX(),
                                            angular_velocity.GetY(),
                                            angular_velocity.GetZ());

        JPH::Vec3 center_mass = body_interface.GetCenterOfMassPosition(body_id);
        body.center_mass_position = glm::vec3(
            center_mass.GetX(), center_mass.GetY(), center_mass.GetZ());

        body.gravity_factor = body_interface.GetGravityFactor(body_id);
        body.friction = body_interface.GetFriction(body_id);
        body.restitution = body_interface.GetRestitution(body_id);
        body.body_type = static_cast<uint8_t>(body_interface.GetMotionType(body_id));

        return body;
    }

    void jolt_context::destroy_bodies() {
        auto& body_interface = m_physics_system->GetBodyInterface();

        JPH::BodyIDVector all_body_ids;
        m_physics_system->GetBodies(all_body_ids);

        if (!all_body_ids.empty()) {

            body_interface.DeactivateBodies(all_body_ids.data(),
                                            static_cast<int>(all_body_ids.size()));

            body_interface.RemoveBodies(all_body_ids.data(),
                                        static_cast<int>(all_body_ids.size()));

            body_interface.DestroyBodies(all_body_ids.data(),
                                         static_cast<int>(all_body_ids.size()));

            // std::unordered_map<uint64_t, JPH::RefConst<JPH::Shape>> empty;

            // m_shape_registry.swap(empty);
            // m_shape_registry.clear();
            m_cached_body_ids.clear();
        }

        // if(!m_cached_body_ids.empty()) {
        // m_cached_body_ids.clear();
        // }

        console_log_info("Removed All shapes and bodies...\n");
    }

    void jolt_context::prepare_and_finalize() {
        using namespace JPH;

        //! @brief We actually do not need to pass in the body ID's into std::vector. Though we may need ways to store JPH::BodyID for modifying specific bodies
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

    // void jolt_context::execute_collisions() {
    //     // m_contact_listener->run_events_added();
    // }
}
