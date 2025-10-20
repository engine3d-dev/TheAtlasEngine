#pragma once
#include <cstdint>

namespace atlas::event {

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
};