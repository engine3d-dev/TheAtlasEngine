#include <core/application.hpp>
#include "editor_world.hpp"

class editor_application : public atlas::application {
public:
    editor_application(const atlas::application_settings& p_settings)
      : application(p_settings) {
        m_world = atlas::create_ref<editor_world>("Editor World");
    }

private:
    atlas::ref<editor_world> m_world;
};

namespace atlas {
    ref<application> initialize_application() {
        application_settings settings = {
            .name = "Editor",
            .width = 1200,
            .height = 800,
        };

        return create_ref<editor_application>(settings);
    }

} // end of namespace atlas