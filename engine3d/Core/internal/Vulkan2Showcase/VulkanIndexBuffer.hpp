#pragma once
#include <Core/GraphicDrivers/IndexBuffer.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    class VulkanIndexBuffer : public IndexBuffer {
    public:
        VulkanIndexBuffer(std::vector<uint32_t>& p_Indices);

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