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
                              const mesh_source* p_material) {
        p_output << YAML::Key << "Mesh Source";

        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Model Path" << YAML::Value
                 << p_material->model_path;
        p_output << YAML::Key << "Diffuse" << YAML::Value
                 << p_material->diffuse;
        p_output << YAML::Key << "Specular" << YAML::Value
                 << p_material->specular;
        p_output << YAML::EndMap;
        return p_output;
    }

    YAML::Emitter& operator<<(YAML::Emitter& p_output, const point_light* p_material) {
        p_output << YAML::Key << "Point Light";

        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Color" << YAML::Value << p_material->color;
        p_output << YAML::Key << "Attenuation" << YAML::Value << p_material->attenuation;
        p_output << YAML::Key << "Ambient" << YAML::Value << p_material->ambient;
        p_output << YAML::Key << "Diffuse" << YAML::Value << p_material->diffuse;
        p_output << YAML::Key << "Specular" << YAML::Value << p_material->specular;
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
        p_output << YAML::Key << "Force" << YAML::Value << p_body->force;
        p_output << YAML::Key << "Impulse" << YAML::Value << p_body->impulse;
        p_output << YAML::Key << "Torque" << YAML::Value << p_body->torque;
        p_output << YAML::Key << "Mass Factor" << YAML::Value
                 << p_body->mass_factor;
        p_output << YAML::Key << "Center Mass Position" << YAML::Value
                 << p_body->center_mass_position;
        p_output << YAML::Key << "Friction" << YAML::Value << p_body->friction;
        p_output << YAML::Key << "Restitution" << YAML::Value
                 << p_body->restitution;
        p_output << YAML::Key << "Body Movement Type" << YAML::Value
                 << static_cast<int>(p_body->body_movement_type);
        p_output << YAML::Key << "Body Layer Type" << YAML::Value
                 << static_cast<int>(p_body->body_layer_type);
        p_output << YAML::EndMap;
        return p_output;
    }

    YAML::Emitter& operator<<(YAML::Emitter& p_output,
                              const box_collider* p_body) {
        // Tag this specific serialization values to the box collider
        p_output << YAML::Key << "Box Collider";

        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Half Extent" << YAML::Value
                 << p_body->half_extent;
        p_output << YAML::EndMap;

        return p_output;
    }

    YAML::Emitter& operator<<(YAML::Emitter& p_output,
                              const sphere_collider* p_body) {
        //
        p_output << YAML::Key << "Sphere Collider";

        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Radius" << YAML::Value << p_body->radius;

        p_output << YAML::EndMap;

        return p_output;
    }

    YAML::Emitter& operator<<(YAML::Emitter& p_output,
                              const capsule_collider* p_body) {
        //
        p_output << YAML::Key << "Capsule Collider";
        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Radius" << YAML::Value << p_body->radius;
        p_output << YAML::Key << "Half Height" << YAML::Value
                 << p_body->half_height;
        p_output << YAML::EndMap;

        return p_output;
    }
};