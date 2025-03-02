#pragma once
#include <core/core.hpp>
#include <deque>
#include <flecs.h>
#include <string>

namespace atlas {

    /**
     * @name WorldArea
     * TODO: Do I want to call this world_scope, WorldArea, or WorldInstance
     * (???)
     * @note Seld-contained container of scenes
     * @note Can be loaded/unloaded independent from scenes in itself
     * @note sharing settings between multiple scenes
     * @note manages when a player enters a scene-area
     *
     *
     * @note WorldArea is going to be the scope of the entire environment the
     * player is in
     * @note Meaning if we only have one world, this would be considered the
     * global scope while each of our scenes independently are local scope
     * @note A world area would have a transform associated with it
     *
     * TOPIC: Regarding Gameplay
     * @note How we also define gameplay and startup code matter.
     * @note Typically when engines load up a world and starting off with a
     * single scene
     * @note There are different variations, some engines like unreal have a
     * pre-init, world-loading, and post-loading where unreal also in the middle
     * of those phases does extra checks for checking state validation before
     * the world gets loaded
     * @note Defining how a world can be created and applied can be quite tricky
     * as their are many variations and approaches to this
     * @note For now the approach I have decided to go with is the following
     *
     * @note WorldArea (might change to world_scope)
     * @note Internally the engine itself should handle scenes and how that is
     * managed meaning that BVH may be implemented here, and other combinations
     * of these structures that gets sent to the scene renderer
     * @note The idea that I have are two options currently:
     *       1) Per scene we have scene renderer takes the context of the scene
     * and does a few things, does some pre-load for objects checking, then
     * checks game objects heirarchy (like BVH or something) so the scene
     * renderer knows the order of which to load scenes 1.1) Then if we add
     * camera frustrum, we can utilize spatial data structures to know where in
     * our world should be rendered [Something to Note]: Inside our Editor, some
     * of the way these scenes here are going to be defined, since the editor
     * will also be doing more heavy-lifting for considering that if someone
     * wants to utilize the editor to build their game all the way, they could
     *         1.2) What this allows for us to do is not only render scenes in
     * the order the player is closest to, but also we can integrate what game
     * objects in the current scene the player is in [Cons that I can see]
     *          - Could take long processing larger scenes
     *          - Larger the scene, more time it takes to process per scene
     *          - Need to consider scenes will vary in size, how is that
     * accounted for
     *
     *              and the objects closer or in other words in the view of the
     * camera frustrum (player's perspective) 2) Pass in the world context 2.1)
     * What this means is that instead of passing context per scene we can
     *
     * @note Something to consider
     * @note When implementing the Editor we can a
     */
    class scene_scope;
    class world_scope {
    public:
        world_scope() = default;
        world_scope(const std::string& p_tag);

        std::string get_tag() { return m_tag; }

        void add_scene(ref<scene_scope> p_scene_context);

        flecs::world* get_registry() { return &m_scene_registry; }

        operator flecs::world&() { return m_scene_registry; }

    private:
        flecs::world m_scene_registry;
        //! @note Should be managed by either octal-trees or some scene spatial
        //! data structure
        //! @note But not by using deque, TODO: Make changes to this to use diff
        //! data structures
        std::deque<ref<scene_scope>> m_scene_manager_queue;
        ref<scene_scope> m_current_scene;
        std::string m_tag = "Undefined Tag";
    };
}; // namespace atlas