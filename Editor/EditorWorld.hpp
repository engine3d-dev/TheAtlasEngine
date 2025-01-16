#pragma once
#include "LevelScene.hpp"

/**
 * @name EditorWorld
 * @note Dummy implementation of our world
 * @note What does WorldScope mean?
 *      - WorldScope means you are creating a container of multiple scenes
 *      - Each container will contain a mixture of a octal-tree and BVH structure that helps to know when player enters specific scene areas
 *      - Rather then having the octal-tree per scene, we have that be part of the world structure
*/

namespace engine3d{
    class EditorWorld{
    public:
        EditorWorld();
        EditorWorld(const std::string& p_Tag);

        void OnUpdate();

    private:
        engine3d::WorldScope m_MainWorld;
        //! TODO: Would be handled by our system registry
        engine3d::Ref<LevelScene> m_FirstLevelScene;
        std::string m_Tag = "Undefined";
    };
}; // end of engine3d namespace