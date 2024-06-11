#pragma once
#include <engine3d/Core/core.h>
#include "glm/glm.hpp"

namespace engine3d{
    //! @note Meshes
    class Vertex{
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec3 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    class Mesh{
    public:

    private:
    };



    class Asset{
    public:
        Asset(const std::string& path){}
        
        bool IsLoaded() const { return m_IsAssetLoaded; }

    private:
        bool m_IsAssetLoaded = false;
    };
};