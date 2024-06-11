#pragma once
#include <engine3d/Core/core.h>

namespace engine3d{

    /**
     * @name EnvironmentMap
     * @note Is going to be our base for different environment mappings
     * @note To enable how to configure our cubemap(skybox), spheremaps
     * @note While also allowing the client-side to make modifications in creating world viewpoints.
     * @note When client were to create new project, there will be default environment map in the editor
     * 
     * @note Client should not know anything about our actual default environment
     * @note All client should know about is how to configure their environment map
     * @example
     * auto& envMap = Worldview::GetCurrentEnvironmentMap();
     * /// do something ....
     * 
     * @note Class to consider having is WorldView class for our actual world viewpoint
     * @note  EditorWorldView for our editor's runtime world skybox for editing a game
     * @note This may also contain light sources, and various other things (TBD)
    */
    class EnvironmentMap{
    public:
        EnvironmentMap(std::array<std::string, 6>& faces);


    private:

    };
};