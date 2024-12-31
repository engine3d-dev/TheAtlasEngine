#pragma once
#include <glm/glm.hpp>
#include <core/core.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace engine3d{
    /*
        @name VertexBuffer
        @note Contains vertices and does backend API stuff.
    */
    struct Vertex{
        glm::vec3 Position;
        glm::vec3 Color;
        glm::vec3 Normals;
        glm::vec2 Uv;

        bool operator==(const Vertex& other) const{
            return (Position == other.Position and Color == other.Color and Normals == other.Normals and Uv == other.Uv);
        }
    };

    class VertexBuffer{
    public:
        virtual ~VertexBuffer() = default;

        static Ref<VertexBuffer> Create(const std::vector<Vertex>& p_Vertices);

        void Bind(VkCommandBuffer p_CommandBuffer);

        void Draw(VkCommandBuffer p_Command);

    protected:
        virtual void BindVertexBuffer(VkCommandBuffer p_CommandBuffer) = 0;
        virtual void RenderVertexBuffer(VkCommandBuffer p_CommandBuffer) = 0;
    };
};