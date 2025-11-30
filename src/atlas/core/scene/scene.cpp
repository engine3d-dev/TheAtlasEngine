#include <core/scene/scene.hpp>

namespace atlas {
    scene::scene(const std::string& p_name, event::event_bus& p_bus) : m_name(p_name), m_bus(&p_bus) {}

    // strong_ref<scene_object> scene::create_object(const std::string& p_name) {
    //     return create_strong_ref<scene_object>(
    //         m_allocator, &m_registry, p_name);
    // }

    // custom_entity scene::create(const std::string& p_name) {
    //     flecs::entity e = m_registry.entity(p_name.c_str());
    //     return custom_entity(e);
    // }

    scene_object scene::create(const std::string& p_name) {
        return scene_object(m_registry.entity(p_name.c_str()));
    }

};