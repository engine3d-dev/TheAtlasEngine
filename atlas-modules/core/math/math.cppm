module;

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

export module atlas.core.math;

export namespace atlas {

    //! @brief converts vec4 to quaterion
    glm::quat to_quat(const glm::vec4& p_values) {
        return glm::quat({
          p_values.w,
          p_values.x,
          p_values.y,
          p_values.z,
        });
    }

    //! @brief converts vec3 to quaterion
    glm::highp_vec4 from_quat(const glm::vec3& p_values) {
        // converts glm::vec3 rotation to a quaternion returning the
        // quaternion-converted values to glm::highp_vec4
        auto quaternion = glm::quat(p_values);
        return glm::vec4(
          { quaternion.x, quaternion.y, quaternion.z, quaternion.w });
    }

    //! @brief converts glm::vec3 to glm::vec4
    glm::quat to_quat(const glm::vec3& p_values) {
        return glm::quat(p_values);
    }

    glm::quat to_quathp(const glm::highp_vec4& p_values) {
        return glm::quat({
          p_values.w,
          p_values.x,
          p_values.y,
          p_values.z,
        });
    }

    namespace math {
        //! @brief shorthand for glm::vec3(0.f, 0.f, 0.f)
        glm::vec3 zeroes() {
            return glm::vec3(0.f);
        }

        //! @brief shorthand for glm::vec3(1.f, 1.f, 1.f)
        glm::vec3 ones() {
            return glm::vec3(1.f);
        }

        //! @brief rotation equivalent of a zero-vector
        // sets to glm::quat(1.f, 0.f, 0.f, 0.f)
        glm::quat identity() {
            return glm::quat_identity<float, glm::defaultp>();
        }

        //! @brief shorthand for glm::vec3(0.f, 0.f, 1.f)
        glm::vec3 forward() {
            return glm::vec3(0.f, 0.f, 1.f);
        }

        //! @brief shorthand for glm::vec3(0.f, 0.f, -1.f)
        glm::vec3 backward() {
            return glm::vec3(0.f, 0.f, -1.f);
        }

        //! @brief shorthand for glm::vec3(1.f, 0.f, 0.f)
        glm::vec3 right();

        //! @brief shorthand for glm::vec3(-1.f, 0.f, 0.f)
        glm::vec3 left();

        //! @brief shorthand for glm::vec3(0.f, 1.f, 0.f)
        glm::vec3 up() {
            return glm::vec3(0.f, 1.f, 0.f);
        }

        //! @brief shorthand for glm::vec3(0.f, -1.f, 0.f)
        glm::vec3 down() {
            return glm::vec3(0.f, -1.f, 0.f);
        }
    }
};