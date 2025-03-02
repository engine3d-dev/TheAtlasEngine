#pragma once
#include <glm/glm.hpp>

namespace atlas {

    struct camera_ubo {
        glm::mat4 Projection;
        glm::mat4 View;
    };

    /*
    Each Uniform object will have a descriptor set associated with them
    - Per descriptor set contains the following
        - DescriptorWrite
        - DescriptorRead
        - DescriptorLayout

    - Per vertex (In Shader)
        - VertexAttribute
        - VertexAttributeLayout = {ShaderFloat3, "aPos"}
    */
    struct PointLightUbo {
        glm::vec3 Position;

        float Constant;
        float Linear;
        float Quadratic;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    struct DirectionalLightUbo {
        glm::vec3 Direction = { -0.2f, -1.0f, -0.3f };

        glm::vec3 ambient = { 0.05f, 0.05f, 0.05f };
        glm::vec3 diffuse = { 0.4f, 0.4f, 0.4f };
        glm::vec3 specular = { 0.5f, 0.5f, 0.5f };
    };

    struct SpotLightUbo {
        glm::vec3 Position;
        glm::vec3 Direction{ 1.f, 0.f, 0.f };
        float cut_off;
        float outer_cut_off;

        float constant{ 1.f };
        float linear{ 0.09f };
        float quadratic{ 0.032f };

        glm::vec3 ambient{ 0.0f, 0.0f, 0.0f };
        glm::vec3 diffuse{ 1.0f, 1.0f, 1.0f };
        glm::vec3 specular{ 1.0f, 1.0f, 1.0f };
    };
};