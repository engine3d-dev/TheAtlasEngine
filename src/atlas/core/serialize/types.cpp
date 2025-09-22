#include <core/serialize/types.hpp>

namespace atlas {

    YAML::Emitter& operator<<(YAML::Emitter& p_out,
                              const glm::highp_vec2& p_values) {
        p_out << YAML::Flow;
        p_out << YAML::BeginSeq << p_values.x << p_values.y << YAML::EndSeq;
        return p_out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& p_out,
                              const glm::highp_vec3& p_values) {
        p_out << YAML::Flow;
        p_out << YAML::BeginSeq << p_values.x << p_values.y << p_values.z
              << YAML::EndSeq;
        return p_out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& p_out,
                              const glm::highp_vec4& p_values) {
        p_out << YAML::Flow;
        p_out << YAML::BeginSeq << p_values.x << p_values.y << p_values.z
              << p_values.w << YAML::EndSeq;
        return p_out;
    }

    // Serializing atlas::transform component to yaml format
    YAML::Emitter& operator<<(YAML::Emitter& p_output,
                              const transform* p_transform) {
        p_output << YAML::Key << "Transform";

        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Position" << YAML::Value
                 << p_transform->position;
        p_output << YAML::Key << "Scale" << YAML::Value << p_transform->scale;
        p_output << YAML::Key << "Rotation" << YAML::Value
                 << p_transform->rotation;
        p_output << YAML::Key << "Quaternion" << YAML::Value
                 << p_transform->quaternion;
        p_output << YAML::EndMap;
        return p_output;
    }

    // Serialize perspective camera component into yaml format
    YAML::Emitter& operator<<(YAML::Emitter& p_output,
                              const perspective_camera* p_camera) {
        p_output << YAML::Key << "PerspectiveCamera";

        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Plane" << YAML::Value << p_camera->plane;
        p_output << YAML::Key << "Active" << YAML::Value << p_camera->is_active;
        p_output << YAML::Key << "Field of View" << YAML::Value
                 << p_camera->field_of_view;
        p_output << YAML::EndMap;
        return p_output;
    }

    YAML::Emitter& operator<<(YAML::Emitter& p_output,
                              const material* p_material) {
        p_output << YAML::Key << "Material";

        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Model Path" << YAML::Value
                 << p_material->model_path;
        p_output << YAML::Key << "Texture Path" << YAML::Value
                 << p_material->texture_path;
        p_output << YAML::EndMap;
        return p_output;
    }

    YAML::Emitter& operator<<(YAML::Emitter& p_output,
                              const collider_body* p_collider) {
        p_output << YAML::Key << "Collider Body";

        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Enabled" << YAML::Value
                 << p_collider->collision_enabled;
        p_output << YAML::Key << "Shape Type" << YAML::Value
                 << (int)p_collider->shape_type;
        p_output << YAML::Key << "Extent" << YAML::Value
                 << p_collider->half_extents;
        p_output << YAML::Key << "Radius" << YAML::Value << p_collider->radius;
        p_output << YAML::Key << "Capsule Height" << YAML::Value
                 << p_collider->capsule_half_height;
        p_output << YAML::Key << "Body ID" << YAML::Value
                 << p_collider->body_id;
        p_output << YAML::EndMap;

        return p_output;
    }

    YAML::Emitter& operator<<(YAML::Emitter& p_output,
                              const physics_body* p_body) {
        p_output << YAML::Key << "Physics Body";

        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Linear Velocity" << YAML::Value
                 << p_body->linear_velocity;
        p_output << YAML::Key << "Angular Velocity" << YAML::Value
                 << p_body->angular_velocity;
        p_output << YAML::Key << "Cumulative Force" << YAML::Value
                 << p_body->cumulative_force;
        p_output << YAML::Key << "Cumulative Torque" << YAML::Value
                 << p_body->cumulative_torque;
        p_output << YAML::Key << "Mass Factor" << YAML::Value
                 << p_body->mass_factor;
        p_output << YAML::Key << "Center Mass Position" << YAML::Value
                 << p_body->center_mass_position;
        p_output << YAML::Key << "Gravity Enabled" << YAML::Value
                 << p_body->use_gravity;
        p_output << YAML::Key << "Gravity Factor" << YAML::Value
                 << p_body->gravity_factor;
        p_output << YAML::Key << "Body Type" << YAML::Value
                 << p_body->body_type;
        p_output << YAML::Key << "Friction" << YAML::Value << p_body->friction;
        p_output << YAML::Key << "Restitution" << YAML::Value
                 << p_body->restitution;
        p_output << YAML::Key << "Body Movement Type" << YAML::Value
                 << p_body->body_movement_type;
        p_output << YAML::Key << "Body Layer Type" << YAML::Value
                 << p_body->body_layer_type;
        p_output << YAML::Key << "Body ID" << YAML::Value << p_body->body_id;
        p_output << YAML::Key << "Count" << YAML::Value << p_body->count;
        p_output << YAML::EndMap;
        return p_output;
    }
};