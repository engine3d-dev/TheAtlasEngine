#include <drivers/vertex_buffer.hpp>
#include <drivers/vulkan/vulkan_vertex_buffer.hpp>
#include <core/engine_logger.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <drivers/vulkan/helper_functions.hpp>

namespace atlas::vk{
    vk_vertex_buffer::vk_vertex_buffer(const std::vector<vertex>& p_vertices){
        console_log_warn_tagged("vulkan", "Begin Vulkan vertex_buffer Initialization!!");
        m_count = static_cast<uint32_t>(p_vertices.size());
        VkDeviceSize buffer_size = sizeof(p_vertices[0]) * m_count;
        VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        VkMemoryPropertyFlags property_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        
        VkBufferCreateInfo vk_buffer_create_info = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .size = buffer_size,
            .usage = usage_flags,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        vk_check(vkCreateBuffer(vk_context::get_current_driver(), &vk_buffer_create_info, nullptr, &m_vertex_buffer_handler), "vkCreateBuffer", __FILE__, __LINE__, __FUNCTION__);

        VkMemoryRequirements memory_reqs;
        vkGetBufferMemoryRequirements(vk_context::get_current_driver(), m_vertex_buffer_handler, &memory_reqs);

        VkMemoryAllocateInfo alloc_info = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memory_reqs.size,
            .memoryTypeIndex = vk_context::get_current_driver().select_memory_type(memory_reqs.memoryTypeBits, property_flags)
        };

        vk_check(vkAllocateMemory(vk_context::get_current_driver(), &alloc_info, nullptr, &m_vertex_buffer_device_memory), "vkAllocateMemory", __FILE__, __LINE__, __FUNCTION__);

        vkBindBufferMemory(vk_context::get_current_driver(), m_vertex_buffer_handler, m_vertex_buffer_device_memory, 0);

        //! @note Mapping memory data.
        //! @note THIS is how we map our vertices data to our VkBuffer (essentially it is our vertex buffer)
        void* data;
        vkMapMemory(vk_context::get_current_driver(), m_vertex_buffer_device_memory, 0, buffer_size, 0, &data);
        memcpy(data, p_vertices.data(), static_cast<size_t>(buffer_size));
        vkUnmapMemory(vk_context::get_current_driver(), m_vertex_buffer_device_memory);
        // console_log_warn("Vulkan2Showcase: Vulkan vertex_buffer Initialized Completed!!!");
        console_log_warn_tagged("vulkan", "Vulkan vertex_buffer Initialized Completed!!!");
    }

    void vk_vertex_buffer::bind_vertex_buffer(const VkCommandBuffer& p_command_buffer) {
        VkBuffer buffers[] = {m_vertex_buffer_handler};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(p_command_buffer, 0, 1, buffers, offsets);
    }
        
    void vk_vertex_buffer::render_vertex_buffer(const VkCommandBuffer& p_command_buffer) {
        vkCmdDraw(p_command_buffer, m_count, 1, 0, 0);
    }
};