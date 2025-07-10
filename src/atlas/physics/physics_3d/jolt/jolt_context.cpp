#include <physics/jolt-cpp/jolt_error_handler.hpp>
#include <physics/physics_3d/jolt/jolt_context.hpp>
#include <engine_logger.hpp>

#include <physics/types.hpp>
#include <scene/scene.hpp>
#include <physics/physics_3d/jolt/jolt_helper.hpp>
#include <core/scene/world.hpp>
#include <core/system_framework/system_registry.hpp>

namespace atlas::physics {

    // There should only every be one factoy. Even if there are multiple engine
    // running at the same time.
    static bool factory_initialized = false;
    jolt_context::jolt_context(const jolt_settings& p_settings) {
        // Sets up static arena
        JPH::RegisterDefaultAllocator();

        // Custom error handling
        JPH::Trace = trace_impl;
        JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = assert_failed_impl;)

        if (!factory_initialized) {
            JPH::Factory::sInstance = new JPH::Factory();
            JPH::RegisterTypes();
            factory_initialized = true;
        }

        m_settings = p_settings;

        // Creates static arena
        m_temp_allocator =
          create_ref<JPH::TempAllocatorImpl>(m_settings.allocation_amount);

        // Sets up basic settings for physics engine
        m_physics_system = create_ref<JPH::PhysicsSystem>();
        m_broad_phase_layer_interface =
          create_ref<broad_phase_layer_interface>();
        m_object_vs_broadphase_filter =
          create_ref<object_vs_broadphase_layer>();
        m_object_layer_pair_filter = create_ref<object_layer_pair_filter>();

        if (m_settings.thread_type == thread_system::Default) {

            m_thread_system = create_scope<JPH::JobSystemThreadPool>(
              // Max jobs must be a power of 2, otherwise jph crashes.
              // Bianary tree must be fully balanced
              std::pow(2, m_settings.max_jobs_power),
              m_settings.max_barriers,
              m_settings.physics_threads);
        }
        else {
            console_log_error("Unsupported thread system: {}",
                              (int)m_settings.thread_type);
        }

        m_physics_system->Init(m_settings.max_bodies,
                               0,
                               m_settings.max_body_pairs,
                               m_settings.max_contact_constraints,
                               *m_broad_phase_layer_interface,
                               *m_object_vs_broadphase_filter,
                               *m_object_layer_pair_filter);

