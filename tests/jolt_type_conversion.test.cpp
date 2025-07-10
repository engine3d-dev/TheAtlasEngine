#include <boost/ut.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec3.h>
#include <Jolt/Math/Quat.h>
#include <core/scene/scene.hpp>
#include <core/scene/scene_object.hpp>

#include <physics/physics_3d/jolt/jolt_helper.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>
#include <physics/physics_3d/physics.hpp>

namespace atlas {

    bool approx_equal(float a, float b, float epsilon = 1e-6f) {
        return std::fabs(a - b) < epsilon;
    }

    boost::ut::suite<"jolt_helper"> jolt_helper_tests = [] {
        using namespace boost::ut;

        "to_jph::vec3 conversion"_test = [] {
            glm::vec3 glm_vec{ 1.0f, 2.0f, 3.0f };
            JPH::Vec3 jph_vec = physics::to_jph(glm_vec);

            expect(approx_equal(jph_vec.GetX(), 1.0f));
            expect(approx_equal(jph_vec.GetY(), 2.0f));
            expect(approx_equal(jph_vec.GetZ(), 3.0f));
        };

        "to_jph::quat conversion"_test = [] {
            glm::quat glm_quat{ 0.707f, 0.0f, 0.707f, 0.0f };
            glm::vec4 quat_vec =
              glm::vec4(glm_quat.x, glm_quat.y, glm_quat.z, glm_quat.w);

            JPH::Quat jph_quat = physics::to_jph(quat_vec);

            expect(approx_equal(jph_quat.GetX(), 0.0f));
            expect(approx_equal(jph_quat.GetY(), 0.707f));
            expect(approx_equal(jph_quat.GetZ(), 0.0f));
            expect(approx_equal(jph_quat.GetW(), 0.707f));
        };
    };
};