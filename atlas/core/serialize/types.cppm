module;

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

export module atlas.core.serialize.types;
import atlas.core.scene.components;

namespace YAML {
    /**
     * @brief Custom specialized classes from yaml-cpp to specialize to take in
     * a glm::vec2 specifically
     *
     * Specialization is used for converting glm::vec2 to yaml-cpp syntax when
     * saving glm::vec2 data to disk
     */
    template<>
    struct convert<glm::vec2> {
        static Node encode(const glm::vec2& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);

            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs) {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();

            return true;
        }
    };

    /**
     * @brief Custom specialized classes from yaml-cpp to specialize to take in
     * a glm::vec3 specifically
     *
     * Specialization is used for converting glm::vec3 to yaml-cpp syntax when
     * saving glm::vec3 data to disk
     */
    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);

            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs) {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();

            return true;
        }
    };

    /**
     * @brief Custom specialized classes from yaml-cpp to specialize to take in
     * a glm::vec4 specifically
     *
     * Specialization is used for converting glm::vec4 to yaml-cpp syntax when
     * saving glm::vec4 data to disk
     */
    template<>
    struct convert<glm::vec4> {

        static Node encode(const glm::vec4& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);

            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs) {
            if (!node.IsSequence() || node.size() != 4) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template<>
    struct convert<atlas::mesh_source> {

        static Node encode(const atlas::mesh_source& rhs) {
            Node node;
            // Encode glm::vec3 members
            node["Model"] = rhs.model_path;
            node["Color"] = rhs.color;
            node["Diffuse"] = rhs.diffuse;
            node["Specular"] = rhs.specular;
            return node;
        }

        static bool decode(const Node& node, atlas::mesh_source& rhs) {
            if (!node.IsMap()) {
                return false;
            }

            rhs.model_path = node["Model"].as<std::string>();
            rhs.color = node["Angular Velocity"].as<glm::vec4>();
            rhs.diffuse = node["Force"].as<std::string>();
            rhs.specular = node["Impulse"].as<std::string>();
            return true;
        }
    };

    template<>
    struct convert<atlas::point_light> {

        static Node encode(const atlas::point_light& rhs) {
            Node node;
            // Encode glm::vec3 members
            node["Color"] = rhs.color;
            node["Attenuation"] = rhs.attenuation;
            node["Ambient"] = rhs.ambient;
            node["Diffuse"] = rhs.diffuse;
            node["Specular"] = rhs.specular;
            return node;
        }

        static bool decode(const Node& node, atlas::point_light& rhs) {
            if (!node.IsMap()) {
                return false;
            }

            rhs.color = node["Color"].as<glm::vec4>();
            rhs.attenuation = node["Attenuation"].as<float>();
            rhs.ambient = node["Ambient"].as<glm::vec4>();
            rhs.diffuse = node["Diffuse"].as<glm::vec4>();
            rhs.specular = node["Specular"].as<glm::vec4>();
            return true;
        }
    };

    /**
     * @brief Custom specialized classes from yaml-cpp to specialize to take in
     * a atlas::body_type specifically
     *
     * Specialization is used for converting atlas::body_type to yaml-cpp syntax
     * when saving atlas::body_type data to disk
     */
    template<>
    struct convert<atlas::body_type> {
        static Node encode(const atlas::body_type& rhs) {
            Node node;
            node.push_back(static_cast<uint8_t>(rhs));
            return node;
        }

        static bool decode(const Node& node, atlas::body_type& rhs) {
            if (!node.IsScalar()) {
                return false;
            }
            rhs = static_cast<atlas::body_type>(node.as<uint8_t>());
            return true;
        }
    };

    /**
     * @brief Custom specialized classes from yaml-cpp to specialize to take in
     * a atlas::body_layer specifically
     *
     * Specialization is used for converting atlas::body_layer to yaml-cpp
     * syntax when saving atlas::body_layer data to disk
     */
    template<>
    struct convert<atlas::body_layer> {
        static Node encode(const atlas::body_layer& rhs) {
            Node node;
            node.push_back(static_cast<uint8_t>(rhs));
            return node;
        }

        static bool decode(const Node& node, atlas::body_layer& rhs) {
            if (!node.IsScalar()) {
                return false;
            }
            rhs = static_cast<atlas::body_layer>(node.as<uint8_t>());
            return true;
        }
    };

    /**
     * @brief Custom specialized classes from yaml-cpp to specialize to take in
     * a atlas::physics_body specifically
     *
     * Specialization is used for converting atlas::physics_body to yaml-cpp
     * syntax when saving atlas::physics_body data to disk
     */
    template<>
    struct convert<atlas::physics_body> {

        static Node encode(const atlas::physics_body& rhs) {
            Node node;
            // Encode glm::vec3 members
            node["Linear Velocity"] = rhs.linear_velocity;
            node["Angular Velocity"] = rhs.angular_velocity;
            node["Force"] = rhs.force;
            node["Impulse"] = rhs.impulse;
            node["Torque"] = rhs.torque;
            node["Mass Factor"] = rhs.mass_factor;
            node["Center Mass Position"] = rhs.center_mass_position;
            // node["Gravity Enabled"] = rhs.use_gravity;
            node["Gravity Factor"] = rhs.gravity_factor;
            // node["Body Type"] = rhs.body_type;
            node["Friction"] = rhs.friction;
            node["Restitution"] = rhs.restitution;
            node["Body Movement Type"] = rhs.body_movement_type;
            node["Body Layer Type"] = rhs.body_layer_type;

            return node;
        }

        static bool decode(const Node& node, atlas::physics_body& rhs) {
            if (!node.IsMap()) {
                return false;
            }

            rhs.linear_velocity = node["Linear Velocity"].as<glm::vec3>();
            rhs.angular_velocity = node["Angular Velocity"].as<glm::vec3>();
            rhs.force = node["Force"].as<glm::vec3>();
            rhs.impulse = node["Impulse"].as<glm::vec3>();
            rhs.torque = node["Torque"].as<glm::vec3>();
            rhs.mass_factor = node["Mass Factor"].as<float>();
            rhs.center_mass_position =
              node["Center Mass Position"].as<glm::vec3>();
            rhs.gravity_factor = node["Gravity Factor"].as<float>();
            rhs.friction = node["Friction"].as<float>();
            rhs.restitution = node["Restitution"].as<float>();
            rhs.body_movement_type = node["Body Movement Type"].as<uint8_t>();
            rhs.body_layer_type = node["Body Layer Type"].as<uint8_t>();
            return true;
        }
    };

    /**
     * @brief Custom specialized classes from yaml-cpp to specialize to take in
     * a atlas::box_collider specifically
     *
     * Specialization is used for converting atlas::box_collider to yaml-cpp
     * syntax when saving atlas::box_collider data to disk
     */
    template<>
    struct convert<atlas::box_collider> {

        static Node encode(const atlas::box_collider& rhs) {
            Node node;
            // Encode glm::vec3 members
            node["Half Extent"] = rhs.half_extent;

            return node;
        }

        static bool decode(const Node& node, atlas::box_collider& rhs) {
            if (!node.IsMap()) {
                return false;
            }

            rhs.half_extent = node["half Extent"].as<glm::vec3>();
            return true;
        }
    };

    /**
     * @brief Custom specialized classes from yaml-cpp to specialize to take in
     * a atlas::sphere_collider specifically
     *
     * Specialization is used for converting atlas::sphere_collider to yaml-cpp
     * syntax when saving atlas::sphere_collider data to disk
     */
    template<>
    struct convert<atlas::sphere_collider> {

        static Node encode(const atlas::sphere_collider& rhs) {
            Node node;
            // Encode glm::vec3 members
            node["Radius"] = rhs.radius;

            return node;
        }

        static bool decode(const Node& node, atlas::sphere_collider& rhs) {
            if (!node.IsMap()) {
                return false;
            }

            rhs.radius = node["Radius"].as<float>();
            return true;
        }
    };

    /**
     * @brief Custom specialized classes from yaml-cpp to specialize to take in
     * a atlas::capsule_collider specifically
     *
     * Specialization is used for converting atlas::capsule_collider to yaml-cpp
     * syntax when saving atlas::capsule_collider data to disk
     */
    template<>
    struct convert<atlas::capsule_collider> {

        static Node encode(const atlas::capsule_collider& rhs) {
            Node node;
            // Encode glm::vec3 members
            node["Half Height"] = rhs.half_height;
            node["Radius"] = rhs.radius;

            return node;
        }

        static bool decode(const Node& node, atlas::capsule_collider& rhs) {
            if (!node.IsMap()) {
                return false;
            }

            rhs.radius = node["Radius"].as<float>();
            rhs.half_height = node["Half height"].as<float>();
            return true;
        }
    };
};

