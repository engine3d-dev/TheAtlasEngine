#include <core/scene/scene_object.hpp>
#include <core/scene/components.hpp>

namespace atlas {
    scene_object::scene_object(flecs::world* p_registry,
                               const std::string& p_name) {
        m_entity = p_registry->entity(p_name.c_str());
        add<transform>();
    }

    scene_object::scene_object(flecs::world* p_registry,
                               const std::string& p_name,
                               [[maybe_unused]] bool p_lookup) {
        m_entity = p_registry->lookup(p_name.c_str());
        add<transform>();
    }

    scene_object::scene_object(strong_ref<flecs::world>& p_registry,
                               const std::string& p_name) {
        m_entity = p_registry->entity(p_name.c_str());
        add<transform>();
    }

    scene_object::~scene_object() {
        if (m_entity.is_alive()) {
            m_entity.destruct();
        }
    }
};