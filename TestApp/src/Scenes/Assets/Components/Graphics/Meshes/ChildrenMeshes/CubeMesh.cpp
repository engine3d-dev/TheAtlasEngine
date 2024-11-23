
#include "Core/EngineLogger.hpp"
#include "Core/GraphicDrivers/VertexBuffer.hpp"
#include <Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/CubeMesh.hpp>
#include <glm/fwd.hpp>
#include <vector>

CubeMesh::CubeMesh()
{
    std::vector<glm::vec3> verts = 
    {
    {.5f, -.5f, -.5f},
    {.5f, -.5f, .5f},
    {.5f, -.5f, -.5f},
    {.5f, .5f, -.5f},
    {.5f, .5f, .5f},
    {-.5f, -.5f, -.5},
    {.5f, -.5f, .5f},
    {-.5f, -.5f, .5f},
    {-.5f, -.5f, -.5},
    {.5f, -.5f, -.5f},
    {.5f, -.5f, .5f},
    {-.5f, .5f, -.5f},
    {.5f, .5f, .5f},
    {-.5f, .5f, .5f},
    {-.5f, .5f, -.5f},
    {.5f, .5f, -.5f},
    {.5f, .5f, .5f},
    {-.5f, -.5f, 0.5},
    {.5f, .5f, 0.5f},
    {-.5f, .5f, 0.5f},
    {-.5f, -.5f, 0.5},
    {.5f, -.5f, 0.5f},
    {.5f, .5f, 0.5f},
    {-.5f, -.5f, -0.5},
    {.5f, .5f, -0.5f},
    {-.5f, .5f, -0.5},
    {-.5f, -.5f, -0.5},
    {.5f, -.5f, -0.5},
    {.5f, .5f, -0.5f}

    };

    std::vector<glm::vec3> colors =
    {
    {.8f, .8f, .1f},
    {.8f, .8f, .1f},
    {.8f, .8f, .1f},
    {.8f, .8f, .1f},
    {.8f, .8f, .1f},
    {.9f, .6f, .1f},
    {.9f, .6f, .1f},
    {.9f, .6f, .1f},
    {.9f, .6f, .1f},
    {.9f, .6f, .1f},
    {.9f, .6f, .1f},
    {.8f, .1f, .1f},
    {.8f, .1f, .1f},
    {.8f, .1f, .1f},
    {.8f, .1f, .1f},
    {.8f, .1f, .1f},
    {.8f, .1f, .1f},

    {.1f, .1f, .8f},
    {.1f, .1f, .8f},
    {.1f, .1f, .8f},
    {.1f, .1f, .8f},
    {.1f, .1f, .8f},
    {.1f, .1f, .8f},

    {.1f, .8f, .1},
    {.1f, .8f, .1f},
    {.1f, .8f, .1f},
    {.1f, .8f, .1},
    {.1f, .8f, .1f},
    {.1f, .8f, .1f}
    };

    vertices = engine3d::VertexBuffer::CreateVertexVector(verts, colors);
    if(vertices != nullptr){
        ConsoleLogWarn("SphereMesh vertices are a nullptr. Implement VertexBuffer::CreateVertexVector() in VulkanVertexBuffer to get this working and not nullptr");
    }
}
