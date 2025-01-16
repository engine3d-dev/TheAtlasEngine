#include "drivers/vulkan/helper_functions.hpp"
#include "drivers/vulkan/vulkan_context.hpp"
#include <drivers/vulkan/vulkan_uniform_buffer.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    VulkanUniformBuffer::VulkanUniformBuffer(uint32_t p_Size){
        //! @note We specify our uniform buffer and creating our buffer object handler
        VkBufferCreateInfo uniform_buffer_create_info = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = p_Size,
            .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices = nullptr,
        };

        vk_check(vkCreateBuffer(VulkanContext::GetDriver(), &uniform_buffer_create_info, nullptr, &m_BufferObject), "vkCreateBuffer", __FILE__, __LINE__, __FUNCTION__);

        VkMemoryRequirements memory_requirements;
        vkGetBufferMemoryRequirements(VulkanContext::GetDriver(), m_BufferObject, &memory_requirements);

        // VkMemoryAllocateInfo memory_allocate_info = {
        //     .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        //     .pNext = nullptr,
        //     .allocationSize = memory_requirements.size,
        //     .memoryTypeIndex = 0,
        // };

        // bool pass = VulkanContext::GetDriver().SelectMemoryType()
    }
};