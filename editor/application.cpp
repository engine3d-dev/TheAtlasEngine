import atlas.application;
import atlas.common;

import editor_world;

import atlas.drivers.graphics_context;

class test_application : public atlas::application {
public:
    test_application(atlas::ref<atlas::graphics_context> p_context,
                     const atlas::application_settings& p_settings)
      : atlas::application(p_context, p_settings) {

        // TODO -- this is going to be changed with the use of the level
        // streamer API
        // Passing in the renderer instance to be able to render according to the current scene
        // Based the current scene. We render the entities within that particular scene.
        m_world = atlas::create_ref<editor_world>("Editor World", renderer_instance());


        current_world(m_world);
    }

private:
    atlas::ref<editor_world> m_world;
};

atlas::ref<atlas::application>
initialize_application(atlas::ref<atlas::graphics_context> p_contetxt) {
    atlas::application_settings settings = {
        .name = "Editor",
        .width = 1510,
        .height = 877,
        .background_color = { 0.f, 0.f, 0.f, 0.f },
    };
    return create_ref<test_application>(p_contetxt, settings);
}