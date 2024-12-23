#pragma once
#include <entt/entt.hpp>
#include <memory>

namespace engine3d{
    /**
     * @name SceneNode
     * @note Defines our singular scene where our scene objects may be located.
     * 
    
    SceneNode and Scene Object.



    Scene Node knows about its parent, not the Scene Object.
    Scene Object is defined as the geometry within the scene

    Scene Node and Scene Object are the same thing.

    SceneGraph will consist of SceneNodes. Every scene know may have a list of scene objects per scene node.
    - SceneGraph needs to be DAG (Directed Acyclic Graphs)


    Systems Design
    Scene
    - Contains a collection of scene objects/nodes.
    - Scene Graph essentially is our world. It is a collection of scenes that has scene nodes to them

    SceneObject/SceneNode
    - An example is if we have a car.
        - The engine (is scene node), 4 wheels (4 scene nodes), car base (scene node), etc
        - Scene Nodes are scene objects that make up our entire scene graph
        - Scene Graph is the construction of multiple objects grouped together

    */
    struct SceneNode{
        //! @note per scene tag.
        std::string Tag;

        //! @note registry on how scene objects can attach components.
        entt::registry Registry;
    };
};