#include <boost/ut.hpp>
#include <core/math/types.hpp>
#include <scene/ecs/entity.hpp>
#include <flecs.h>

#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec4.h>

namespace engine3d{

    template<>
    struct vector3<JPH::Vec3>{
        vector3() = default;
        
        vector3(const JPH::Vec3& p_Other){
            value = {p_Other.GetX(), p_Other.GetY(), p_Other.GetZ()};
        }

        operator glm::vec3() {
            return value;
        }

        glm::vec3 operator=(const JPH::Vec3& p_Other) {
            return {p_Other.GetX(), p_Other.GetY(), p_Other.GetZ()};
        }

        bool operator==(const glm::vec3& p_Other){
            return (value.x == p_Other.x and value.y == p_Other.y and value.z == p_Other.z);
        }
        
    private:
        glm::vec3 value;
    };

    /**
     * @name Mock Projectile Missle Test
     * @note Essentially an active component
     * @note Purpose of this is to see if we have a dummy test that simulates some logic
     * @note Not actual logic but utilizes our type conversion and checks if and passes that to our transform
     * @note What I am checking here is if our type conversion will set the transform of our entity
     * @note In our later tests we can for actual logic correctness, this is only for testing types.hpp is working
     * @note If something breaks in math.test.cpp that should also happen here as well
    */
    struct MockProjectileMissle{
    public:
        MockProjectileMissle(){
            Position = {1.f, 1.f, 1.f};
        }

        glm::vec3 GetPosition() const {
            return vector3<JPH::Vec3>(Position);
        }

        void OnUpdate(){
            JPH::Vec3 InitialPosition = {0.f, 1.0f, 2.0f};
            Position += InitialPosition;
        }
    private:
        JPH::Vec3 Position;
    };
    
    struct TestTagComponent{
        std::string Tag = "Undefined";
        uint64_t UUID = -1;
    };

    struct TestTransform {
        glm::vec3 Postion = {1.f, 1.f, 1.f};
        glm::vec3 Scale = {0.f, 0.f, 0.f};
        glm::vec3 Rotation = {0.f, 0.f, 0.f};
    };

    struct TestVelocity{
        glm::vec3 Position;
    };


    template<typename Component, typename Component2>
    static flecs::query<Component, Component2> QuerySelectedEntity(flecs::world* p_Registry){
        return p_Registry->query<Component, Component2>();
    }

    boost::ut::suite<"ecs::component"> ecs_test = [](){
        using namespace boost::ut;

        //! @note Each scene will define flecs::world typically
        //! @note flecs::world is how flecs (ECS) stores entities and components
        flecs::world scene_registry;

        "CreateEntity::AddComponent<T>"_test = [&scene_registry]{
            EntityObject entity = EntityObject(&scene_registry, "Mock Entity");
            
            expect(entity.IsAlive());

            entity.AddComponent<TestTagComponent>();
            expect(entity.HasComponent<TestTagComponent>());
        };

        "GetComponent"_test = [&scene_registry](){
            EntityObject entity = EntityObject(&scene_registry, "Mock Entity2");
            entity.AddComponent<TestTagComponent>();
            //! @note Flecs require you to get the component for the use of only reading the data from component
            //! @note To actually set values to that component you use the SetComponent usnig their API they specify
            entity.SetComponent<TestTagComponent>({.Tag = "New Entity"});
            
            const TestTagComponent* get_tag = entity.GetComponent<TestTagComponent>();
            expect(get_tag->Tag == "New Entity");
        };

        "::SetComponent"_test = [&scene_registry](){
            // EntityObject entity = test_scene1.CreateEntity("New Entity1");
            EntityObject entity = EntityObject(&scene_registry, "New Entity");
            MockProjectileMissle projectile;
            projectile.OnUpdate();

            TestTransform transform;
            transform.Postion = projectile.GetPosition();

            TestVelocity test_velocity;
            test_velocity.Position = {2.f, 2.f, 0.f};

            //! @note This is how you would set multiple
            entity.SetComponent<TestTransform, TestVelocity>(transform, test_velocity);

            expect(transform.Postion == projectile.GetPosition());

            //! @note How flecs sets this up is if TestVelocity is a component that is not set.
            //! @note Flecs will add that component, then set the values to that component.
            expect(test_velocity.Position == entity.GetComponent<TestVelocity>()->Position);
        };

        //! @note Need to learn more about how felcs::query works
        "::QueryComponent"_test = [](){
            // EntityObject entity = EntityObject(&scene_registry, "Querying Entity");

            // //! @note Creating a flecs::query<T, U>
            // auto query_test = QuerySelectedEntity<TestTransform, TestVelocity>(&scene_registry);

            // //! @note flec's way of using those queried components to iterate through components stored
            // query_test.each([](TestTransform& t, const TestVelocity& v){
            //     t.Postion += v.Position;
            // });


        };


    };
}; // end of engine3d namespace

