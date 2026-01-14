import atlas.application;
import atlas.common;

import editor_world;
import atlas.core.scene.system_registry;

class test_application : public atlas::application {
public:
    test_application(const atlas::application_settings& p_settings) : atlas::application(p_settings) {
        // atlas::register_update(this, &test_application::on_update);std::pmr::monotonic_buffer_resource resource{ 4096 };
        // m_allocator.construct(&resource);

        // TODO -- this is going to be changed with the use of the level
        // streamer API
        m_world = atlas::create_ref<editor_world>("Editor World", renderer_instance());
    }


private:
    // std::pmr::polymorphic_allocator<uint8_t> m_allocator;
    atlas::ref<editor_world> m_world;

};

atlas::ref<atlas::application> initialize_application() {
    atlas::application_settings settings = {
        .name = "Editor",
        .width = 1200,
        .height = 800,
        .background_color = { 0.f, 0.f, 0.f, 0.f },
    };
    return create_ref<test_application>(settings);
}