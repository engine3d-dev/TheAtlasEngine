#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    struct Vertex{
        glm::vec4 Position;
        glm::vec2 uv; // {u, v} = {x, y}
    };
    
    // template<typename VertexType, size_t Size>
    class VulkanVertexBuffer{
    public:
        struct BufferData{
            BufferData() : Buffer(VK_NULL_HANDLE), DeviceMemory(VK_NULL_HANDLE), Size(0), MemoryPropertyFlags(0), MappedData(nullptr) {}
            VkBuffer Buffer;
            VkDeviceMemory DeviceMemory;
            size_t Size;
            VkMemoryPropertyFlags MemoryPropertyFlags;
            void* MappedData;
            VkDescriptorBufferInfo BufferInfo;
        };

        VulkanVertexBuffer() = default;
        // VulkanVertexBuffer(VkBuffer p_Buffer, VkDeviceMemory p_Memory, size_t p_Size, const std::vector<Vertex>& p_Triangle, std::vector<float> p_MappedData = std::vector<float>());
        VulkanVertexBuffer(const std::vector<Vertex>& p_Triangle);
        // VulkanVertexBuffer(const std::array<VertexType, Size>& p_Data);

    private:
        void initialize_vertex_buffeR(BufferData& p_BufferData, const uint8_t* p_Data, const VkBufferUsageFlags& p_Usage);

        bool SelectMemoryTypeProperties(const VkPhysicalDeviceMemoryProperties& p_Flags, uint32_t typeBits, VkFlags requirements_mask, uint32_t* typeIndex);

        size_t AlignedDataSize(size_t dataSize, size_t minimumAlignment);

    private:

        VkPhysicalDeviceMemoryProperties m_PhysicalDeviceMemoryProperties;

        VkBuffer m_Buffer;
        VkDeviceMemory m_Memory;
        size_t m_Size;
        VkMemoryPropertyFlags m_MemoryPropertyFlags;
        std::vector<float> m_MappedData;
        VkDescriptorBufferInfo m_BufferInfo;

        BufferData m_VertexBuffer;
        BufferData m_DynamicUniformBuffer;
    };
};