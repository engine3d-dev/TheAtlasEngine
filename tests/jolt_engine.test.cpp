#include <core/engine_logger.hpp>

#include <core/system/registry.hpp>

#include <boost/ut.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec3.h>
#include <Jolt/Math/Quat.h>
#include <core/scene/world.hpp>
#include <core/scene/scene.hpp>

#include <physics/physics_3d/jolt/jolt_helper.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>
#include <physics/physics_3d/physics.hpp>

namespace atlas {

    /**
     * @brief The physics is fully reliant on the ecs working. As it utilizes
     * queries as well as scene to navigate location of entities.
     *
     */
    boost::ut::suite<"physics_engine_integration"> engine_test = [] {
        using namespace boost::ut;

        class test_scene : public scene_scope {
        public:
            test_scene(const std::string& p_tag)
              : scene_scope(p_tag){};

            std::pmr::polymorphic_allocator<> m_object_allocator;
        };

        class editor_world : public world_scope {
        public:
            editor_world(const std::string& p_tag)
              : world_scope(p_tag){};
        };

        "initialize_engine with physics_world and setup object"_test = [] {
            system_registry m_log_registery = system_registry("Registry");

            editor_world editor = editor_world("Editor World");

            // ref<test_scene> physics_world =
            // editor.create_custom_scene<test_scene>("LevelScene");
            // expect(eq(physics_world->get_tag(), std::string("LevelScene")));

            /**
             * @bug FIXME: This does not work because the operator in
             * scene_scope:
             * operator flecs::world() { return m_registry; }
             *
             * Returns a copy of the flecs world. This can be an invalid
             * state. Instead have a get_world() function.
             */
            // flecs::world test_world = *physics_world;

            // expect(test_world.get_info() != nullptr);

            // strong_ref<scene_object> physics_setup =
            //   physics_world->create_object("Physics Tooling");

            // // Add required config components
            // physics_setup->add<physics::jolt_settings>();
            // physics_setup->add<physics::jolt_config>();

            // // Now initialize the physics engine using that setup
            // optional_ref<physics::physics_engine> engine =
            //   atlas::physics::initialize_engine(physics_world->m_object_allocator,physics_setup,
            //   test_world);

            // expect(engine != nullptr);

            //   engine->start_runtime();
            //   engine->physics_step();
            //   engine->stop_runtime();
        };
    };
};