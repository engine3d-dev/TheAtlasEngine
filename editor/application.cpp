#include <core/application.hpp>
#include "editor_world.hpp"
#include <core/core.hpp>
#include <core/common.hpp>

/**
 * This represents our editor's application
 *
 * TODO: Add a preload API to preload the application. For providing user
 * preferences, project settings, or other configuration settings that are
 * projects-related
 */
class editor_application : public atlas::application {
public:
    editor_application(const atlas::application_settings& p_settings)
      : application(p_settings) {
        std::pmr::monotonic_buffer_resource resource{ 4096 };
        m_allocator.construct(&resource);

        // TODO -- this is going to be changed with the use of the level
        // streamer API
        m_world =
          atlas::create_strong_ref<editor_world>(m_allocator, "Editor World");
    }

private:
    std::pmr::polymorphic_allocator<uint8_t> m_allocator;
    atlas::optional_ref<editor_world> m_world;
};

namespace atlas {
    ref<application> initialize_application() {
        application_settings settings = {
            .name = "Editor",
            .width = 1200,
            .height = 800,
            .background_color = { 0.f, 0.f, 0.f, 0.f },
        };

        return create_ref<editor_application>(settings);
    }

} // end of namespace atlas