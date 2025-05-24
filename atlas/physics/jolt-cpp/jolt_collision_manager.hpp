#pragma once


#include <cstdint>
#include <physics/jolt-cpp/jolt_collision.hpp>
namespace atlas::physics {

    class jolt_collision_manager {
    public:
        jolt_collision_manager() = default;
        static scope<jolt_collision_manager> initialize(const std::string& p_tag);
        void run_collision_added(uint64_t p_id, jolt::contact_event& p_event);
        // run_collision_persisted(uint64_t id);
        // run_collision_persisted(uint64_t id);
        static void subscribe_action(uint64_t p_id, jolt_collision& p_collision_action);
    private:
        static std::unordered_map<uint64_t, jolt_collision> m_handlers;
        static std::string m_tag;
    };
};