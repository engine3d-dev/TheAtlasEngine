#include <boost/ut.hpp>
#include <core/scene/scene.hpp>

/**
 * @name Scene Test
 * @note Some ideas of the tests we can create
 * @note We can see if the traversal through the scene and validate that
 * @note Since there are various ways of managing scenes, this part wont be
 * written at the moment
 * @note Test: if the scene we create is allow to pass in scene contexts
 * correctly
 * @note Test: User-defined customizable scenes which can help to test parts of
 * creating scenes if users want to
 * @note Test: When we create quad/octal trees we can use them to see if we
 * switch in between different scenes.
 * @note Test: Creating various scene structures and validate them
 */
boost::ut::suite<"::scene"> scene_test = []() {
    using namespace boost::ut;

    "create_object"_test = []() {

    };
};
