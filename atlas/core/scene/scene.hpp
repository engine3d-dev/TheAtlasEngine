#pragma once
#include <core/core.hpp>
#include <core/engine_logger.hpp>
#include <core/scene/scene_object.hpp>
#include <string>
#include <core/scene/types.hpp>

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
        scene_scope(const std::string& p_name)
          : m_name(p_name) {}

        strong_ref<scene_object> create_object(const std::string& p_tag) {
            return create_strong_ref<scene_object>(
              m_allocator, &m_registry, p_tag);
        }

        // TODO -- come back to this
        // The idea behind this is to acquire a custom object that can be
        // defined by the application-side Such as character_controller,
        // camera_controller, etc.
        template<typename T, typename... Args>
        strong_ref<scene_object> acquire_object(Args&&... args) {
			static_assert(std::is_base_of_v<scene_scope, T>,
                          "invalid scene_object not inherited with base class "
                          "being scene_object");
            return create_strong_ref<scene_object>(m_allocator,
                                                   std::forward<T>(args...));
        }

        template<typename... Comps, typename... Args>
        flecs::query_builder<Comps...> query_builder(Args&&... args) const {
            return flecs::query_builder<Comps...>(m_registry,
                                                  std::forward(args)...);
        }

        strong_ref<scene_object> search_entity(const std::string& p_name) {
            return memory::make_strong_ptr<scene_object>(
              m_allocator, &m_registry, p_name, false);
        }

		[[nodiscard]] bool defer_begin() const {
			return m_registry.defer_begin();
		}

		[[nodiscard]] bool defer_end() const {
			return m_registry.defer_end();
		}

        virtual ~scene_scope() = default;

        [[nodiscard]] std::string name() const { return m_name; }

        // It's required that the flecs::world is returned by reference
        // This prevents corruption onto the flecs::world object
        operator world&() { return m_registry; }

    private:
        std::pmr::polymorphic_allocator<> m_allocator;
        world m_registry;
		std::string m_name;
    };
}; // namespace atlas