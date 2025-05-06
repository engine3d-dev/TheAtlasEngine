#include <physics/physics_3d/jolt/jolt_context.hpp>
#include <cstdarg>
#include <engine_logger.hpp>

#include <physics/physics_3d/data/physics_body.hpp>
#include <physics/physics_3d/data/collider_body.hpp>
#include <components/transform.hpp>
#include <physics/types.hpp>
#include <scene/scene.hpp>
#include <physics/physics_3d/jolt/jolt_helper.hpp>

namespace atlas::physics {

    static void trace_impl(const char* p_in_fmt, ...) {
        va_list list;
        va_start(list, p_in_fmt);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), p_in_fmt, list);
        va_end(list);
        console_log_error("{}", buffer);
    }

    static bool factory_initialized = false;
    jolt_context::jolt_context(jolt_settings p_settings) {
        JPH::RegisterDefaultAllocator();
        JPH::Trace = trace_impl;

        if (!factory_initialized) {
            JPH::Factory::sInstance = new JPH::Factory();
            JPH::RegisterTypes();
            factory_initialized = true;
        }

        m_settings = p_settings;
        m_temp_allocator = create_ref<JPH::TempAllocatorImpl>(10 * 1024 * 1024);
        m_physics_system = create_ref<JPH::PhysicsSystem>();
        m_broad_phase_layer_interface =
          create_ref<broad_phase_layer_interface>();
        m_object_vs_broadphase_filter =
          create_ref<object_vs_broadphase_layer>();
        m_object_layer_pair_filter = create_ref<object_layer_pair_filter>();

        if (m_settings.thread_type == thread_system::Default) {
            m_thread_system = create_scope<JPH::JobSystemThreadPool>(
              m_settings.max_jobs,
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

        console_log_info("jolt context", "Physics system initialized.");
    }

    void add_body(flecs::entity e,
                  const physics_body& body,
                  const collider_body& collider,
                  const transform_physics& location,
                  std::vector<JPH::BodyCreationSettings>& settings_list,
                  std::vector<flecs::entity>& entity_list,
                  std::unordered_map<uint64_t, JPH::RefConst<JPH::Shape>>&
                    p_shape_registry) {
        JPH::Shape* created_shape = nullptr;

        switch (collider.shape_type) {
            case collider_shape::Box: {
                JPH::BoxShapeSettings shape_settings(
                  to_jph(collider.half_extents));
                auto result = shape_settings.Create();
                if (result.HasError()) {
                    console_log_fatal("Box shape creation error: {}",
                                      result.GetError());
                    return;
                }
                created_shape = result.Get();
                p_shape_registry[e.id()] = created_shape;
                break;
            }
            case collider_shape::Sphere: {
                JPH::SphereShapeSettings shape_settings(collider.radius);
                auto result = shape_settings.Create();
                if (result.HasError()) {
                    console_log_fatal("Sphere shape creation error: {}",
                                      result.GetError());
                    return;
                }
                created_shape = result.Get();
                p_shape_registry[e.id()] = created_shape;
                break;
            }
            case collider_shape::Capsule: {
                JPH::CapsuleShapeSettings shape_settings(
                  collider.capsule_half_height, collider.radius);
                auto result = shape_settings.Create();
                if (result.HasError()) {
                    console_log_fatal("Capsule shape creation error: {}",
                                      result.GetError());
                    return;
                }
                created_shape = result.Get();
                p_shape_registry[e.id()] = created_shape;
                break;
            }
            default:
                console_log_fatal("Unknown shape type.");
                return;
        }

        JPH::BodyCreationSettings body_settings(
          created_shape,
          to_jph(location.position),
          to_jph(location.quaterion_rotation),
          (JPH::EMotionType)body.body_type,
          (uint32_t)body.body_layer_type);

        body_settings.mUserData = static_cast<uintptr_t>(e.raw_id());
        settings_list.push_back(std::move(body_settings));
        entity_list.push_back(e);
    }

    void add_body_collider(
      flecs::entity e,
      const collider_body& collider,
      const transform_physics& location,
      std::vector<JPH::BodyCreationSettings>& settings_list,
      std::vector<flecs::entity>& entity_list,
      std::unordered_map<uint64_t, JPH::RefConst<JPH::Shape>>&
        p_shape_registry) {
        JPH::Shape* created_shape = nullptr;

        switch (collider.shape_type) {
            case collider_shape::Box: {
                JPH::BoxShapeSettings shape_settings(
                  to_jph(collider.half_extents));
                auto result = shape_settings.Create();
                if (result.HasError()) {
                    console_log_fatal("Box shape creation error: {}",
                                      result.GetError());
                    return;
                }
                created_shape = result.Get();
                p_shape_registry[e.id()] = created_shape;
                break;
            }
            case collider_shape::Sphere: {
                JPH::SphereShapeSettings shape_settings(collider.radius);
                auto result = shape_settings.Create();
                if (result.HasError()) {
                    console_log_fatal("Sphere shape creation error: {}",
                                      result.GetError());
                    return;
                }
                created_shape = result.Get();
                p_shape_registry[e.id()] = created_shape;
                break;
            }
            case collider_shape::Capsule: {
                JPH::CapsuleShapeSettings shape_settings(
                  collider.capsule_half_height, collider.radius);
                auto result = shape_settings.Create();
                if (result.HasError()) {
                    console_log_fatal("Capsule shape creation error: {}",
                                      result.GetError());
                    return;
                }
                created_shape = result.Get();
                p_shape_registry[e.id()] = created_shape;
                break;
            }
            default:
                console_log_fatal("Unknown shape type.");
                return;
        }

        JPH::BodyCreationSettings body_settings(
          created_shape,
          to_jph(location.position),
          to_jph(location.quaterion_rotation),
          JPH::EMotionType::Static,
          0);

        body_settings.mUserData = static_cast<uintptr_t>(e.raw_id());

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
              add_body(e,
                       phys,
                       col,
                       loc,
                       settings_list,
                       entity_list,
                       m_shape_registry);
          });

        scene->query_builder<collider_body, transform_physics>()
          .without<physics_body>()
          .each([&](flecs::entity e,
                    const collider_body& col,
                    const transform_physics& loc) {
              add_body_collider(
                e, col, loc, settings_list, entity_list, m_shape_registry);
          });

        if (settings_list.empty()) {
            console_log_error("No physics bodies to create.");
            return;
        }

        auto& body_interface = m_physics_system->GetBodyInterface();
        std::vector<JPH::BodyID> body_ids;
        body_ids.reserve(settings_list.size());

        for (size_t i = 0; i < settings_list.size(); ++i) {
            const auto& settings = settings_list[i];

            if (!settings.GetShape()) {
                console_log_fatal("Shape is null at index {}", i);
                continue;
            }

            JPH::Body* body = body_interface.CreateBodyWithID((JPH::BodyID)settings.mUserData,settings);
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

        //! @note this is where you would deserialize
    }

    void jolt_context::engine_run_physics_step() {

        console_log_info("jolt context", "Running physics step...");

        m_physics_system->Update(application::delta_time(),
                                 (int)application::physics_step(),
                                 m_temp_allocator.get(),
                                 m_thread_system.get());
    }

    jolt_context::~jolt_context() = default;

}
