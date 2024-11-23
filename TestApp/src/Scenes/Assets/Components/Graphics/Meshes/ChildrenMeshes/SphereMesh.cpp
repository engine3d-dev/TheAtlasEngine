#include "Core/internal/Vulkan2Showcase/VulkanModel.hpp"
#include <Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/SphereMesh.hpp>
#include <numbers>
SphereMesh::SphereMesh(int Radius, int SectorCount, int StackCount)
{

    std::vector<glm::vec3> verts;
    std::vector<glm::vec3>().swap(normals);
    std::vector<glm::vec3>().swap(verts);
    std::vector<glm::vec2>().swap(texCoords);
    float nx, ny, nz, LengthInv = 1.0f / Radius; //vertex normal
    float x, y, z, xy;                           //vertex position
    float s, t;                                  //vertex texCoord
    float SectorStep = 2 * std::numbers::pi / SectorCount;
    float StackStep = std::numbers::pi / StackCount;
    float SectorAngle, StackAngle;
    for(int i = 0; i <= StackCount; ++i)
    {
        // starting from pi/2 to -pi/2
        StackAngle = std::numbers::pi / 2 - i * StackStep; 
        xy = Radius * cosf(StackAngle); // r * cos(u)
        z = Radius * sinf(StackAngle);  // r * sin(u)

        // add (SectorCount+1) vertices per stack
        // first and last vertices have same position 
        // and normal, but different tex coords
        for(int j = 0; j <= SectorCount; ++j)
        {
        
            SectorAngle = j * SectorStep;         // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(SectorAngle);        // r * cos(u) * cos(v)
            y = xy * sinf(SectorAngle);        // r * cos(u) * sin(v)
            verts.push_back(glm::vec3(x,y,z));

            // normalized vertex normal (nx, ny, nz)
            nx = x * LengthInv;
            ny = y * LengthInv;
            nz = z * LengthInv;
            normals.push_back(glm::vec3(nx,ny,nz));

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / SectorCount;
            t = (float)i / StackCount;
            texCoords.push_back(glm::vec2(s,t));
        }
    }

    vertices = engine3d::vk::VulkanModel::CreateVertexVector(verts);
}