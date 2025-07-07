#include <core/scene/scene_object.hpp>
#include <core/scene/components.hpp>

namespace atlas {
    scene_object::scene_object(flecs::world* p_registry,
                               const std::string& p_name) {
        m_entity = p_registry->entity(p_name.c_str());
        add<transform>();
    }
};