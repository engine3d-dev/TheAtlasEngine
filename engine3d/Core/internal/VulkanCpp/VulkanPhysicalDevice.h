#pragma once
#include <string>
#include <unordered_set>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    namespace vk{
        class VulkanPhysicalDevice{
            struct QueueFamilyIndices{
                int Graphics = -1;
                int Compute = -1;
                int Transfer = -1;
            };
        public:
            VulkanPhysicalDevice();

            VkPhysicalDevice& GetVkPhysicalDevice();
            VkFormat GetDepthFormat();

            bool IsExtensionSupported(const std::string& ext_name);
        private:
            VulkanPhysicalDevice::QueueFamilyIndices GetQueueFamilyIndices(int flag);

            VkFormat SearchAvailableDepthFormat();

            void print_physical_device_property(VkPhysicalDeviceProperties properties);
            std::vector<VkPhysicalDevice> get_available_devices();


        private:
            VkPhysicalDevice m_PhysicalDeviceHandler = nullptr; // actual physical device handler
            VkPhysicalDevice m_SelectedPhysicalDevice = nullptr;
            VkPhysicalDeviceProperties m_PhysicalDeviceProperties; // provides specications about our physical device.
            VkPhysicalDeviceFeatures m_PhysicalDeviceFeatures; // enabling fine-grained features for buffer-level access
            VkPhysicalDeviceMemoryProperties m_PhysicalMemoryProperties; // enable to check memory heaps that are accessible
            QueueFamilyIndices m_QueueIndices; // Indices that can be request by logical devices that vary from graphics, compute, and transfer.
            VkFormat m_DepthFormat = VK_FORMAT_UNDEFINED; // To validate whether depth format is supported or available from our physical devices.

            // queue family properties
            std::vector<VkQueueFamilyProperties> m_QueueFamilyProperties;
            std::vector<VkDeviceQueueCreateInfo> m_DeviceQueueCreateInfos;

            std::unordered_set<std::string> m_SupportedExtensions;
        };
    };
};