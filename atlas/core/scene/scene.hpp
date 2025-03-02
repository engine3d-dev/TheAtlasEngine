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
     * @brief We need to also figure
     *
     *
     *
     *
     *
     *
     * TODO: Create something called SceneData (or SceneProperties)
     * @note This struct would contain properties of the tag, and the UUID
     * attached with this scene object
     * @note Here are properties ot he scene_scope.
     * @note This was the solution so that scenes know who they are associated
     * with. Which would be a look up table.
     * @note I do not like the idea of having a scene_object that contains a
     * pointer to the scene that it's apart of (Purely a design decision)
     *
     * @note scene_object may have similar properties
     * @note Where we need each scene object to be instantiated with an UUID
     *
     * SceneObjectProperties{
     *  UUID: Hash ID to itself
     *  ParentUUID: Hash ID to the scene the object's associated with (use of
     * getting objects to interact with each other)
     * };
     *
     *
     * SceneProperties{
     *  GetUUID: Hash ID of the scene itself
     *  GetParentUUID: World UUID that helps to associate
     * }
     *
     *
     * @note New Idea for get_world(this);
     *
     * New API Would be the following
     *
     * // Setting the scene
     * m_physics_system = physics::physics_system(this);
     *
     * // Inside physics system
     *
     * physics_system(const scene_scope* p_scene_ctx){
     *      m_scene_registry = system_registry::get_world(string Tag:
     * p_scene_ctx);
     * }
     *
     */
    class scene_scope {
    public:
        scene_scope()
          : m_entity(nullptr, "Undefined")
          , m_tag("Undefined") {}
        scene_scope(const std::string& p_tag)
          : m_tag(p_tag) {}

        //! TODO: Could move these as free-standing functions outside the class
        /*

        ref<scene_object> sphere = create_new_object(this, "sphere");

        */
        ref<scene_object> create_new_object(const std::string& p_tag) {
            return create_ref<scene_object>(
              system_registry::get_world().get_registry(), p_tag);
        }

        virtual ~scene_scope() = default;

        std::string get_tag() { return m_tag; }

    private:
        entity_t m_entity;
        std::string m_tag = "Undefined Tag";
    };
}; // namespace atlas