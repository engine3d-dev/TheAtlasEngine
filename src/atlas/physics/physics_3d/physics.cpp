#include <physics/physics_3d/physics.hpp>
#include <physics/physics_3d/jolt/jolt_api.hpp>
#include <physics/physics_3d/physics_context.hpp>
#include <physics/physics_3d/jolt/jolt_context.hpp>

namespace atlas::physics {

    physics_backend test_api = physics_backend::jolt_backend;
    ref<physics_context> engine_api;
    ref<physics_api> backend_api;

    strong_ref<physics_engine> initialize_engine(
      std::pmr::polymorphic_allocator<>& p_object_allocator,
      const strong_ref<scene_object>& p_physics_object,
      flecs::world& p_registery) {
        switch (test_api) {
            case physics_backend::jolt_backend: {

                ref<jolt_context> engine_access = create_ref<jolt_context>(
                  *p_physics_object->get<jolt_settings>());

                jolt_api user_api(*p_physics_object->get_mut<jolt_config>(),
                                  engine_access->m_physics_system,
                                  p_registery);
                backend_api = create_ref<jolt_api>(user_api);

                engine_api = engine_access;

                return create_strong_ref<physics_engine>(
                  p_object_allocator,
                  *p_physics_object->get<jolt_settings>(),
                  engine_api,
                  backend_api);
            }
            default:
                throw("No physics api with that name!");
        }
    }

    void add_force(const glm::vec3& force, const physics_body& body) {
        backend_api->add_force_by_body(force, body);
    }
    //! TODO: Create the world point force funciton
    // void add_force_world_point(glm::vec3 force,
    //                            glm::vec3 position,
    //                            physics_body body) {

    //                            }

    void add_velocity(const glm::vec3& velocity, const physics_body& body) {
        backend_api->add_linear_velocity_by_body(velocity, body);
    }

    void add_angular_velocity(const glm::vec3& angular_velocity,
                              const physics_body& body) {
        backend_api->add_angular_velocity_by_body(angular_velocity, body);
    }

    void set_velocity(const glm::vec3& velocity, const physics_body& body) {
        backend_api->set_linear_velocity_by_body(velocity, body);
    }

    void set_angular_velocity(const glm::vec3& angular_velocity,
                              const physics_body& body) {
        backend_api->set_angular_velocity_by_body(angular_velocity, body);
    }
}