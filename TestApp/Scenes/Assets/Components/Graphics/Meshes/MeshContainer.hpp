#pragma once
#include <Core/Core.hpp>
#include <engine3d/Core/GraphicDrivers/VertexBuffer.hpp>
#include <glm/glm.hpp>
#include <vector>

//! @brief Factory
class MeshContainer
{
    public:
        MeshContainer() = default;
        engine3d::Ref<engine3d::VertexBuffer> GetVertices(){return vertices;}
        std::vector<glm::vec3> GetNormals(){return normals;}
        std::vector<glm::vec2> GettexCoords(){return texCoords;}

    protected:   
        engine3d::Ref<engine3d::VertexBuffer> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
};