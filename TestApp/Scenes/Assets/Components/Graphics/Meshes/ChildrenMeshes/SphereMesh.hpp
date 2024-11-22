#pragma once
#include <Scenes/Assets/Components/Graphics/Meshes/MeshContainer.hpp>

class SphereMesh : public MeshContainer
{   
    public:
        SphereMesh(int Radius, int SectorCount, int StackCount);
};