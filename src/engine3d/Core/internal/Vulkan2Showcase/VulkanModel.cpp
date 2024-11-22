#include "EngineLogger.hpp"
#include <internal/Vulkan2Showcase/helper_functions.hpp>
#include <internal/Vulkan2Showcase/VulkanContext.hpp>
#include <internal/Vulkan2Showcase/VulkanModel.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    VulkanModel::VulkanModel(const std::vector<Vertex>& p_Vertices, const VkBufferUsageFlags& p_UsageFlags, const VkMemoryPropertyFlags& p_PropertyFlags){
        ConsoleLogInfo("Vulkan2Showcase: Begin Vulkan Model Initialization!!");
        m_VerticesCount = static_cast<uint32_t>(p_Vertices.size());
        VkDeviceSize buffer_size = sizeof(p_Vertices[0]) * m_VerticesCount;

        //! @note Host = CPU, Device = GPU
        /*
        
        createBuffer(
            VkDeviceSize:           VCbufferSize,
            VkBufferUsageFlags:     VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            VkBuffer:               vertexBuffer,
            VkDeviceMemory:         vertexDeviceMemory
        );

        */

        //! @note Initialize Vertex Buffer
        //! @not eDoes initialize_vertex_buffers(vertices) stuff
        VkBufferCreateInfo vk_buf_create_info = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .size = buffer_size,
            .usage = p_UsageFlags,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        vk_check(vkCreateBuffer(VulkanContext::GetDriver(), &vk_buf_create_info, nullptr, &m_VertexBuffer), "vkCreateBuffer", __FILE__, __LINE__, __FUNCTION__);

        VkMemoryRequirements memory_reqs;
        vkGetBufferMemoryRequirements(VulkanContext::GetDriver(), m_VertexBuffer, &memory_reqs);

        VkMemoryAllocateInfo alloc_info = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memory_reqs.size,
            .memoryTypeIndex = VulkanContext::GetDriver().SelectMemoryType(memory_reqs.memoryTypeBits, p_PropertyFlags)
        };

        vk_check(vkAllocateMemory(VulkanContext::GetDriver(), &alloc_info, nullptr, &m_VertexBufferDeviceMemory), "vkAllocateMemory", __FILE__, __LINE__, __FUNCTION__);

        vkBindBufferMemory(VulkanContext::GetDriver(), m_VertexBuffer, m_VertexBufferDeviceMemory, 0);

        //! @note Mapping memory data.
        //! @note THIS is how we map our vertices data to our VkBuffer (essentially it is our vertex buffer)
        void* data;
        vkMapMemory(VulkanContext::GetDriver(), m_VertexBufferDeviceMemory, 0, buffer_size, 0, &data);
        memcpy(data, p_Vertices.data(), static_cast<size_t>(buffer_size));
        vkUnmapMemory(VulkanContext::GetDriver(), m_VertexBufferDeviceMemory);


        ConsoleLogWarn("Vulkan2Showcase: Vulkan Model Initialized Completed!!!");

        // initialize_vertex_buffers(p_Vertices);

    }

    void VulkanModel::initialize_vertex_buffers(const std::vector<Vertex>& p_Vertices){

    }

    void VulkanModel::Draw(VkCommandBuffer p_Command){
        vkCmdDraw(p_Command, m_VerticesCount, 1, 0, 0);
    }

    void VulkanModel::Bind(VkCommandBuffer p_Command){
        VkBuffer buffers[] = { m_VertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(p_Command, 0, 1, buffers, offsets);
    }

    std::vector<VkVertexInputBindingDescription> VulkanModel::Vertex::GetVertexInputBindDescription(){
        std::vector<VkVertexInputBindingDescription> binding_descriptions(1);
        binding_descriptions[0] = {
            .binding = 0,
            .stride = sizeof(Vertex),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
        };

        return binding_descriptions;
    }
    
    std::vector<VkVertexInputAttributeDescription> VulkanModel::Vertex::GetVertexAttributeDescriptions(){
        std::vector<VkVertexInputAttributeDescription> attribute_description(2);
        attribute_description[0] = {
            .location = 0, // // layout(location = 0)
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            // .offset = 0
            .offset = offsetof(Vertex, Position)
        };

        attribute_description[1] = {
            .location = 1, // layout(location = 1)
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = offsetof(Vertex, Color)
        };

        return attribute_description;
    }
    
};