        m_contact_listener = create_ref<contact_listener>();
        // Default contact listener impl and can change during runtime
        m_physics_system->SetContactListener(m_contact_listener.get());
    }

    JPH::RefConst<JPH::Shape> jolt_context::create_shape_from_collider(
      flecs::entity e,
      const collider_body& collider) {
        using namespace JPH;

        RefConst<Shape> created_shape;

        switch (collider.shape_type) {
            case collider_shape::Box: {
                BoxShapeSettings shape_settings(to_jph(collider.half_extents));
                auto result = shape_settings.Create();
                if (result.HasError()) {
                    console_log_fatal("Box shape creation error: {}",
                                      result.GetError());
                    return nullptr;
                }
                created_shape = result.Get();
                break;
            }
            case collider_shape::Sphere: {
                SphereShapeSettings shape_settings(collider.radius);
                auto result = shape_settings.Create();
                if (result.HasError()) {
                    console_log_fatal("Sphere shape creation error: {}",
                                      result.GetError());
                    return nullptr;
                }
                created_shape = result.Get();
                break;
            }
            case collider_shape::Capsule: {
                CapsuleShapeSettings shape_settings(
                  collider.capsule_half_height, collider.radius);
                auto result = shape_settings.Create();
                if (result.HasError()) {
                    console_log_fatal("Capsule shape creation error: {}",
                                      result.GetError());
                    return nullptr;
                }
                created_shape = result.Get();
                break;
            }
            // Missing Mesh, Terrain, and Traingle types
            // Mesh shapes are still new and will cause a crash if two collide.
            default:
                console_log_fatal("Unknown shape type.");
                return nullptr;
        }

        // Sanity check
        JPH_ASSERT(created_shape != nullptr);
        m_shape_registry[e.id()] = created_shape;
        return created_shape;
    }

    void jolt_context::add_body(
      flecs::entity e,
      const physics_body* body_opt,
      const collider_body& collider,
      const transform_physics& location,
      std::vector<JPH::BodyCreationSettings>& out_settings_list,
      std::vector<flecs::entity>& out_entity_list) {
        auto shape = create_shape_from_collider(e, collider);
        if (!shape)
            return;

        JPH::EMotionType motion_type = JPH::EMotionType::Static;
        uint8_t layer_type = body_layer::NonMoving;

        if (body_opt) {
            // Needs to change to a better body type system
            motion_type = static_cast<JPH::EMotionType>(body_opt->body_type);
            layer_type = body_opt->body_layer_type;
        }

        JPH::BodyCreationSettings body_settings(
          shape,
          to_jph(location.position),
          to_jph(location.quaterion_rotation),
          motion_type,
          layer_type);

        // Gives bodies thier assocaited flecs entity
        body_settings.mUserData = static_cast<uint64_t>(e.id());

        out_settings_list.push_back(std::move(body_settings));
        out_entity_list.push_back(e);
    }

    void jolt_context::engine_create_physics_bodies() {
        std::vector<JPH::BodyCreationSettings> settings_list;
        std::vector<flecs::entity> entity_list;

        // This needs to change to get all active scenes that exsist.
        // Need a get_active_scenes() function that returns a list of worlds
        ref<world_scope> world_object =
          system_registry::get_world("Editor World");
        if (!world_object) {
            console_log_error("World not found.");
            return;
        }

        ref<scene_scope> scene = world_object->get_scene("LevelScene");
        if (!scene) {
            console_log_error("Scene not found.");
            return;
        }

        scene->query_builder<physics_body, collider_body, transform_physics>()
          .each([&](flecs::entity e,
                    const physics_body& phys,
                    const collider_body& col,
                    const transform_physics& loc) {
              add_body(e, &phys, col, loc, settings_list, entity_list);
          });

        scene->query_builder<collider_body, transform_physics>()
          .without<physics_body>()
          .each([&](flecs::entity e,
                    const collider_body& col,
                    const transform_physics& loc) {
              add_body(e, nullptr, col, loc, settings_list, entity_list);
          });

        if (settings_list.empty()) {
            console_log_error("No physics bodies to create.");
            return;
        }

        auto& body_interface = m_physics_system->GetBodyInterface();

        std::vector<JPH::BodyID> body_ids;
        body_ids.reserve(settings_list.size());

        for (size_t i = 0; i < settings_list.size(); ++i) {
            JPH::BodyCreationSettings& settings = settings_list[i];

            if (!settings.GetShape()) {
                console_log_fatal("Shape is null at index {}", i);
                continue;
            }

            JPH::Body* body = body_interface.CreateBody(settings);
            if (!body) {
                console_log_fatal("CreateBody returned nullptr at index {}", i);
                continue;
            }

            JPH::BodyID body_id = body->GetID();
            body_ids.push_back(body_id);

            if (entity_list[i].has<physics_body>())
                entity_list[i].get_mut<physics_body>()->body_id =
                  body_id.GetIndexAndSequenceNumber();

            if (entity_list[i].has<collider_body>())
                entity_list[i].get_mut<collider_body>()->body_id =
                  body_id.GetIndexAndSequenceNumber();
        }

        auto state = body_interface.AddBodiesPrepare(
          body_ids.data(), static_cast<int>(body_ids.size()));
        body_interface.AddBodiesFinalize(body_ids.data(),
                                         static_cast<int>(body_ids.size()),
                                         state,
                                         JPH::EActivation::Activate);

        console_log_info("Batch inserted {} bodies successfully.",
                         body_ids.size());
    }

    void jolt_context::engine_clean_physics_bodies() {
        auto& body_interface = m_physics_system->GetBodyInterface();

        JPH::BodyIDVector all_body_ids;
        m_physics_system->GetBodies(all_body_ids);

        if (!all_body_ids.empty()) {

            body_interface.DeactivateBodies(all_body_ids.data(),
                                            (int)(all_body_ids.size()));

            body_interface.RemoveBodies(all_body_ids.data(),
                                        (int)(all_body_ids.size()));

            body_interface.DestroyBodies(all_body_ids.data(),
                                         (int)(all_body_ids.size()));

            std::unordered_map<uint64_t, JPH::RefConst<JPH::Shape>> empty;

            m_shape_registry.swap(empty);
        }

        console_log_info("Removed All shapes and bodies...\n");

        // This is where you would deserialize
    }

    void jolt_context::engine_run_physics_step() {

        // Step the simulation

        console_log_info("Application {}", application::delta_time());
        int physics_step = 1 + (int)(60 * application::delta_time());
        if (!m_settings.use_fixed_timestep) {
            m_physics_system->Update(application::delta_time(),
                                     physics_step,
                                     m_temp_allocator.get(),
                                     m_thread_system.get());
        }
        else {
            console_log_info("Application {}", application::delta_time());
            int time_step = 1 + (int)(60 * m_settings.fixed_time_step);
            m_physics_system->Update(application::delta_time(),
                                     time_step,
                                     m_temp_allocator.get(),
                                     m_thread_system.get());
        }
    }

    void jolt_context::engine_run_contact_added() {
        m_contact_listener->run_events_added();
    }

    jolt_context::~jolt_context() = default;

}
