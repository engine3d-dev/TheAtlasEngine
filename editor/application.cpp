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
                     const atlas::application_settings& p_settings)
      : atlas::application(std::move(p_context), p_settings) {

        std::array<uint8_t, 1024> byte{};
        std::pmr::monotonic_buffer_resource resource{byte.data(), byte.size()};
        m_allocator.construct(&resource);
        m_stream = std::allocate_shared<atlas::level_streamer>(m_allocator, m_bus);

        // TODO -- this is going to be changed with the use of the level
        // streamer API
        // Passing in the renderer instance to be able to render according to the current scene
        // Based the current scene. We render the entities within that particular scene.
        m_world = atlas::create_ref<editor_world>("Editor World", m_bus, renderer_instance(), m_stream);


        current_world(m_world);
    }

private:
    atlas::event::bus m_bus;
    atlas::ref<editor_world> m_world;
    std::pmr::polymorphic_allocator<atlas::level_streamer> m_allocator;
    atlas::ref<atlas::level_streamer> m_stream;
};

atlas::ref<atlas::application>
initialize_application(/*NOLINT*/atlas::ref<atlas::graphics_context> p_contetxt) {
    atlas::application_settings settings = {
        .name = "Editor",
        .width = 1510,
        .height = 877,
        .background_color = { 0.f, 0.f, 0.f, 0.f },
    };
    return create_ref<test_application>(p_contetxt, settings);
}