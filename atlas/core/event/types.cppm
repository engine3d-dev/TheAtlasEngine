module;

#include <cstdint>
#include <string_view>

export module atlas.core.event:types;

export namespace atlas::event {

    struct collision_enter {
        uint64_t entity1;
        uint64_t entity2;
    };

    struct collision_persisted {
        uint64_t entity1;
        uint64_t entity2;
    };

    struct collision_exit {
        uint64_t entity1;
        uint64_t entity2;
    };

    /**
     * @brief event to trigger when to transition specific scenes
     *
     * @param from_scene is the current scene that is being transitioned from
     * @param to_scene is the new scene that is going to be transitioned to
     */
    struct scene_transition {
        const void* from_scene;
        const void* to_scene;
    };

    /**
    * @param entity_id is the entity to reload the mesh for
    * @param filename is the filename to reload the mesh into
    */
    struct mesh_reload {
        uint64_t entity_id;
        std::string_view filename;
    };
};