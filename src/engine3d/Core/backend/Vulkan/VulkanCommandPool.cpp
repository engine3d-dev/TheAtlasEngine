#include <Core/backend/Vulkan/VulkanCommandPool.h>
#include <Core/backend/Vulkan/VulkanDevice.h>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{

    static VkCommandPool g_CommandPool;

    void VulkanCommandPool::InitializeCommandPool(){
        VkCommandPoolCreateInfo commandPoolInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = VulkanDevice::GetLogicalDevice().QueueFamily()
        };

        VkResult res = vkCreateCommandPool(VulkanDevice::GetVkLogicalDeviceInstance(), &commandPoolInfo, nullptr, &g_CommandPool);
    }

    VkCommandPool& VulkanCommandPool::GetVkCommandPoolInstance(){ return g_CommandPool; }

};