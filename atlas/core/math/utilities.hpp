#pragma once
#include <glm/glm.hpp>

namespace atlas {

    //! @brief converts vec4 to quaterion
    glm::quat to_quat(const glm::vec4& p_values);

    //! @brief converts vec3 to quaterion
    glm::quat to_quat(const glm::vec3& p_values);

    //! @brief converts glm::vec3 to glm::vec4
    glm::highp_vec4 from_quat(const glm::vec3& p_values);

    glm::quat to_quathp(const glm::highp_vec4& p_values);

    namespace math {
        //! @brief shorthand for glm::vec3(0.f, 0.f, 0.f)
        glm::vec3 zeroes();

        //! @brief shorthand for glm::vec3(1.f, 1.f, 1.f)
        glm::vec3 ones();

        //! @brief rotation equivalent of a zero-vector
        // sets to glm::quat(1.f, 0.f, 0.f, 0.f)
        glm::quat identity();

        //! @brief shorthand for glm::vec3(0.f, 0.f, 1.f)
        glm::vec3 forward();

        //! @brief shorthand for glm::vec3(0.f, 0.f, -1.f)
        glm::vec3 backward();

        //! @brief shorthand for glm::vec3(1.f, 0.f, 0.f)
        glm::vec3 right();

        //! @brief shorthand for glm::vec3(-1.f, 0.f, 0.f)
        glm::vec3 left();

        //! @brief shorthand for glm::vec3(0.f, 1.f, 0.f)
        glm::vec3 up();

        //! @brief shorthand for glm::vec3(0.f, -1.f, 0.f)
        glm::vec3 down();
    }
};