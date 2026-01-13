module;

#include <cstdint>

export module atlas.core.event.types;

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
};