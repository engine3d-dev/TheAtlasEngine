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
};