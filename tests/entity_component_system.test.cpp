#include <boost/ut.hpp>
#include <core/math/types.hpp>
#include <scene/ecs/entity.hpp>
#include <scene/scene.hpp>

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
            return engine3d::vector3<JPH::Vec3>(Position);
        }

        void OnUpdate(){
            JPH::Vec3 InitialPosition = {0.f, 1.0f, 2.0f};
            Position += InitialPosition;
        }
    private:
        JPH::Vec3 Position;
    };
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

boost::ut::suite<"ecs::components"> ecs_test = [](){
    using namespace boost::ut;

    engine3d::Scene test_scene1 = engine3d::Scene("New Scene");

    "CreateEntity::AddComponent<T>"_test = [&test_scene1]{
        engine3d::EntityObject entity = test_scene1.CreateEntity("New Entity1");
        
        expect(entity.IsAlive());

        entity.AddComponent<TestTagComponent>();
        expect(entity.HasComponent<TestTagComponent>());
    };

    "GetComponent"_test = [&test_scene1](){
        engine3d::EntityObject entity = test_scene1.CreateEntity("New Entity1");
        entity.AddComponent<TestTagComponent>();
        //! @note Flecs require you to get the component for the use of only reading the data from component
        //! @note To actually set values to that component you use the SetComponent usnig their API they specify
        entity.SetComponent<TestTagComponent>({.Tag = "New Entity"});
        
        const TestTagComponent* get_tag = entity.GetComponent<TestTagComponent>();
        expect(get_tag->Tag == "New Entity");
    };

    "::SetComponent"_test = [&test_scene1](){
        engine3d::EntityObject entity = test_scene1.CreateEntity("New Entity1");

        engine3d::MockProjectileMissle projectile;
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
        
        //! @note Setting multiple types
        // entity.SetComponent<TestTransform, TestVelocity, TagComponent>(transform, test_velocity, assign_tag);
        // const TagComponent* modified_tag = entity.GetComponent<TagComponent>();

        // expect(modified_tag->Tag == assign_tag.Tag);


    };


};