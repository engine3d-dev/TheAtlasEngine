module;

#include <Jolt/Jolt.h>
#include <Jolt/Core/Core.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Core/IssueReporting.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/Memory.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Core/IssueReporting.h>

#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <Jolt/Physics/Collision/Shape/Shape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

#include <Jolt/Physics/EActivation.h>

// jolt's math includes
#include <Jolt/Math/MathTypes.h>
#include <Jolt/Math/Real.h>
#include <Jolt/Math/Quat.h>
#include <cstdarg>
#include <glm/glm.hpp>
#include <map>

export module atlas.drivers.jolt_cpp.context;

// import atlas.logger;
// import atlas.common;
import atlas.core.utilities;
import atlas.core.event;
import atlas.core.scene.components;

import atlas.drivers.jolt_cpp.broad_phase;
import atlas.drivers.jolt_cpp.contact_listener;

import atlas.drivers.jolt_cpp.types;
import atlas.drivers.physics_context;
import atlas.drivers.jolt_cpp.types;

namespace atlas::physics {

    enum thread_type : uint8_t {
        default_system = 0,
        job_system = 1,
    };

    // This might be able to be generalized eventually but we will have to
    // create our own manifold before that happens.
    struct contact_event {
        uint64_t entity_a = 0;
        uint64_t entity_b = 0;
        JPH::ContactManifold manifold;
        JPH::ContactSettings settings;
    };

    /**
     * @brief Jolt-specific context configurations
     * These are going to be internally integrated to jolt_context
     *
     * As these parameters currently are going to be specific to Jolt.
     *
     * These parameters are also only specific to the construction-level, not
     * initiation level of the API's
     *
     * @remark Min and max world bounds are values that are artbitrary (in other
     * words limit the simulation space) as JoltPhysics has a limit on distance
     * for its limitation in simulation space
     */
    struct jolt_settings {

        uint32_t allocation_amount = 10 * 1024 * 1024;

        //! @brief Specifying which threading system to use for Jolt.
        thread_type thread_type = thread_type::default_system;

        uint32_t physics_threads =
          std::max(1u, std::thread::hardware_concurrency() - 2);

        uint32_t max_jobs_power = 10;
        uint32_t max_barriers = physics_threads * 16;
        bool enable_multithread = true;

        // Max memory size per scene
        uint32_t max_bodies = 16384;
        uint32_t max_body_pairs = 32768;
        uint32_t max_contact_constraints = 8192;
    };

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

    /**
     * @brief jolt_context is the backend implementation of physics context
     *
     * This is the jolt specific implementation of the physics context
     *
     * Represents the external abstraction that is defined through the the
     * engine-specific parameters.
     */
    export class jolt_context : public physics_context {
    public:
        /**
         * @brief construct a new physics context with jolt implemented as its
         * backend
         *
         * @param p_settings are the configuration parameters for initiating
         * JoltPhysics
         * @param p_bus is the event::bus that allows for publishing physics
         * events to the subscribers of those said events
         */
        jolt_context(event::bus& p_bus)
          : m_contact_listener(p_bus) {
            jolt_settings settings = {};
            JPH::RegisterDefaultAllocator();

            JPH::Trace = trace_impl;
            JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = assert_failed_impl;)

            JPH::Factory::sInstance = new JPH::Factory();
            JPH::RegisterTypes();

            m_temp_allocator =
              create_ref<JPH::TempAllocatorImpl>(settings.allocation_amount);

            // This just sets up the JoltPhysics system and any listeners
            m_physics_system = create_ref<JPH::PhysicsSystem>();
            m_broad_phase_layer_interface =
              create_ref<broad_phase_layer_interface>();
            m_object_vs_broadphase_filter =
              create_ref<object_vs_broadphase_layer>();
            m_object_layer_pair_filter = create_ref<object_layer_pair_filter>();

            if (settings.thread_type == thread_type::default_system) {

                m_thread_system = create_scope<JPH::JobSystemThreadPool>(
                  // Max jobs must be a power of 2, otherwise jph crashes.
                  // Bianary tree must be fully balanced
                  std::pow(2, settings.max_jobs_power),
                  settings.max_barriers,
                  settings.physics_threads);
            }
            else {
                console_log_error("Unsupported custom job system");
                assert(false);
            }

            m_physics_system->Init(settings.max_bodies,
                                   0,
                                   settings.max_body_pairs,
                                   settings.max_contact_constraints,
                                   *m_broad_phase_layer_interface,
                                   *m_object_vs_broadphase_filter,
                                   *m_object_layer_pair_filter);

            // Default contact listener impl and can change during runtime
            m_physics_system->SetContactListener(&m_contact_listener);
        }

        ~jolt_context() override = default;

