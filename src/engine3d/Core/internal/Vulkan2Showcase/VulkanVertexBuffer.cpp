#include <Core/EngineLogger.hpp>
#include <Core/internal/Vulkan2Showcase/VulkanContext.hpp>
#include <Core/internal/Vulkan2Showcase/helper_functions.hpp>
#include <Core/internal/Vulkan2Showcase/VulkanVertexBuffer.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    VulkanVertexBuffer::VulkanVertexBuffer(const std::vector<Vertex>& p_Vertices){
        ConsoleLogInfo("Vulkan2Showcase: Begin Vulkan VertexBuffer Initialization!!");
        m_VerticesCount = static_cast<uint32_t>(p_Vertices.size());
        VkDeviceSize buffer_size = sizeof(p_Vertices[0]) * m_VerticesCount;
        VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        VkMemoryPropertyFlags property_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        
        VkBufferCreateInfo vk_buffer_create_info = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .size = buffer_size,
            .usage = usage_flags,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        vk_check(vkCreateBuffer(VulkanContext::GetDriver(), &vk_buffer_create_info, nullptr, &m_VertexBufferHandler), "vkCreateBuffer", __FILE__, __LINE__, __FUNCTION__);

        VkMemoryRequirements memory_reqs;
        vkGetBufferMemoryRequirements(VulkanContext::GetDriver(), m_VertexBufferHandler, &memory_reqs);

        VkMemoryAllocateInfo alloc_info = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memory_reqs.size,
            .memoryTypeIndex = VulkanContext::GetDriver().SelectMemoryType(memory_reqs.memoryTypeBits, property_flags)
        };

        vk_check(vkAllocateMemory(VulkanContext::GetDriver(), &alloc_info, nullptr, &m_VertexBufferDeviceMemoryHandler), "vkAllocateMemory", __FILE__, __LINE__, __FUNCTION__);

        vkBindBufferMemory(VulkanContext::GetDriver(), m_VertexBufferHandler, m_VertexBufferDeviceMemoryHandler, 0);

        //! @note Mapping memory data.
        //! @note THIS is how we map our vertices data to our VkBuffer (essentially it is our vertex buffer)
        void* data;
        vkMapMemory(VulkanContext::GetDriver(), m_VertexBufferDeviceMemoryHandler, 0, buffer_size, 0, &data);
        memcpy(data, p_Vertices.data(), static_cast<size_t>(buffer_size));
        vkUnmapMemory(VulkanContext::GetDriver(), m_VertexBufferDeviceMemoryHandler);
        ConsoleLogWarn("Vulkan2Showcase: Vulkan VertexBuffer Initialized Completed!!!");
    }

    void VulkanVertexBuffer::BindVertexBuffer(VkCommandBuffer p_CommandBuffer) {
        VkBuffer buffers[] = {m_VertexBufferHandler};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(p_CommandBuffer, 0, 1, buffers, offsets);
    }
        
    void VulkanVertexBuffer::RenderVertexBuffer(VkCommandBuffer p_CommandBuffer) {
        vkCmdDraw(p_CommandBuffer, m_VerticesCount, 1, 0, 0);
    }
};