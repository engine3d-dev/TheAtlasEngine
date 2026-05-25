module;
#include <boost/ut.hpp>
#include <flecs.h>

#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec4.h>
#include <glm/glm.hpp>
export module atlas.tests:entity_component_system;

import atlas.core.scene;
import atlas.core.scene.game_object;
import atlas.core.math.types;
import atlas.core.event;
import atlas.drivers.jolt_cpp.types;

namespace atlas {
    /**
     * @name Mock Projectile Missle Test
     * @note Essentially an active component
     * @note Purpose of this is to see if we have a dummy test that simulates
     * some logic
     * @note Not actual logic but utilizes our type conversion and checks if and
     * passes that to our transform
     * @note What I am checking here is if our type conversion will set the
     * transform of our entity
     * @note In our later tests we can for actual logic correctness, this is
     * only for testing types.hpp is working
     * @note If something breaks in math.test.cpp that should also happen here
     * as well
     */
    struct mock_projectile {
    public:
        mock_projectile() { m_position = { 1.f, 1.f, 1.f }; }

        [[nodiscard]] glm::vec3 position() const {
            return vector3<JPH::Vec3>(m_position);
        }

        void on_update() {
            JPH::Vec3 initial_position = { 0.f, 1.0f, 2.0f };
            m_position += initial_position;
        }

    private:
        JPH::Vec3 m_position;
    };

    struct test_tag_component {
        std::string tag = "Undefined";
        uint64_t uuid = -1;
    };

    struct test_transform {
        glm::vec3 position = { 1.f, 1.f, 1.f };
        glm::vec3 scale = { 0.f, 0.f, 0.f };
        glm::vec3 rotation = { 0.f, 0.f, 0.f };
    };

    struct test_velocity {
        glm::vec3 position;
    };
};

export void
test_entity_component_system() {
    boost::ut::suite<"ecs::component"> ecs_test = []() {
        using namespace boost::ut;

        //! @note Each scene will define flecs::world typically
        //! @note flecs::world is how flecs (ECS) stores entities and components
        atlas::event::bus test_event_bus;
        atlas::scene test_scene = atlas::scene("Mock Scene", test_event_bus);

        "create_entity::add<T>"_test = [&test_scene] {
            // flecs::entity entity = scene_registry.entity("Mock Entity");
            atlas::game_object entity = test_scene.entity("Mock Entity");

            // expect(entity.is_alive());

            entity.add<atlas::test_tag_component>();
            expect(entity.has<atlas::test_tag_component>());
        };

        "create_entity::get"_test = [&test_scene]() {
            atlas::game_object entity = test_scene.entity("Mock Entity 2");
            entity.add<atlas::test_tag_component>();
            // flecs requires reading only operations are through the get<T> API
            // to write or set new parameters you can use get_mut<T> or
            // set<T>(T&&); in this case, I use set<T> in this test case
            entity.set<atlas::test_tag_component>({ .tag = "New Entity" });

            const atlas::test_tag_component* get_tag =
              entity.get<atlas::test_tag_component>();
            expect(get_tag->tag == "New Entity");
        };

        "create_entity::set"_test = [&test_scene]() {
            atlas::game_object entity = test_scene.entity("New Entity");
            atlas::mock_projectile projectile;
            projectile.on_update();
            entity.set<atlas::mock_projectile>(projectile);

            expect(entity.has<atlas::mock_projectile>());

            // test_transform transform;
            // transform.position = projectile.position();

            // test_velocity mock_velocity;
            // mock_velocity.position = { 2.f, 2.f, 0.f };

            // expect(transform.position == projectile.position());

            // expect(mock_velocity.position ==
            //        entity.get<test_velocity>().position);
        };
    };
}