    private:
        void prepare_and_finalize() override {
            using namespace JPH;

            //! @brief We actually do not need to pass in the body ID's into
            //! std::vector. Though we may need ways to store JPH::BodyID for
            //! modifying specific bodies
            JPH::BodyIDVector all_body_ids;
            m_physics_system->GetBodies(all_body_ids);

            auto& body_interface = m_physics_system->GetBodyInterface();
            auto state = body_interface.AddBodiesPrepare(
              all_body_ids.data(), static_cast<int>(all_body_ids.size()));
            body_interface.AddBodiesFinalize(
              all_body_ids.data(),
              static_cast<int>(all_body_ids.size()),
              state,
              JPH::EActivation::Activate);
        }

        void update_simulation(float p_delta_time) override {
            float fixed_time_step = 1.0f / 60.0f;
            int time_step = 1 + (int)(60 * fixed_time_step);
            m_physics_system->Update(p_delta_time,
                                     time_step,
                                     m_temp_allocator.get(),
                                     m_thread_system.get());
        }

        /**
         * @brief Performs any specific cleanup needed by Jolt
         */
        void destroy_bodies() override {
            auto& body_interface = m_physics_system->GetBodyInterface();

            // Retrieve all body ID's to ensure that we do proper deactivation
            // and post cleanup for the physics simulation
            JPH::BodyIDVector all_body_ids;
            m_physics_system->GetBodies(all_body_ids);

            if (!all_body_ids.empty()) {

                body_interface.DeactivateBodies(
                  all_body_ids.data(), static_cast<int>(all_body_ids.size()));

                body_interface.RemoveBodies(
                  all_body_ids.data(), static_cast<int>(all_body_ids.size()));

                body_interface.DestroyBodies(
                  all_body_ids.data(), static_cast<int>(all_body_ids.size()));

                m_cached_body_ids.clear();
            }
        }

    protected:
        void emplace_box_collider(uint32_t p_entity_id,
                                  const transform* p_transform,
                                  const physics_body* p_body,
                                  const box_collider* p_collider) override {
            using namespace JPH;
            auto& body_interface = m_physics_system->GetBodyInterface();

            // Creating our box shape and specifying half_extent that is a
            // glm::vec3 conversion to JPH::Vec3 half_extents must be 0.5f or
            // else it can get an invalid convex radius
            BoxShapeSettings shape_settings(
              jolt::to_vec3(p_collider->half_extent));
            auto result = shape_settings.Create();

            if (result.HasError()) {
                console_log_error("Box shape creation error: {}",
                                  result.GetError());
                return;
            }
            EMotionType motion_type = EMotionType::Static;
            switch (p_body->body_movement_type) {
                case body_type::fixed:
                    motion_type = EMotionType::Static;
                    break;
                case body_type::dynamic:
                    motion_type = EMotionType::Dynamic;
                    break;
                case body_type::kinematic:
                    motion_type = EMotionType::Kinematic;
                    break;
            }

            auto& box = result.Get();
            BodyCreationSettings body_settings(
              box,
              jolt::to_vec3(p_transform->position),
              jolt::to_quat(p_transform->quaternion),
              motion_type,
              p_body->body_layer_type);

            // NOTE TO  SELF ------ This is setting some pointer to the entity
            // ID WE CAN USE THIS TO TELL THE EVENT SYSTEM WHICH FLECS ENTITY
            // COLLIDED WITH EACH OTHER!!!!!!!! Because each contact listener
            // allows you to take a pointer from the physics bodies that are
            // just blocks of data!!!
            body_settings.mUserData = static_cast<uint64_t>(p_entity_id);
            body_settings.mFriction = p_body->friction;
            body_settings.mRestitution = p_body->restitution;
            body_settings.mLinearVelocity =
              jolt::to_vec3(p_body->linear_velocity);
            body_settings.mAngularVelocity =
              jolt::to_vec3(p_body->angular_velocity);

            Body* body = body_interface.CreateBody(body_settings);
            m_cached_body_ids.emplace(p_entity_id, body->GetID());
        }

