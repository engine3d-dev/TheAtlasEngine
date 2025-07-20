#pragma once
#include <string>
#include <glm/glm.hpp>
#include <flecs.h>
#include <vector>

namespace atlas {

    struct transform {
        glm::highp_vec3 position{ 0.f };
        glm::highp_vec4 quaternion{ 0.f, 0, 0, 1 };
        glm::highp_vec3 rotation{ 0.f };
        glm::highp_vec3 scale{ 1.f };
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

    // TEMPORARY: Port is just a way to handle setting up main active cameras
    // Also use for texture camera (so minimap-like view)
    // This will be replaced with specifying rendertargets rather then the
    // screens and when we get viewport enabled screen = render to the screen
    enum viewport {
        none,
        screen, // rendered to the screen
    };

    //! @brief Component for setting up perspective camera
    struct perspective_camera {
        // glm::vec2 represented as {near: x, far: y}
        glm::vec2 plane{ 0.f };

        // Sets camera to be the main camera
        bool is_active = false;

        // Specify camera field of view
        // Defaults to 45.0f in radians
        float field_of_view = glm::radians(45.f);

        // rendertarget specifically for camera (TEMP -- need to implement
        // viewport management potentially)
        // viewport target = viewport::none;
    };

    // inside the namespace tag are specialized structs that may get used by
    // flecs::system to correspond different tags for specifying what the system
    // should do with those components.
    // Usage: add<flecs::pair<tag::editor, atlas::transform>>();
    namespace tag {
        //  specialized struct for specifying objects which are
        //  editorial-specific objects
        struct editor {};
    };

    //! TODO: Might need this to be relocated. Not sure currently, locating
    //! it here in components.hpp
    struct projection_view {
        glm::mat4 projection;
        glm::mat4 view;
    };

}; // namespace atlas