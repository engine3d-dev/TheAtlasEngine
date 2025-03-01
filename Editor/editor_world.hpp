#pragma once
#include "level_scene.hpp"

/**
 * @name editor_world
 * @note Dummy implementation of our world
 * @note What does world_scope mean?
 *      - world_scope means you are creating a container of multiple scenes
 *      - Each container will contain a mixture of a octal-tree and BVH structure that helps to know when player enters specific scene areas
 *      - Rather then having the octal-tree per scene, we have that be part of the world structure
*/

class editor_world{
public:
    editor_world();
    editor_world(const std::string& p_tag);

private:
    atlas::world_scope m_main_world;
    //! TODO: Would be handled by our system registry
    atlas::ref<level_scene> m_first_scene;
    std::string m_tag = "Undefined";
};