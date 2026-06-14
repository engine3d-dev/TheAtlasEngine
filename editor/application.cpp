#include <utility>
#include <memory_resource>
#include <memory>

import atlas.application;
import atlas.core.utilities;
import atlas.core.level_streamer;
import atlas.drivers.vulkan;
import atlas.core.event;
import editor;

class editor_application : public atlas::application {
public:
    editor_application(/*NOLINT*/std::shared_ptr<atlas::graphics_context> p_context,
                       const atlas::application_settings& p_settings,
                       atlas::event::bus& p_bus)
      : atlas::application(p_context, p_settings, p_bus) {

        m_world =
          std::make_shared<editor_world>(p_context, "Editor World", p_bus, m_stream);

        current_world(m_world);
    }


    // ~editor_application() {
    //     m_world->destruct();
    // }
    ~editor_application() = default;

private:
    std::shared_ptr<editor_world> m_world;
    atlas::level_streamer m_stream;
};

atlas::ref<atlas::application>
initialize_application(/*NOLINT*/std::shared_ptr<atlas::graphics_context> p_context, atlas::event::bus& p_bus) {
    atlas::application_settings settings = {
        .name = "Editor",
        .width = 1510,
        .height = 877,
        .background_color = { 0.f, 0.f, 0.f, 0.f },
    };
    return create_ref<editor_application>(p_context, settings, p_bus);
}