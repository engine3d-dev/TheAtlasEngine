#pragma once
#include <internal/VulkanCpp/VulkanPhysicalDevice.hpp>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    namespace vk{
        /*
         * @name VulkanLogicalDevice
         * @note Represent our vulkan interface to the physical device.
         * @note Using this logical device allows for multiple application accessing the physical devices on our machine.
         * @note Utilizing our selected physical device (VulkanPhysicalDevice), using this logical device to specify features/extensions with our selected physical device.
        */
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
