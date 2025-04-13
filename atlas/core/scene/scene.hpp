#pragma once
#include <core/engine_logger.hpp>
#include <core/scene/scene_object.hpp>
#include <core/scene/world.hpp>
#include <core/system_framework/system_registry.hpp>
#include <string>

namespace atlas {
    /**
     * @name scene_scope
     * @note Self-contained scope that is defining a scene
     * @note flecs::world is instantiate within our world_scope structure, using
     * that class to manage multiple scene
     * @note We utilize system_registry::get_world() to create our scene objects
     * and associating our scene objects with those scenes
     * @note The way we set this is up may change to actually do the following
     * later.....
     * \example
     *
     * Example New Implementation Alternative #1
     * // This takes in the scene that associates with our world
     * // Because flecs::world is managing our entities, it makes sense to
     * rather managing groups of those scene obejcts with the world, you'd have
     * the world manage those entities and when they transition to multiple
     * scene
     * // We may utilize the hash_id (UUID) to tell where this scene belongs to
     * and what scene this object at creation is associating with itself to auto
     * new_object_sphere = create_new_object(system_registry::Get(this),
     * Tag.c_str()); new_object_sphere.add<PhysicsBody>();
     * 
     * @brief scene_scope is defining a scope of a scene
     * @brief Scenes are part of the world; where the world contains sets of scenes per world
     */
    class scene_scope {
    public:
        scene_scope() : m_tag("Undefined") {}
        scene_scope(const std::string& p_tag)
          : m_tag(p_tag) {}

        //! TODO: Could move these as free-standing functions outside the class
        /*

        ref<scene_object> sphere = create_new_object(this, "sphere");

        */
        ref<scene_object> create_new_object(const std::string& p_tag) {
            // return create_ref<scene_object>(system_registry::get_world().get_registry(), p_tag);
            return create_ref<scene_object>(&m_registry, p_tag);
        }

        template <typename... Comps, typename... Args>
        flecs::query_builder<Comps...> query_builder(Args &&... args) const {
            return flecs::query_builder<Comps...>(m_registry, std::forward(args)...);
        }

        virtual ~scene_scope() = default;

        std::string get_tag() { return m_tag; }

    private:
        // entity_t m_entity;
        flecs::world m_registry;
        std::string m_tag = "Undefined Tag";
    };
}; // namespace atlas