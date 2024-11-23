#pragma once
#include <Core/Core.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    /*
        @name VertexBuffer
        @note Contains vertices and does backend API stuff.
    */
    struct Vertex{
        glm::vec3 Position;
        glm::vec3 Color;
    };

    class VertexBuffer{
    public:
        virtual ~VertexBuffer() = default;

        static Ref<VertexBuffer> Create(const std::vector<Vertex>& p_Vertices);
        static Ref<VertexBuffer> CreateVertexVector(const std::vector<glm::vec3>& p_Vertices, std::vector<glm::vec3> p_Colors);
        static Ref<VertexBuffer> CreateVertexVector(const std::vector<glm::vec3>& p_Vertices);

        void Bind(VkCommandBuffer p_CommandBuffer);

        void Draw(VkCommandBuffer p_Command);

    protected:
        virtual void BindVertexBuffer(VkCommandBuffer p_CommandBuffer) = 0;
        virtual void RenderVertexBuffer(VkCommandBuffer p_CommandBuffer) = 0;
    };

    // vector<vector<vecs>>
};