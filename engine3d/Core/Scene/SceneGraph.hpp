#pragma once
#include <entt/entt.hpp>

namespace engine3d{
    /*
    Scene Graph
    - Typically are Directed Acyclic Graphs (DAG) that should not cycle
    - Constructs groups of scene objects/nodes together to represent an object within a scene
    - Each SceneNode/Object should contain children nodes
    - Scene node's have a parent-child relationships on how objects are spatially organized
    
    For explicit information, a scene graph would be handled for you if you were to create your own scene object.
    */
    class SceneGraph{
    public:
        //! @param p_Filename is the filename to the mesh that we want to load from.
        SceneGraph(const std::string& p_Filename);


    private:
        entt::registry m_Registry;
    };
};