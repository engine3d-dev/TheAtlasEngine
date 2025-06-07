#include <physics/physics_3d/physics.hpp>
#include <physics/physics_3d/jolt/jolt_api.hpp>
#include <physics/physics_3d/physics_context.hpp>
#include <physics/physics_3d/jolt/jolt_context.hpp>

namespace atlas::physics {

    physics_backend test_api = physics_backend::JoltBackend;
    ref<physics_context> engine_api;
    ref<physics_api> backend_api;

    ref<physics_engine> initialize_engine(
      const ref<scene_object>& p_physics_object,
      flecs::world& p_registery) {
        switch (test_api) {
            case physics_backend::JoltBackend: {

                ref<jolt_context> engine_access = create_ref<jolt_context>(
                  *p_physics_object->get<jolt_settings>());

                jolt_api user_api(*p_physics_object->get_mut<jolt_config>(),
                                  engine_access->m_physics_system,
                                  p_registery);
                backend_api = create_ref<jolt_api>(user_api);

                engine_api = engine_access;

                return create_ref<physics_engine>(
                  *p_physics_object->get<jolt_settings>(),
                  engine_api,
                  backend_api);
            }
            default:
                return nullptr;
        }
        return nullptr;
    }

    // void add_force(glm::vec3 force, physics_body body);

    // void add_force_world_point(glm::vec3 force,
    //                            glm::vec3 position,
    //                            physics_body body);

    // void add_force_on_contact(glm::vec3 force, physics_body body);

    // void set_velocity(const glm::vec3& velocity, physics_body body);

    // void set_angular_velocity(const glm::vec3& angular_velocity, physics_body
    // body);
}