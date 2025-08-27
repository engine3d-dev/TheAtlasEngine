#include <core/serialize/types.hpp>

namespace atlas {
    YAML::Emitter& operator<<(YAML::Emitter& p_out, const glm::highp_vec2& p_values) {
        p_out << YAML::Flow;
        p_out << YAML::BeginSeq << p_values.x << p_values.y << YAML::EndSeq;
        return p_out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& p_out, const glm::highp_vec3& p_values) {
        p_out << YAML::Flow;
        p_out << YAML::BeginSeq << p_values.x << p_values.y << p_values.z << YAML::EndSeq;
        return p_out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& p_out, const glm::highp_vec4& p_values) {
        p_out << YAML::Flow;
        p_out << YAML::BeginSeq << p_values.x << p_values.y << p_values.z << p_values.w << YAML::EndSeq;
        return p_out;
    }

    // Serializing atlas::transform component to yaml format
    YAML::Emitter& operator<<(YAML::Emitter& p_output, const transform* p_transform) {
        p_output << YAML::Key << "Transform";

        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Position" << YAML::Value << p_transform->position;
        p_output << YAML::Key << "Scale" << YAML::Value << p_transform->scale;
        p_output << YAML::Key << "Rotation" << YAML::Value << p_transform->rotation;
        p_output << YAML::Key << "Quaternion" << YAML::Value << p_transform->quaternion;
        p_output << YAML::EndMap;
        return p_output;
    }

    // Serialize perspective camera component into yaml format
    YAML::Emitter& operator<<(YAML::Emitter& p_output,  const perspective_camera* p_camera) {
        p_output << YAML::Key << "PerspectiveCamera";

        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Plane" << YAML::Value << p_camera->plane;
        p_output << YAML::Key << "Active" << YAML::Value << p_camera->is_active;
        p_output << YAML::Key << "Field of View" << YAML::Value << p_camera->field_of_view;
        p_output << YAML::EndMap;
        return p_output;
    }

    YAML::Emitter& operator<<(YAML::Emitter& p_output,  const material* p_material) {
        p_output << YAML::Key << "Material";

        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Model Path" << YAML::Value << p_material->model_path;
        p_output << YAML::Key << "Texture Path" << YAML::Value << p_material->texture_path;
        p_output << YAML::EndMap;
        return p_output;
    }
};