#include <boost/ut.hpp>
#include <core/math/types.hpp>
#include <flecs.h>
#include <core/scene/entity.hpp>

#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec4.h>

namespace atlas {

    template<>
    struct vector3<JPH::Vec3> {
        vector3() = default;

        vector3(const JPH::Vec3& p_other) {
            value = { p_other.GetX(), p_other.GetY(), p_other.GetZ() };
        }

        operator glm::vec3() { return value; }

        glm::vec3 operator=(const JPH::Vec3& p_other) {
            return { p_other.GetX(), p_other.GetY(), p_other.GetZ() };
        }

        bool operator==(const glm::vec3& p_other) {
            return (value.x == p_other.x and value.y == p_other.y and
                    value.z == p_other.z);
        }

    private:
        glm::vec3 value;
    };

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

        glm::vec3 get_position() const {
            return vector3<JPH::Vec3>(m_position);
        }

        void on_update() {
            JPH::Vec3 InitialPosition = { 0.f, 1.0f, 2.0f };
            m_position += InitialPosition;
        }

    private:
        JPH::Vec3 m_position;
    };

    struct TestTagComponent {
        std::string Tag = "Undefined";
        uint64_t UUID = -1;
    };

    struct TestTransform {
        glm::vec3 Postion = { 1.f, 1.f, 1.f };
        glm::vec3 Scale = { 0.f, 0.f, 0.f };
        glm::vec3 Rotation = { 0.f, 0.f, 0.f };
    };

    struct TestVelocity {
        glm::vec3 Position;
    };

    template<typename Component, typename Component2>
    static flecs::query<Component, Component2> query_selected_entity(
      flecs::world* p_registry) {
        return p_registry->query<Component, Component2>();
    }

    boost::ut::suite<"ecs::component"> ecs_test = []() {
        using namespace boost::ut;

        //! @note Each scene will define flecs::world typically
        //! @note flecs::world is how flecs (ECS) stores entities and components
        flecs::world scene_registry;

        "create_entity::add<T>"_test = [&scene_registry] {
            entity_t entity = entity_t(&scene_registry, "Mock Entity");

            expect(entity.is_alive());

            entity.add<TestTagComponent>();
            expect(entity.has<TestTagComponent>());
        };

        "create_entity::get"_test = [&scene_registry]() {
            entity_t entity = entity_t(&scene_registry, "Mock Entity2");
            entity.add<TestTagComponent>();
            //! @note Flecs require you to get the component for the use of only
            //! reading the data from component
            //! @note To actually set values to that component you use the set
            //! usnig their API they specify
            entity.set<TestTagComponent>({ .Tag = "New Entity" });

            const TestTagComponent* get_tag = entity.get<TestTagComponent>();
            expect(get_tag->Tag == "New Entity");
        };

        "create_entity::set"_test = [&scene_registry]() {
            // entity_t entity = test_scene1.CreateEntity("New Entity1");
            entity_t entity = entity_t(&scene_registry, "New Entity");
            mock_projectile projectile;
            projectile.on_update();

            TestTransform transform;
            transform.Postion = projectile.get_position();

            TestVelocity test_velocity;
            test_velocity.Position = { 2.f, 2.f, 0.f };

            //! @note This is how you would set multiple
            entity.set<TestTransform, TestVelocity>(transform, test_velocity);

            expect(transform.Postion == projectile.get_position());

            //! @note How flecs sets this up is if TestVelocity is a component
            //! that is not set.
            //! @note Flecs will add that component, then set the values to that
            //! component.
            expect(test_velocity.Position ==
                   entity.get<TestVelocity>()->Position);
        };

        //! @note Need to learn more about how felcs::query works
        "create_entity::query_component"_test = []() {
            // entity_t entity = entity_t(&scene_registry, "Querying Entity");

            // //! @note Creating a flecs::query<T, U>
            // auto query_test = QuerySelectedEntity<TestTransform,
            // TestVelocity>(&scene_registry);

            // //! @note flec's way of using those queried components to iterate
            // through components stored query_test.each([](TestTransform& t,
            // const TestVelocity& v){
            //     t.Postion += v.Position;
            // });
        };
    };
}; // namespace atlas
