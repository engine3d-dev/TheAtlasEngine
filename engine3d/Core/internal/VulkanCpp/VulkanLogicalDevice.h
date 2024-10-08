#pragma once
#include <internal/VulkanCpp/VulkanPhysicalDevice.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    namespace vk{
        class VulkanLogicalDevice{
        public:
            VulkanLogicalDevice() {}
            VulkanLogicalDevice(VulkanPhysicalDevice& p_PhysicalDevice);

            VkDevice GetVkDeviceInstance();
            VulkanPhysicalDevice& GetPhysicalDevice();
            VkPhysicalDeviceFeatures GetVkPhysicalDeviceFeatures();

            VkQueue GetVkGraphicsQueue();
            VkQueue GetVkComputeQueue();

        private:
            VkDevice m_LogicalDevice = nullptr;
            VulkanPhysicalDevice m_PhysicalDevice;
            VkPhysicalDeviceFeatures m_EnabledPhysicalFeatures;

            //! @note Allocator stuff.

            VkQueue m_GraphicsQueue;
            VkQueue m_ComputeQueue;
        };
    };
};