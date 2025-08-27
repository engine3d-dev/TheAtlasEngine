#pragma once
#include <string>
#include <glm/glm.hpp>
#include <flecs.h>
#include <vector>
#include <core/math/utilities.hpp>

namespace atlas {

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

    //! @note Our interpretation of the RigidBody3D
    struct rigidbody3d {
        rigidbody3d() = default;

        enum class body_type { e_static = 0, e_dynamic = 1, e_kinematic = 2 };

        body_type type = body_type::e_static;
        bool hax_fixation = false;
        void* body_at_runtime =
          nullptr; // for storing the actual data of the body
    };

    //! @note Our interpretation of the BoxCollider that will get interpreted
    //! based on physics API's enabled
    struct box_collider3d {
        box_collider3d() = default;
        box_collider3d(const box_collider3d&) = default;

        float density = 1.0f;
        float friction = 0.5f;
        float restitution = 0.0f;
        float restitutionThreshold = 0.5f;

        glm::vec3 offset = { 0.f, 0.f, 0.f };
        glm::vec3 size = { 0.5f, 0.5f, 0.5f };
    };

    //! TODO: Make this better (when we do lighting)
    struct light {
        glm::vec3 Position{ 1.f };
    };

    /**
     * @brief Actually might do a query for this along with rendertarget3d
     * @brief This is because we can have multiple things that could be reloaded
     * @brief Such as the single texture and the mesh .obj 3d model as well
     * @brief
     */
    struct reload {
        bool on_reload = false;
        bool on_texture_reload = false;
    };

    /**
     * @brief material component for specifying a specific type of material for
     * a scene object
     */
    struct material {
        glm::vec4 color{ 1.f };
        std::string model_path = "";
        std::string texture_path =
          ""; // This just contains the path to load the texture
        std::vector<std::string> texture_filepaths;
        bool model_reload = false;
        bool texture_reload = false;
    };

    struct perspective_camera {
        // glm::vec2 represented as {near: x, far: y}
        glm::vec2 plane{ 0.f };

        // Sets camera to be the main camera
        bool is_active = false;

        // Specify camera field of view
        // Defaults to 45.0f in radians
        float field_of_view = glm::radians(45.f);
    };

    //! @brief specialized namespace tag to use for specifying operation with
    //! either flecs::system or tags to handle specific querying of entities
    // Example Usage: add<flecs::pair<tag::editor, atlas::transform>>();
    namespace tag {
        struct editor {};
    };

    //! TODO: Consider either relocating where this is and how it gets handled.
    struct projection_view {
        glm::mat4 projection;
        glm::mat4 view;
    };

}; // namespace atlas