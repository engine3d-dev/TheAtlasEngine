#include "Scenes/Assets/Components/Physics/PhysicsBody3D.hpp"
#include <Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/SphereMesh.hpp>
#include <Scenes/Assets/Components/Graphics/Meshes/ChildrenMeshes/CubeMesh.hpp>
class MeshContainer
{
    public:
        MeshContainer(int Radius, int SectorCount, int StackCount) : 
        Radius(Radius), SectorCount(SectorCount), StackCount(StackCount){};
        
        engine3d::Transform& GetTransform() 
        {
         return m_Transform;
        };

        const engine3d::Transform& GetTransform() const 
        {
            return m_Transform;
        };

        void SphereBlueprint();
        void CubeBlueprint();
        
    private:
        // Geometry parameters
        const int Radius;
        const int SectorCount;
        const int StackCount;

        // Transform component
        engine3d::Transform m_Transform;

        // Geometry data
        glm::vec3 m_TransformPosition;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
};