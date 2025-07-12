#pragma once
#include <flecs.h>
#include <core/scene/components.hpp>

namespace atlas {
    //! @brief camera system --- systematic way to manage per-scene cameras
    //! @brief System does the querying of camera-related entities that can then
    //! be applied to this system for keeping track
    // Used for synchronizing the changes of the camera system that is relative
    // to the scene they are apart of
    // Reminder: This is going to be replaced with using flecs::system instead
    // to handle action and state-related operations.
    class camera_system {
    public:
        camera_system() = default;

        camera_system(const float p_aspect_ratio,
                      const flecs::world& p_registry);

        void query_camera_entities();

    private:
        [[maybe_unused]] float m_aspect_ratio;
        flecs::world m_registry;
        flecs::query<projection_view, transform, perspective_camera> m_query;
    };
};