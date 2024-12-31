#pragma once
#include <scene/ecs/registry.hpp>
#include <string>

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
        //! @note We provide properties for constructing a scene entity specifically with the entt::registry
        /*
        struct SceneEntity{
            entt::entity EntityHandler{entt::null};
            std::string Tag="Untitled";
            // This UUID is also would be automatically generated everytime you create a new scene object for this scene
            // uint32_t UUID; // this would replace with our generating for the UUID.
        };
        */
    public:
        Scene() = default;
        Scene(const std::string& p_Tag) : m_Tag(p_Tag){}
        ~Scene(){
            m_SceneRegistry.Clear();
        }


        //! @note Creates a new entity for the specific scene with a tag
        // SceneEntity CreateEntity(){
        //     return {m_Registry.create(), "Untitled"};
        // }

        //! @note can be used to delete specific scene objects if need-be
        // void DeleteEntity(entt::entity p_SourceEntity){
        //     m_Registry.destroy(p_SourceEntity);
        // }

        /**
         * @note Function to provide a way to copy from one scene to another
         * @note This would utilize the function inside SceneNode called CopyComponent<UComponent>
         * @note Because if we want to copy scenes we want to copy all of our scene objects with those scenes
         * @note TODO: Still needs to be decided on how this is going to work with SceneNode (in scene_node.hpp)
        */
        // static Scene Copy(const Scene& p_SourceSceneToCopy){
        //     return Scene();
        // }

        //! @note Enables us to treat Scene as also an entt::registry
        //! @note entt::registry is how this scene is going to register scene objects within this scene and it's components
        operator Registry&(){
            return m_SceneRegistry;
        }

    private:
        std::string m_Tag = "Untitled";
        // entt::registry m_Registry;
        Registry m_SceneRegistry;
    };
};