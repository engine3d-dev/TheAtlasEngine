#pragma once
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include <core/scene/components.hpp>
#include <physics/components.hpp>

namespace YAML {
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

    // template<>
    // struct convert<atlas::collider_shape> {
    //     static Node encode(const atlas::collider_shape& rhs) {
    //         Node node;
    //         node.push_back(static_cast<uint8_t>(rhs));
    //         return node;
    //     }

    //     static bool decode(const Node& node,
    //                        atlas::collider_shape& rhs) {
    //         if (!node.IsScalar()) {
    //             return false;
    //         }
    //         rhs =
    //           static_cast<atlas::collider_shape>(node.as<uint8_t>());
    //         return true;
    //     }
    // };

    // Specialization for body_type enum
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

    // Specialization for body_layer enum
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

    template<>
    struct convert<atlas::physics_body> {

        static Node encode(const atlas::physics_body& rhs) {
            Node node;
            // Encode glm::vec3 members
            node["Linear Velocity"] = rhs.linear_velocity;
            node["Angular Velocity"] = rhs.angular_velocity;
            node["Cumulative Force"] = rhs.cumulative_force;
            node["Cumulative Torque"] = rhs.cumulative_torque;
            node["Mass Factor"] = rhs.mass_factor;
            node["Center Mass Position"] = rhs.center_mass_position;
            node["Gravity Enabled"] = rhs.use_gravity;
            node["Gravity Factor"] = rhs.gravity_factor;
            node["Body Type"] = rhs.body_type;
            node["Friction"] = rhs.friction;
            node["Restitution"] = rhs.restitution;
            node["Body Movement Type"] = rhs.body_movement_type;
            node["Body Layer Type"] = rhs.body_layer_type;
            node["Body ID"] = rhs.body_id;
            node["Count"] = rhs.count;

            return node;
        }

        static bool decode(const Node& node,
                           atlas::physics_body& rhs) {
            if (!node.IsMap()) {
                return false;
            }

            rhs.linear_velocity = node["Linear Velocity"].as<glm::vec3>();
            rhs.angular_velocity = node["Angular Velocity"].as<glm::vec3>();
            rhs.cumulative_force = node["Cumulative Force"].as<glm::vec3>();
            rhs.cumulative_torque = node["Cumulative Torque"].as<glm::vec3>();
            rhs.mass_factor = node["Mass Factor"].as<float>();
            rhs.center_mass_position =
              node["Center Mass Position"].as<glm::vec3>();
            rhs.use_gravity = node["Gravity Enabled"].as<bool>();
            rhs.gravity_factor = node["Gravity Factor"].as<float>();
            rhs.body_type = node["Body Type"].as<uint8_t>();
            rhs.friction = node["Friction"].as<float>();
            rhs.restitution = node["Restitution"].as<float>();
            rhs.body_movement_type = node["Body Movement Type"].as<uint8_t>();
            rhs.body_layer_type = node["Body Layer Type"].as<uint8_t>();
            rhs.body_id = node["Body ID"].as<uint32_t>();
            rhs.count = node["Count"].as<int>();
            return true;
        }
    };
};

namespace atlas {
    YAML::Emitter& operator<<(YAML::Emitter& p_out,
                              const glm::highp_vec2& p_values);

    YAML::Emitter& operator<<(YAML::Emitter& p_out,
                              const glm::highp_vec3& p_values);

    YAML::Emitter& operator<<(YAML::Emitter& p_out,
                              const glm::highp_vec4& p_values);

    YAML::Emitter& operator<<(YAML::Emitter& p_output,
                              const transform* p_transform);

    YAML::Emitter& operator<<(YAML::Emitter& p_output,
                              const perspective_camera* p_camera);

    YAML::Emitter& operator<<(YAML::Emitter& p_output,
                              const material* p_material);

    // physics-based components
    // YAML::Emitter& operator<<(YAML::Emitter& p_output,
    //                           const collider_body* p_collider);

    YAML::Emitter& operator<<(YAML::Emitter& p_output,
                              const physics_body* p_body);
};