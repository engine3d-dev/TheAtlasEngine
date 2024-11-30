#pragma once
#include <Core/Core.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    class UniformBuffer{
    public:
        UniformBuffer() = default;
        /*
        UniformBuffer(
            p_InstanceSize:         sizeof(Data),
            p_InstanceCount:        MAX_FRAMES_IN_FLIGHT,
            p_UsageFlags:           VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            p_MemoryPropertyFlags:  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            p_MinOffsetAlignment:   VulkanDriver::GetProperties().minUniformBUfferOffsetAlignment
        )
        */
        UniformBuffer(uint64_t p_InstanceSize, uint32_t p_InstanceCount, VkBufferUsageFlags p_UsageFlags, VkMemoryPropertyFlags p_MemoryPropertyFlags, uint64_t p_MinOffsetAlignment=1);

        operator VkBuffer(){
            return m_UniformBufferHandler;
        }

        void MapData(uint64_t p_DevSize = VK_WHOLE_SIZE, uint64_t p_Offset=0);
        void UnmapData();

        //! @note WriteToBuffer
        void CopyTo(void* p_Data, uint64_t p_Size, uint64_t p_Offset);

        void Flush(uint64_t p_Size, uint64_t p_Offset);

        void Invalidate(uint64_t p_Size, uint64_t p_Offset);

        VkDescriptorBufferInfo InitializeDescriptorInfo(uint64_t p_Size, uint64_t p_Offset);

        void WriteAt(void* p_Data, int p_Index);

        void FlushAt(int p_Index);

        VkDescriptorBufferInfo InitializeDescriptorInfoAtIndex(int p_Index);

        void InvalidateIndex(int p_Index);

    private:
        // uint64_t GetAlignment(uint64_t p_InstanceSize, uint64_t p_MinOffsetAlignment);

    private:
        VkBuffer m_UniformBufferHandler;
        VkDeviceMemory m_UboDeviceMemory;
        uint64_t m_InstanceSize;
        uint32_t m_InstanceCount;
        uint64_t m_AlignmentSize;
        uint64_t m_BufferSize;
        // VkBufferUsageFlags m_UsageFlags;
        // VkMemoryPropertyFlags m_MemoryPropertyFlag;

        void* m_MappedData;

    };
};