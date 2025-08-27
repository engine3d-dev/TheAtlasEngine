#include <boost/ut.hpp>
#include <core/scene/scene.hpp>

boost::ut::suite<"::scene"> scene_test = []() {
    using namespace boost::ut;

    atlas::scene_scope test_scope = atlas::scene_scope("Mock Scene 1");

    "create_object"_test = [&test_scope]() {
        atlas::strong_ref<atlas::scene_object> test_object =
          test_scope.create_object("Entity 1");

        test_object->set<atlas::transform>({ .position{ 1.f } });
        glm::vec3 mock_pos{ 1.f };

        const atlas::transform* t = test_object->get<atlas::transform>();
        expect(mock_pos == t->position);
    };
};
