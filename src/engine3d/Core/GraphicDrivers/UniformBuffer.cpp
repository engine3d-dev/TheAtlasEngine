#include <Core/GraphicDrivers/UniformBuffer.hpp>
#include <internal/Vulkan2Showcase/VulkanContext.hpp>
#include <internal/Vulkan2Showcase/helper_functions.hpp>
#include <Core/EngineLogger.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    static uint64_t GetAlignment(uint64_t p_InstanceSize, uint64_t p_MinOffsetAlignment){
        if(p_MinOffsetAlignment > 0){
            return (p_InstanceSize + p_MinOffsetAlignment-1) & ~(p_MinOffsetAlignment - 1);
        }

        return p_InstanceSize;
    }

    /*
    UniformBuffer(
        p_InstanceSize:         sizeof(Data),
        p_InstanceCount:        MAX_FRAMES_IN_FLIGHT,
        p_UsageFlags:           VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        p_MemoryPropertyFlags:  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        p_MinOffsetAlignment:   VulkanDriver::GetProperties().minUniformBUfferOffsetAlignment
    )
    */
    UniformBuffer::UniformBuffer(uint64_t p_InstanceSize, uint32_t p_InstanceCount, VkBufferUsageFlags p_UsageFlags, VkMemoryPropertyFlags p_MemoryPropertyFlags, uint64_t p_MinOffsetAlignment) : m_InstanceSize(p_InstanceSize), m_InstanceCount(p_InstanceCount), m_AlignmentSize(p_MinOffsetAlignment){
        ConsoleLogInfo("Vulkan2Showcase: Begin Vulkan UniformBuffer Initialization!!!");
        m_AlignmentSize = GetAlignment(p_InstanceSize, p_MinOffsetAlignment);
        m_BufferSize = m_AlignmentSize * p_InstanceCount;

        // m_UsageFlags = p_UsageFlags;
        // m_MemoryPropertyFlag = p_MemoryPropertyFlags;

        VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        // VkMemoryPropertyFlags mem_prop_flag = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
        VkMemoryPropertyFlags mem_prop_flag = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

        VkBufferCreateInfo vk_ubo_create_info = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .size = m_BufferSize,
            // .usage = m_UsageFlags,
            .usage = usage_flags,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        vk::vk_check(vkCreateBuffer(vk::VulkanContext::GetDriver(), &vk_ubo_create_info, nullptr, &m_UniformBufferHandler), "vkCreateBuffer", __FILE__, __LINE__, __FUNCTION__);

        VkMemoryRequirements memory_reqs;
        vkGetBufferMemoryRequirements(vk::VulkanContext::GetDriver(), m_UniformBufferHandler, &memory_reqs);

        VkMemoryAllocateInfo alloc_info = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memory_reqs.size,
            // .memoryTypeIndex = vk::VulkanContext::GetDriver().SelectMemoryType(memory_reqs.memoryTypeBits, m_MemoryPropertyFlag)
            .memoryTypeIndex = vk::VulkanContext::GetDriver().SelectMemoryType(memory_reqs.memoryTypeBits, mem_prop_flag)
        };

        vk::vk_check(vkAllocateMemory(vk::VulkanContext::GetDriver(), &alloc_info, nullptr, &m_UboDeviceMemory), "vkAllocateMemory", __FILE__, __LINE__, __FUNCTION__);

        vkBindBufferMemory(vk::VulkanContext::GetDriver(), m_UniformBufferHandler, m_UboDeviceMemory, 0);

        // //! @note Mapping memory data.
        // //! @note THIS is how we map our vertices data to our VkBuffer (essentially it is our vertex buffer)
        // void* data;
        // vkMapMemory(vk::VulkanContext::GetDriver(), m_UboDeviceMemory, 0, buffer_size, 0, &data);
        // memcpy(data, p_Vertices.data(), static_cast<size_t>(buffer_size));
        // vkUnmapMemory(vk::VulkanContext::GetDriver(), m_UboDeviceMemory);'
        
        //! @note Probably utilize ubo.map() for this instead.
        ConsoleLogWarn("Vulkan2Showcase: Vulkan UniformBuffer Initialized Completed!!!");
    }


    //! @note In tutorial referred to as .map()
    void UniformBuffer::MapData(uint64_t p_DevSize, uint64_t p_Offset){
        if(m_UniformBufferHandler == nullptr and m_UboDeviceMemory == nullptr){
            ConsoleLogFatal("You tried to map memory to an uninitialized uniform buffer!!!");
            return;
        }

        vk::vk_check(vkMapMemory(vk::VulkanContext::GetDriver(), m_UboDeviceMemory, p_Offset, p_DevSize, 0, &m_MappedData), "vkMapMemory", __FILE__, __LINE__, __FUNCTION__);
    }

    void UniformBuffer::UnmapData(){
        if(m_MappedData){
            vkUnmapMemory(vk::VulkanContext::GetDriver(), m_UboDeviceMemory);
            m_MappedData = nullptr;
        }
    }

    void UniformBuffer::CopyTo(void* p_Data, uint64_t p_BufferSize, uint64_t p_Offset){
        if(p_BufferSize == VK_WHOLE_SIZE){
            memcpy(m_MappedData, p_Data, m_BufferSize);
        }
        else{
            char* mem_offset = (char*)m_MappedData;
            mem_offset += p_Offset;
            memcpy(mem_offset, p_Data, p_BufferSize);
        }
    }

    void UniformBuffer::Flush(uint64_t p_Size, uint64_t p_Offset){
        VkMappedMemoryRange mapped_range = {
            .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
            .memory = m_UboDeviceMemory,
            .offset = p_Offset,
            .size = p_Size
        };

        vk::vk_check(vkFlushMappedMemoryRanges(vk::VulkanContext::GetDriver(), 1, &mapped_range), "vkFlushMappedMemoryRanges", __FILE__, __LINE__, __FUNCTION__);
    }

    void UniformBuffer::Invalidate(uint64_t p_Size, uint64_t p_Offset){
        VkMappedMemoryRange map_mem_range = {
            .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
            .memory = m_UboDeviceMemory,
            .offset = p_Offset,
            .size = p_Size
        };

        vk::vk_check(vkInvalidateMappedMemoryRanges(vk::VulkanContext::GetDriver(), 1, &map_mem_range), "vkInvalidateMappedMemoryRanges", __FILE__, __LINE__, __FUNCTION__);
    }

    VkDescriptorBufferInfo UniformBuffer::InitializeDescriptorInfo(uint64_t p_Size, uint64_t p_Offset){
        return VkDescriptorBufferInfo{
            m_UniformBufferHandler,
            p_Offset,
            p_Size
        };
    }

    void UniformBuffer::WriteAt(void* p_Data, int p_Index){
        CopyTo(p_Data, m_InstanceCount, p_Index * m_AlignmentSize);
    }

    void UniformBuffer::FlushAt(int p_Index){
        Flush(m_AlignmentSize, p_Index * m_AlignmentSize);
    }

    VkDescriptorBufferInfo UniformBuffer::InitializeDescriptorInfoAtIndex(int p_Index){
        return InitializeDescriptorInfo(m_AlignmentSize, m_AlignmentSize * p_Index);
    }

    void UniformBuffer::InvalidateIndex(int p_Index){
        return Invalidate(m_AlignmentSize, p_Index * m_AlignmentSize);
    }
};