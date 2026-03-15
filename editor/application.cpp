#include <utility>
#include <memory_resource>
#include <memory>

import atlas.application;
import atlas.common;
import atlas.core.level_streamer;
import atlas.core.event;
import editor_world;

import atlas.drivers.graphics_context;

class editor_application : public atlas::application {
public:
    editor_application(atlas::ref<atlas::graphics_context> p_context,
                       const atlas::application_settings& p_settings,
                       atlas::event::bus& p_bus)
      : atlas::application(std::move(p_context), p_settings, p_bus) {

        m_world =
          atlas::create_ref<editor_world>("Editor World", p_bus, m_stream);

        current_world(m_world);
    }

private:
    atlas::ref<editor_world> m_world;
    atlas::level_streamer m_stream;
};

atlas::ref<atlas::application>
initialize_application(
  /*NOLINT*/ atlas::ref<atlas::graphics_context> p_contetxt,
  atlas::event::bus& p_bus) {
    atlas::application_settings settings = {
        .name = "Editor",
        .width = 1510,
        .height = 877,
        .background_color = { 0.f, 0.f, 0.f, 0.f },
    };
    return create_ref<editor_application>(p_contetxt, settings, p_bus);
}