module;
#include <boost/ut.hpp>
export module atlas.tests:scene;

import atlas.core.scene;
import atlas.core.scene.game_object;
import atlas.core.scene.components;
import atlas.core.event;



export void test_scenes() {
    boost::ut::suite<"::scene"> scene_test = []() {
        using namespace boost::ut;
        atlas::event::bus test_event_bus;
        atlas::scene test_scene = atlas::scene("Mock Scene 1", test_event_bus);

        "create_object"_test = [&test_scene]() {
            atlas::game_object test_object = test_scene.entity("Entity 1");
            test_object.add<atlas::transform>();

            expect(test_object.has<atlas::transform>());
        };
    };
};