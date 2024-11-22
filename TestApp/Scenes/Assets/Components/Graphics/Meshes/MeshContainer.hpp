#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <engine3d/Core/internal/Vulkan2Showcase/VulkanModel.hpp>
//! @brief Factory
class MeshContainer
{
    public:
        MeshContainer() = default;
        std::vector<engine3d::vk::VulkanModel::Vertex>* GetVertices(){return vertices;}
        std::vector<glm::vec3> GetNormals(){return normals;}
        std::vector<glm::vec2> GettexCoords(){return texCoords;}

    protected:   
        std::vector<engine3d::vk::VulkanModel::Vertex>* vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
};