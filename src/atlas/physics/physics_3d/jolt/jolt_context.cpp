#include <physics/jolt-cpp/jolt_error_handler.hpp>
#include <physics/physics_3d/jolt/jolt_context.hpp>
#include <engine_logger.hpp>

#include <physics/types.hpp>
#include <scene/scene.hpp>
#include <physics/physics_3d/jolt/jolt_helper.hpp>

namespace atlas::physics {

    static bool factory_initialized = false;
    jolt_context::jolt_context(jolt::jolt_settings p_settings) {
        JPH::RegisterDefaultAllocator();
        JPH::Trace = trace_impl;
        JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = assert_failed_impl;)

        JPH_LOG("Trace implemented... Starting shape factory.");

        if (!factory_initialized) {
            JPH::Factory::sInstance = new JPH::Factory();
            JPH::RegisterTypes();
            factory_initialized = true;
        }

        JPH_LOG("Shape factory created... Allocating memory");

        m_settings = p_settings;
        m_temp_allocator = create_ref<JPH::TempAllocatorImpl>(m_settings.allocation_amount);
        m_physics_system = create_ref<JPH::PhysicsSystem>();
        m_broad_phase_layer_interface =
          create_ref<broad_phase_layer_interface>();
        m_object_vs_broadphase_filter =
          create_ref<object_vs_broadphase_layer>();
        m_object_layer_pair_filter = create_ref<object_layer_pair_filter>();

        JPH_LOG("Memory Allocated... Starting thread manager.");

        if (m_settings.thread_type == thread_system::Default) {

            m_thread_system = create_scope<JPH::JobSystemThreadPool>(
              std::pow(2, m_settings.max_jobs_power),
              m_settings.max_barriers,
              m_settings.physics_threads);
        }
        else {
            console_log_error("Unsupported thread system: {}",
                              (int)m_settings.thread_type);
        }

        JPH_LOG("Thread manager created... Starting initialization.");

        m_physics_system->Init(m_settings.max_bodies,
                               0,
                               m_settings.max_body_pairs,
                               m_settings.max_contact_constraints,
                               *m_broad_phase_layer_interface,
                               *m_object_vs_broadphase_filter,
                               *m_object_layer_pair_filter);

        JPH_LOG("Init completed... Starting contact_listener.");
        m_contact_listener = create_ref<contact_listener>();

        m_physics_system->SetContactListener(m_contact_listener.get());

        JPH_LOG("All initialized... engine created.");
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
            default:
                console_log_fatal("Unknown shape type.");
                return nullptr;
        }

        JPH_ASSERT(created_shape != nullptr);
        m_shape_registry[e.id()] = created_shape;
        return created_shape;
    }

    void jolt_context::add_body(
      flecs::entity e,
      const physics_body& body,
      const collider_body& collider,
      const transform_physics& location,
      std::vector<JPH::BodyCreationSettings>& settings_list,
      std::vector<flecs::entity>& entity_list) {
        auto shape = create_shape_from_collider(e, collider);
        if (!shape)
            return;

        JPH::BodyCreationSettings body_settings(
          shape,
          to_jph(location.position),
          to_jph(location.quaterion_rotation),
          static_cast<JPH::EMotionType>(body.body_type),
          body.body_layer_type);

        body_settings.mUserData = (uint64_t)(e.id());

        settings_list.push_back(std::move(body_settings));
        entity_list.push_back(e);
    }

    void jolt_context::add_body_collider(
      flecs::entity e,
      const collider_body& collider,
      const transform_physics& location,
      std::vector<JPH::BodyCreationSettings>& settings_list,
      std::vector<flecs::entity>& entity_list) {
        auto shape = create_shape_from_collider(e, collider);
        if (!shape)
            return;

        JPH::BodyCreationSettings body_settings(
          shape,
          to_jph(location.position),
          to_jph(location.quaterion_rotation),
          JPH::EMotionType::Static,
          body_layer::NonMoving);

        body_settings.mUserData = (uint64_t)(e.id());

        settings_list.push_back(std::move(body_settings));
        entity_list.push_back(e);
    }

    void jolt_context::engine_create_physics_bodies() {
        std::vector<JPH::BodyCreationSettings> settings_list;
        std::vector<flecs::entity> entity_list;

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
        console_log_info("getting here!\n");
        scene->query_builder<physics_body, collider_body, transform_physics>()
          .each([&](flecs::entity e,
                    const physics_body& phys,
                    const collider_body& col,
                    const transform_physics& loc) {
              add_body(e, phys, col, loc, settings_list, entity_list);
          });

        scene->query_builder<collider_body, transform_physics>()
          .without<physics_body>()
          .each([&](flecs::entity e,
                    const collider_body& col,
                    const transform_physics& loc) {
              add_body_collider(e, col, loc, settings_list, entity_list);
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
                  body_id.GetIndex();

            if (entity_list[i].has<collider_body>())
                entity_list[i].get_mut<collider_body>()->body_id =
                  body_id.GetIndex();
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

        // Temporary solve for transform fix change later.
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

        flecs::world registry = *scene;

        auto& body_interface = m_physics_system->GetBodyInterface();

        // Step the simulation
        m_physics_system->Update(application::delta_time(),
                                 (int)application::physics_step(),
                                 m_temp_allocator.get(),
                                 m_thread_system.get());

        //! @note FIXME: Temporary solve for transform fix

        JPH::BodyIDVector all_body_ids;
        m_physics_system->GetBodies(all_body_ids);

        for (JPH::BodyID id : all_body_ids) {
            flecs::entity flecs_object;
            flecs_object =
              flecs::entity(registry, body_interface.GetUserData(id));
            flecs::ref<transform> location;
            location = flecs_object.get_ref<transform>();

            JPH::RVec3 physics_position;
            physics_position = body_interface.GetPosition(id);
            location->Position = { physics_position.GetX(),
                                   physics_position.GetY(),
                                   physics_position.GetZ() };
            JPH::Vec3 physics_rotation;
            physics_rotation = body_interface.GetRotation(id).GetEulerAngles();
            location->Rotation = { physics_rotation.GetX(),
                                   physics_rotation.GetY(),
                                   physics_rotation.GetZ() };
        }
    }

    void jolt_context::engine_run_contact_added() {
        m_contact_listener->run_events_added();
    }

    jolt_context::~jolt_context() = default;

}
