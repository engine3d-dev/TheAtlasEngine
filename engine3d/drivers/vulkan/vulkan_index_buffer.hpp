#pragma once
#include <drivers/index_buffer.hpp>

namespace engine3d::vk{
    class VulkanIndexBuffer : public IndexBuffer {
    public:
        VulkanIndexBuffer(std::vector<uint32_t>& p_Indices);
        virtual ~VulkanIndexBuffer(){}

    private:
        void BindToIndexBuffer(VkCommandBuffer p_CommandBuffer) override;
        void RenderIndexBuffer(VkCommandBuffer p_CommandBuffer) override;
        bool HasIndices() const override;

    private:
        VkBuffer m_IndexBufferHandler;
        VkDeviceMemory m_IndexDeviceBufferMemory;
        uint32_t m_IndicesCount;
        bool m_HasIndices = false;
    };
};