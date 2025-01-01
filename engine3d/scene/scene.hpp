#pragma once
// #include <scene/ecs/registry.hpp>
#include "scene/components/components.hpp"
#define FLECS_CPP        // Build FLECS_SYSTEM
#include <flecs.h>
#include <string>
#include <scene/ecs/entity.hpp>

namespace engine3d{
    /**
     * @name Scene
     * @note Defines what a scene is
     * @note Initializes our actual scene and objects that may be contained within this scene
     * 
     * @note A few operations (not limited to) that I can see being part of the way its handled within the scene is the following:
     * @note Creating/Deleting scene nodes/objects within a given scene
     * @note Copying from one scene to another
     * 
     * @note entt::registry is defined in the documentation for the use of managing entities (identifiers) and components
     * @note Meaning since a scene graph helps manages our scene graph is essentially the way an entire structure of an entire scene object
     * @note Something TODO: entt::registry be something defined by scene graph to manage the entity
     * @note Since a scene graph will manage its own sets of entities, rather then have one entire registry that manages all of the entities and components
     * 
    */
    class Scene{
    public:
        Scene() = default;
        Scene(const std::string& p_Tag);
        
        ~Scene();

        EntityObject CreateEntity(const std::string& p_Tag);

        void Test();

        /**
         * @note Function to provide a way to copy from one scene to another
         * @note This would utilize the function inside SceneNode called CopyComponent<UComponent>
         * @note Because if we want to copy scenes we want to copy all of our scene objects with those scenes
         * @note TODO: Still needs to be decided on how this is going to work with SceneNode (in scene_object.hpp)
        */
        // static Scene Copy(const Scene& p_SourceSceneToCopy){
        //     return Scene();
        // }

    private:
        std::string m_Tag = "Untitled";
        flecs::world m_SceneRegistry;
    };
};