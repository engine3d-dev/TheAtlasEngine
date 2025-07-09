#pragma once
#include <core/core.hpp>
#include <core/engine_logger.hpp>
#include <core/scene/scene_object.hpp>
#include <string>

namespace atlas {

    /**
     * @brief scene for defining part of a world_scope a group of scene object
     * correspond to that scene
     *
     * scenes are chunks where scene objects are contained but managed lifetimes
     * by world_scope. This is to differentiate different areas of scene objects
     * and where they are located within the scope of the world
     */
    class scene_scope {
    public:
        scene_scope() = default;
        scene_scope(const std::string& p_tag)
          : m_tag(p_tag) {}

        strong_ref<scene_object> create_object(const std::string& p_tag) {
            return create_strong_ref<scene_object>(
              m_object_allocator, &m_registry, p_tag);
        }

        template<typename T>
        strong_ref<scene_object> create_custom_object(
          const std::string& p_tag) {
            static_assert(std::is_base_of_v<scene_scope, T>,
                          "invalid scene_object not inherited with base class "
                          "being scene_object");
            return create_strong_ref<T>(m_object_allocator, p_tag);
        }

        template<typename... Comps, typename... Args>
        flecs::query_builder<Comps...> query_builder(Args&&... args) const {
            return flecs::query_builder<Comps...>(m_registry,
                                                  std::forward(args)...);
        }

        virtual ~scene_scope() = default;

        std::string get_tag() { return m_tag; }

        operator flecs::world() const { return m_registry; }

        operator flecs::world() { return m_registry; }

    private:
        std::pmr::polymorphic_allocator<> m_object_allocator;
        flecs::world m_registry;
        std::string m_tag = "Undefined Tag";
    };
}; // namespace atlas