#include <boost/ut.hpp>
#include <core/scene/scene.hpp>
#include <core/event/event_bus.hpp>
#include <core/scene/components.hpp>

boost::ut::suite<"::scene"> scene_test = []() {
    using namespace boost::ut;
    atlas::event::event_bus test_event_bus;
    atlas::scene test_scene = atlas::scene("Mock Scene 1", test_event_bus);

    "create_object"_test = [&test_scene]() {
        atlas::game_object test_object = test_scene.entity("Entity 1");
        test_object.add<atlas::transform>();

        expect(test_object.has<atlas::transform>());
    };
};
