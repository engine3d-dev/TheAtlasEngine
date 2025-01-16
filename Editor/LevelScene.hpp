#pragma once
#include <core/scene/scene.hpp>

namespace engine3d{
    /**
     * @name LevelScene
     * @note Implementation of our scene
     * @note Meaning this is just the first iteration of how we can look at these scenes and how they can be defined
     * @note Each scene is a type of scene scope, this means that when a player leaves the outer scope of a scene,
     *       the player transitions into a different scene
     * @note Either the player transitions into a different scene or hits a invisible wall preventing leaving the scope
    */
    class LevelScene : public SceneScope{
    public:
        LevelScene() = default;

        LevelScene(const std::string& p_Tag);

        virtual ~LevelScene(){}

        void Update() override;


    private:
        Ref<SceneObject> m_Sphere;
        Ref<SceneObject> m_Camera;
    };
};