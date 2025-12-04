#include <core/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <core/scene/scene.hpp>
#include <core/event/event_bus.hpp>

#include <drivers/jolt-cpp/jolt-imports.hpp>
#include <drivers/jolt-cpp/types.hpp>
#include <drivers/jolt-cpp/jolt_components.hpp>

#include <boost/ut.hpp>

namespace atlas {

    /**
     * @brief The physics is fully reliant on the ecs working. As it utilizes
     * queries as well as scene to navigate location of entities.
     *
     */
    boost::ut::suite<"physics_engine_integration"> engine_test = [] {
        using namespace boost::ut;

        // Setup to do testing and assume we have a specified current scene that
        // gets provided to you during that runtime
        atlas::event::event_bus bus;
        atlas::scene test_environment_scene = atlas::scene("Mock 1", bus);

        // Do some testing if these two collides or something like that
        // atlas::game_object obj1 = test_environment_scene.entity("Entity
        // 1"); atlas::game_object obj2 =
        // test_environment_scene.entity("Entity 2");

        "on_collision_enter"_test = [] {};
    };
};