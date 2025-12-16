#include <core/engine_logger.hpp>
#include <core/scene/world.hpp>
#include <core/system/registry.hpp>

namespace atlas {
    system_registry* system_registry::s_instance = nullptr;

    system_registry::system_registry(const std::string& p_tag)
      : m_tag(p_tag) {
        s_instance = this;
    }

    system_registry::~system_registry() = default;

    ref<world> system_registry::create_world(const std::string& p_tag) {
        return s_instance->append_world_and_get(create_ref<world>(p_tag));
    }

    ref<world> system_registry::get_world(const std::string& p_tag) {
        return s_instance->search_world(p_tag);
    }

    ref<world> system_registry::search_world(const std::string& p_tag) {
        return m_world_registered[p_tag];
    }

    ref<world> system_registry::append_world_and_get(
      const ref<world>& p_world) {
        m_world_registered.emplace(p_world->name(), p_world);
        return m_world_registered[p_world->name()];
    }
};