export namespace atlas {
    //! @brief from yaml-cpp, saving glm::highp_vec2 values to disk
    YAML::Emitter& operator<<(YAML::Emitter& p_out,
                              const glm::highp_vec2& p_values) {
        p_out << YAML::Flow;
        p_out << YAML::BeginSeq << p_values.x << p_values.y << YAML::EndSeq;
        return p_out;
    }

    //! @brief from yaml-cpp, saving glm::highp_vec3 values to disk
    YAML::Emitter& operator<<(YAML::Emitter& p_out,
                              const glm::highp_vec3& p_values) {
        p_out << YAML::Flow;
        p_out << YAML::BeginSeq << p_values.x << p_values.y << p_values.z
              << YAML::EndSeq;
        return p_out;
    }

    //! @brief from yaml-cpp, saving glm::highp_vec4 values to disk
    YAML::Emitter& operator<<(YAML::Emitter& p_out,
                              const glm::highp_vec4& p_values) {
        p_out << YAML::Flow;
        p_out << YAML::BeginSeq << p_values.x << p_values.y << p_values.z
              << p_values.w << YAML::EndSeq;
        return p_out;
    }

    //! @brief from yaml-cpp, saving transform values to disk
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

    //! @brief from yaml-cpp, saving perspective_camera values to disk
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