        void emplace_sphere_collider(
          uint32_t p_entity_id,
          const transform* p_transform,
          const physics_body* p_body,
          const sphere_collider* p_collider) override {
            using namespace JPH;
            auto& body_interface = m_physics_system->GetBodyInterface();
            // ensure that the half_extent of the box shape always matches the
            // object and reference that information through the transform
            SphereShapeSettings shape_settings(p_collider->radius);
            auto result = shape_settings.Create();

            if (result.HasError()) {
                console_log_error("Sphere shape creation error: {}",
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
            // Fetched when collision happens
            body_settings.mUserData = static_cast<uint64_t>(p_entity_id);
            body_settings.mFriction = p_body->friction;
            body_settings.mRestitution = p_body->restitution;
            body_settings.mLinearVelocity =
              jolt::to_vec3(p_body->linear_velocity);
            body_settings.mAngularVelocity =
              jolt::to_vec3(p_body->angular_velocity);
            Body* body = body_interface.CreateBody(body_settings);

            // body_interface.AddForce(body->GetID(),
            // jolt::to_vec3(p_body->cumulative_force));
            m_cached_body_ids.emplace(p_entity_id, body->GetID());
        }

        void emplace_capsule_collider(
          uint32_t p_entity_id,
          const transform* p_transform,
          const physics_body* p_body,
          const capsule_collider* p_collider) override {
            using namespace JPH;
            auto& body_interface = m_physics_system->GetBodyInterface();
            // ensure that the half_extent of the box shape always matches the
            // object and reference that information through the transform
            CapsuleShapeSettings shape_settings(p_collider->half_height,
                                                p_collider->radius);
            auto result = shape_settings.Create();

            if (result.HasError()) {
                console_log_error("Capsule shape creation error: {}",
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
            body_settings.mLinearVelocity =
              jolt::to_vec3(p_body->linear_velocity);
            body_settings.mAngularVelocity =
              jolt::to_vec3(p_body->angular_velocity);
            body_settings.mUserData = static_cast<uint64_t>(p_entity_id);

            Body* body = body_interface.CreateBody(body_settings);
            m_cached_body_ids.emplace(p_entity_id, body->GetID());
        }

        // void set_position_rotation(flecs::entity p_entity, const
        // physics_body* p_body, const box_collider* p_collider, const
        // transform* p_transform) override;
        transform context_read_transform(uint32_t p_id) override {
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

        physics_body context_read_physics_body(uint32_t p_id) override {
            using namespace JPH;
            auto& body_interface = m_physics_system->GetBodyInterface();

            // TODO: Will need to change this because if this entity doesn't
            // exist then it will be set to zeroes, gotta be careful about this
            if (!m_cached_body_ids.contains(p_id)) {
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

        void linear_velocity(uint64_t p_id,
                             const glm::vec3& p_linear_velocity) override {
            using namespace JPH;
            auto& body_interface = m_physics_system->GetBodyInterface();

            body_interface.SetLinearVelocity(m_cached_body_ids.at(p_id),
                                             jolt::to_vec3(p_linear_velocity));
        }

        void angular_velocity(uint64_t p_id,
                              const glm::vec3& p_angular_velocity) override {
            using namespace JPH;
            auto& body_interface = m_physics_system->GetBodyInterface();
            BodyID body_id(p_id);

            body_interface.SetAngularVelocity(
              m_cached_body_ids.at(p_id), jolt::to_vec3(p_angular_velocity));
        }

        void force(uint64_t p_id, const glm::vec3& p_force) override {
            using namespace JPH;
            auto& body_interface = m_physics_system->GetBodyInterface();
            BodyID body_id(p_id);

            body_interface.AddForce(m_cached_body_ids.at(p_id),
                                    jolt::to_vec3(p_force));
        }

        void add_force_and_torque(uint64_t p_id,
                                  const glm::vec3& p_force,
                                  const glm::vec3& p_torque) override {
            using namespace JPH;
            auto& body_interface = m_physics_system->GetBodyInterface();

            body_interface.AddForceAndTorque(m_cached_body_ids.at(p_id),
                                             jolt::to_vec3(p_force),
                                             jolt::to_vec3(p_torque));
        }

        void add_impulse(uint64_t p_id, const glm::vec3& p_impulse) override {
            using namespace JPH;
            auto& body_interface = m_physics_system->GetBodyInterface();

            body_interface.AddImpulse(m_cached_body_ids.at(p_id),
                                      jolt::to_vec3(p_impulse));
        }

    private:
        //! @note Must be defined before physics can be initialized otherwise
        //! jolt cannot be created properly.
        jolt_settings m_settings;

        /**
         * @brief Creates a static allocation of all data
         *
         */
        ref<JPH::TempAllocatorImpl> m_temp_allocator;

        /**
         * @brief Sets up a thread system, either jolts or custom based on
         * thread settings in m_settings
         *
         */
        scope<JPH::JobSystemThreadPool> m_thread_system;

        /**
         * @brief Creates filtering for the quad tree in terms of movement
         *
         */
        ref<broad_phase_layer_interface> m_broad_phase_layer_interface;

        /**
         * @brief Creates a filter for the quad tree in terms of objects types
         *
         */
        ref<object_vs_broadphase_layer> m_object_vs_broadphase_filter;

        /**
         * @brief Creates a filter for pairs of collisions
         *
         */
        ref<object_layer_pair_filter> m_object_layer_pair_filter;

        /**
         * @brief Creates a way to recognize collisions
         *
         */
        // ref<contact_listener> m_contact_listener;
        contact_listener m_contact_listener;

        /**
         * @brief Gives access to the physics system. Is given to jolt_api as
         * well. This is to allow us to give seperation of concerns. However,
         * may change now do to Jolt conflict and unhandled dangling pointers
         * within jolt_api.
         *
         */
        ref<JPH::PhysicsSystem> m_physics_system;

        std::map<uint32_t, JPH::BodyID> m_cached_body_ids;
    };
};