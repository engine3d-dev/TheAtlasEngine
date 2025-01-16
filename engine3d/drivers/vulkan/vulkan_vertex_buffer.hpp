#pragma once
#include <drivers/vertex_buffer.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    class VulkanVertexBuffer : public VertexBuffer{
    public:
        VulkanVertexBuffer(const std::vector<Vertex>& p_Vertices);
        virtual ~VulkanVertexBuffer(){}

    protected:

        void BindVertexBuffer(VkCommandBuffer p_CommandBuffer) override;

        void RenderVertexBuffer(VkCommandBuffer p_CommandBuffer) override;

    private:
        VkBuffer m_VertexBufferHandler;
        VkDeviceMemory m_VertexBufferDeviceMemoryHandler;
        uint32_t m_VerticesCount;
    };
};