    //! @brief from yaml-cpp, saving mesh_source values to disk
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

    //! @brief from yaml-cpp, saving mesh_source values to disk
    YAML::Emitter& operator<<(YAML::Emitter& p_output,
                              const point_light* p_material) {
        p_output << YAML::Key << "Point Light";

        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Color" << YAML::Value << p_material->color;
        p_output << YAML::Key << "Attenuation" << YAML::Value
                 << p_material->attenuation;
        p_output << YAML::Key << "Ambient" << YAML::Value
                 << p_material->ambient;
        p_output << YAML::Key << "Diffuse" << YAML::Value
                 << p_material->diffuse;
        p_output << YAML::Key << "Specular" << YAML::Value
                 << p_material->specular;
        p_output << YAML::EndMap;
        return p_output;
    }

    //! @brief from yaml-cpp, saving physics_body values to disk
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

    //! @brief from yaml-cpp, saving box_collider values to disk
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

    //! @brief from yaml-cpp, saving sphere_collider values to disk
    YAML::Emitter& operator<<(YAML::Emitter& p_output,
                              const sphere_collider* p_body) {
        p_output << YAML::Key << "Sphere Collider";

        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Radius" << YAML::Value << p_body->radius;

        p_output << YAML::EndMap;

        return p_output;
    }

    //! @brief from yaml-cpp, saving capsule_collider values to disk
    YAML::Emitter& operator<<(YAML::Emitter& p_output,
                              const capsule_collider* p_body) {
        p_output << YAML::Key << "Capsule Collider";
        p_output << YAML::BeginMap;
        p_output << YAML::Key << "Radius" << YAML::Value << p_body->radius;
        p_output << YAML::Key << "Half Height" << YAML::Value
                 << p_body->half_height;
        p_output << YAML::EndMap;

        return p_output;
    }
};