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

    struct environment {
        std::string filepath="";
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

    /**
     * @brief static is represented as fixed
     */
    enum body_type : uint8_t {
        fixed = 0,
        kinematic = 1,
        dynamic = 2,
        bodynum,
    };

    /**
     * @param non_moving is used for static objects that saves for not using the
     * collider component
     * @param moving is used for dynamic, kinematic, and character objects that
     * will be used
     */
    enum body_layer : uint8_t {
        non_moving = 0,
        moving = 1,
        layer_num,
    };

    enum activation : uint8_t { activate, deactivate };

    /**
     * @brief physics body data-driven representative
     *
     * TODO: Add parameters for force, impulse, and torque
     */
    struct physics_body {
        glm::vec3 linear_velocity = glm::vec3(0.0);
        glm::vec3 angular_velocity = glm::vec3(0.0f);

        glm::vec3 force = glm::vec3(0.0f);
        glm::vec3 impulse = glm::vec3(0.0f);
        glm::vec3 torque = glm::vec3(0.0f);

        float mass_factor = 1.0f;
        glm::vec3 center_mass_position = glm::vec3(0.0f);
        float linear_damping = 0.0f;
        float angular_damping = 0.0f;

        float gravity_factor = 1.0f;
        float friction = 0.8f;
        float restitution = 0.2f;

        //! @brief body_type::fixed means this physics body is static
        uint8_t body_movement_type = body_type::fixed;

        //! @brief body_layer (object layers) refer to the rules of the
        //! collision system specified in JoltPhysics
        uint8_t body_layer_type = body_layer::moving;
    };

    struct box_collider {
        glm::vec3 half_extent = glm::vec3(0.5f);
    };

    struct capsule_collider {
        float radius = 0.5f;
        float half_height = 0.5f;
    };

    struct sphere_collider {
        float radius = 0.5f;
    };

}; // namespace atlas