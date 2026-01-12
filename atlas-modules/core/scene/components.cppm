module;

#include <string>
#include <glm/glm.hpp>
#include <flecs.h>
#include <vector>


export module atlas.core.scene.components;

import atlas.core.math;

export namespace atlas {

    struct transform {
        glm::highp_vec3 position{ 0.f };
        glm::highp_vec4 quaternion{ 0.f, 0, 0, 1 };
        glm::highp_vec3 rotation{ 0.f };
        glm::highp_vec3 scale{ 1.f };

        //! @brief sets rotation and automatically converts rotation glm::vec3
        //! to quaternion
        void set_rotation(const glm::highp_vec3& p_value) {
            rotation = p_value;
            quaternion = from_quat(rotation);
        }
    };

    struct material_metadata {
        glm::vec4 ambient{ 0.2f };
        glm::vec4 diffuse{ 0.5f };
        glm::vec4 specular{ 1.f };
        float shininess = 0.5f;
    };
    struct directional_light {
        glm::vec3 direction{ 0.f };
        glm::vec3 view_position{ 0.f };
        // {x, y, z, w: intensity}
        glm::vec4 ambient{ 1.f };
        glm::vec4 diffuse{ 1.f };
        // {x, y, z, w: intensity}
        glm::vec4 specular{ 1.f, 1.f, 0.f, 0.f };
        glm::vec4 color{ 1.f };
    };

    struct point_light {
        glm::vec3 position; // this is provided by the transform
        glm::vec4 color = { 1.f, 1.f, 1.f, 1.f };
        float attenuation = 1.f;
        float constant = 1.f;
        float linear = 1.f;
        float quadratic = 1.f;

        glm::vec4 ambient = glm::vec4(1.f);
        glm::vec4 diffuse = glm::vec4(1.f);
        glm::vec4 specular = glm::vec4(1.f);
    };

    /**
     * @brief Loads a mesh source
     *
     * @param color is the albedo color values
     * @param model_path is the specified path to the 3d model asset path
     * @param texture_path is the specified path to the specific texture to load
     */
    struct mesh_source {
        bool flip = false; // this is for flipping the texture coordinates
        glm::vec4 color{ 1.f };
        std::string model_path = "";
        std::string diffuse = "";
        std::string specular = "";
    };

    /**
     * @brief define a game object to have a perspective camera that can
     * correspond to it
     */
    struct perspective_camera {
        // represented as {near: x, far: y}
        glm::vec2 plane{ 0.f };

        // Activate to be the current camera
        bool is_active = false;

        // Defaults to 45.0f in radians
        float field_of_view = glm::radians(45.f);
    };

    /**
     * @brief specialized namespace tag to use for specifying operations used
     * with flecs in using tags
     *
     * Tags can be handled for specifying queries for specific archetypes (list
     * of those entities)
     *
     * Example Usage:
     *
     * ```C++
     * m_entity.add<flecs::pair<tag::editor, atlas::transform>>();
     * ```
     */
    namespace tag {

        //! @brief to indicate which entities are editor-only
        struct editor {};

        //! @brief to tag entities to serialize through the serializer
        struct serialize {
            bool enable = false;
        };
    };

    //! TODO: Consider either relocating where this is and how it gets handled.

    /**
     * @brief struct for attaching a projection/view matrix to your game object
     *
     * This should only be used if you know your object is going to add
     * atlas::perspective_camera to work
     *
     * TODO: Consider adding this automatically in a way we can represent
     * multiple cameras in a given scene. Which is something to think about
     */
    struct projection_view {
        glm::mat4 projection;
        glm::mat4 view;
    };

}; // namespace atlas