#include <core/math/types.hpp>


#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec4.h>
#include <boost/ut.hpp>

namespace atlas{
    /**
     * @name Specialized template
     * @note What this does is customizing to utilize a different mathematical data type
     * @note We are not defining our own vector and the operations of how that is done
     * @note We simply specialized the type we expect, then do all of that operation with that specified type
     *       - then we simply return to the default glm type once we are done with those computation
    */
    template<>
    struct vector3<JPH::Vec3>{
        vector3() = default;
        
        vector3(const JPH::Vec3& p_other){
            value = {p_other.GetX(), p_other.GetY(), p_other.GetZ()};
        }

        operator glm::vec3() {
            return value;
        }

        glm::vec3 operator=(const JPH::Vec3& p_other) {
            return {p_other.GetX(), p_other.GetY(), p_other.GetZ()};
        }

        bool operator==(const glm::vec3& p_other){
            return (value.x == p_other.x and value.y == p_other.y and value.z == p_other.z);
        }

    private:
        glm::vec3 value;
    };
};

/**
 * @name MockTestTransform
 * @note This is testing if even though our computation is done using jolt's mathematical data types
 * @note We can still return vec3
 * @note Only down-side that I can see being a problem is users need to define specific operator overloads.
 * @note TODO: Just need to add a way for getting some of these types inferenced (at least to the types that are customized by user)
 *        - Just so users are not having to pre-define them
 * @note Minimal example if we have a projectile that requires physics that has some collider with it and Jolt requires you to provide a vec3
 *       and users end up defining how projectiles work and prefer Jolt's vec3. Then they define that how they prefer and just convert it to a glm::vec3
 * @note Looking at the example below. I just test to see if we can actually do addition, and basic arithmetic as a minimal example
 * @note We just check if the conversion to glm is happening correctly, checking for type conversion validation
*/
class mock_test_projectile{
public:
    mock_test_projectile() = default;

    glm::vec3 get_position() const {
        return atlas::vector3<JPH::Vec3>(m_position);
    }

    void on_update(){
        m_position = {0, 1, 2};
        JPH::Vec3 my_value2 = {1, 2, 3}; // simulating some data
        m_position += my_value2;
    }

private:
    JPH::Vec3 m_position;
};

/**
 * @name atlas::custom_vec3 test suite
 * @note Tests for customizable data types that can be defined by the user.
 * @note This considers that if users want to utilize a different math library then they can override
 *       the computation to be done with math library of their choice
 * @note All that is required is that they just need to convert to glm::vec2, vec3, or vec4 and other glm types
*/



boost::ut::suite<"::basic_vec3"> original_vec3_test = [](){
    using namespace boost::ut;

    "basic_vec3_unit1_test"_test = [](){
        glm::vec3 original_val = {1, 2, 3};
        // atlas::vec3 unit1_test = {1, 2, 3};
        atlas::vec3 unit1 = {{1.f, 2.f, 3.f}};


        expect(original_val == unit1);
    };
};

boost::ut::suite<"::custom_vec3"> vec3_test = [](){
    using namespace boost::ut;

    "custom_vec3"_test = [](){
        JPH::Vec3 my_value = {0, 1, 2};

        atlas::vector3<JPH::Vec3> vec3_unit = my_value;
        glm::vec3 expected = {0, 1, 2};


        expect(expected == vec3_unit);
    };

    "custom_vec3_add"_test = [](){
        JPH::Vec3 my_value = {0, 1, 2};
        JPH::Vec3 my_value2 = {1, 2, 3};

        atlas::vector3<JPH::Vec3> vec3_unit = my_value;
        atlas::vector3<JPH::Vec3> vec3_unit2 = (my_value + my_value2);

        auto vec3_unit2_result = vec3_unit2;
        
        glm::vec3 expected = {0, 1, 2};

        glm::vec3 expected_unit2 = {1, 3, 5};

        expect(expected == vec3_unit);
        expect(expected_unit2 == vec3_unit2_result);
    };

    "custom_vec3_mock_class"_test = [](){
        mock_test_projectile mock;
        mock.on_update();

        glm::vec3 expected_unit2 = {1, 3, 5};

        expect(expected_unit2 == mock.get_position());
    };
};