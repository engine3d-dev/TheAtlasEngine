#include <physics/jolt-cpp/jolt_collision_manager.hpp>

namespace atlas::physics {

    std::string jolt_collision_manager::m_tag = "Default_Tag";
    std::unordered_map<uint64_t, jolt_collision>
      jolt_collision_manager::m_handlers;

    void jolt_collision_manager::subscribe_action(uint64_t id,
                                                  jolt_collision& handler) {
        m_handlers[id] = handler;
    }

    scope<jolt_collision_manager> jolt_collision_manager::initialize(
      const std::string& p_tag) {
        m_tag = p_tag;
        return create_scope<jolt_collision_manager>();
    }

    void jolt_collision_manager::run_collision_added(
      uint64_t p_id,
      jolt::contact_event& p_event) {
        auto it = m_handlers.find(p_id);
        if (it == m_handlers.end()) {
            console_log_error("Key {} does not exist!\n", p_id);
            return;
        }

        it->second.collision_added(p_event);
    }

}
