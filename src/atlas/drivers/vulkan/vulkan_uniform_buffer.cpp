#include "drivers/vulkan/helper_functions.hpp"
#include "drivers/vulkan/vulkan_context.hpp"
#include <drivers/vulkan/vulkan_uniform_buffer.hpp>
#include <vulkan/vulkan_core.h>

namespace atlas::vk{
    // VulkanUniformBuffer::VulkanUniformBuffer(uint32_t p_Size, uint32_t p_InstanceCount){
    //     //! @note Creating staging buffer
    //     VkBuffer staging_buffer;
    //     VkBufferCreateInfo staging_buffer_ci = {
    //         .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
    //         .size = p_Size,
    //         .usage = 
    //     };
    // }

    void VulkanUniformBuffer::AllocateUniforms(){}

    // void VulkanUniformBuffer::MapWrite(uint32_t p_DeviceSize, uint32_t p_Offset, const void* p_Src){
    // }
};