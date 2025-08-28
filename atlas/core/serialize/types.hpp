#pragma once
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include <core/scene/components.hpp>

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
};

namespace atlas {
    YAML::Emitter& operator<<(YAML::Emitter& p_out, const glm::highp_vec2& p_values);

    YAML::Emitter& operator<<(YAML::Emitter& p_out, const glm::highp_vec3& p_values);

    YAML::Emitter& operator<<(YAML::Emitter& p_out, const glm::highp_vec4& p_values);

    YAML::Emitter& operator<<(YAML::Emitter& p_output, const transform* p_transform);

    YAML::Emitter& operator<<(YAML::Emitter& p_output,  const perspective_camera* p_camera);

    YAML::Emitter& operator<<(YAML::Emitter& p_output,  const material* p_material);
};