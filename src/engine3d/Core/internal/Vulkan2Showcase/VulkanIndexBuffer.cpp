#include <internal/Vulkan2Showcase/VulkanIndexBuffer.hpp>
#include <internal/Vulkan2Showcase/VulkanContext.hpp>
#include <internal/Vulkan2Showcase/helper_functions.hpp>
#include <Core/EngineLogger.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    VulkanIndexBuffer::VulkanIndexBuffer(std::vector<uint32_t>& p_Indices){
        ConsoleLogInfo("Vulkan2Showcase: Begin Vulkan VertexBuffer Initialization!!");
        m_IndicesCount = static_cast<uint32_t>(p_Indices.size());
        
        m_HasIndices = (m_IndicesCount > 0);

        if(!m_HasIndices){
            ConsoleLogWarn("Cannot provide empty indices to the VulkanIndexBuffer!");
            return;
        }

        VkDeviceSize buffer_size = sizeof(p_Indices[0]) * m_IndicesCount;
        VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        VkMemoryPropertyFlags property_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        
        VkBufferCreateInfo vk_buffer_create_info = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .size = buffer_size,
            .usage = usage_flags,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        vk_check(vkCreateBuffer(VulkanContext::GetDriver(), &vk_buffer_create_info, nullptr, &m_IndexBufferHandler), "vkCreateBuffer", __FILE__, __LINE__, __FUNCTION__);

        VkMemoryRequirements memory_reqs;
        vkGetBufferMemoryRequirements(VulkanContext::GetDriver(), m_IndexBufferHandler, &memory_reqs);

        VkMemoryAllocateInfo alloc_info = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memory_reqs.size,
            .memoryTypeIndex = VulkanContext::GetDriver().SelectMemoryType(memory_reqs.memoryTypeBits, property_flags)
        };

        vk_check(vkAllocateMemory(VulkanContext::GetDriver(), &alloc_info, nullptr, &m_IndexDeviceBufferMemory), "vkAllocateMemory", __FILE__, __LINE__, __FUNCTION__);

        vkBindBufferMemory(VulkanContext::GetDriver(), m_IndexBufferHandler, m_IndexDeviceBufferMemory, 0);

        //! @note Mapping memory data.
        //! @note THIS is how we map our vertices data to our VkBuffer (essentially it is our vertex buffer)

        /**
            @note Purpose of these buffer objects are to write these information to memory.
            @note This maps memory on our host to the device's memory.
            @note Where this creates a data region on the host, that is linked to the device memory
            @note This way when we used memcpy to write to the CPU's accessible memory. That information will also be flushed to the GPU's memory, by this link
            @note Then once this information has been mapped, then we no longer need the data on the host's side, therefore unmapping that information.
        */
        void* data;
        vkMapMemory(VulkanContext::GetDriver(), m_IndexDeviceBufferMemory, 0, buffer_size, 0, &data);
        memcpy(data, p_Indices.data(), static_cast<size_t>(buffer_size));
        vkUnmapMemory(VulkanContext::GetDriver(), m_IndexDeviceBufferMemory);

        ConsoleLogWarn("Vulkan2Showcase: Vulkan VertexBuffer Initialized Completed!!!");
    }

    void VulkanIndexBuffer::BindToIndexBuffer(VkCommandBuffer p_CommandBuffer) {
        VkBuffer buffers[] = {m_IndexBufferHandler};
        VkDeviceSize offsets[] = {0};

        if(m_HasIndices){
            vkCmdBindIndexBuffer(p_CommandBuffer, m_IndexBufferHandler, 0, VK_INDEX_TYPE_UINT32);
        }
    }

    void VulkanIndexBuffer::RenderIndexBuffer(VkCommandBuffer p_CommandBuffer) {
        /**
        //! @note This will happen within Renderer itself, but not here nor VertexBuffer class.
        if(m_HasIndices){
            vkCmdDrawIndexed(p_CommandBuffer, m_IndicesCount, 1, 0, 0, 0);    
        }
        else{
            vkCmdDraw(p_CommandBuffer, m_VerticesCount, 1, 0, 0);
        }
        */

        vkCmdDrawIndexed(p_CommandBuffer, m_IndicesCount, 1, 0, 0, 0);
    }

    bool VulkanIndexBuffer::HasIndices() const{
        return m_HasIndices;
    }
};