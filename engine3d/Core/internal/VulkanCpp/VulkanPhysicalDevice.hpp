#pragma once
#include <string>
#include <unordered_set>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    namespace vk{
        /*
         * @name VulkanPhysicalDevice
         * @note Abstracted physical device layer for validating compatbility with our current platform's GPU.
         * @note In other words represent actual physical GPU that is available on our current machine.
         * @note Allows with workloads from vulkan logical devices and exposing to various queue families the GPU supports.
        */
        class VulkanPhysicalDevice{
            struct QueueFamilyIndices{
                int Graphics = -1;
                int Compute = -1;
                int Transfer = -1;
            };
        public:
            VulkanPhysicalDevice();

            VkPhysicalDevice& GetVkPhysicalDevice();

            /*
             * @name GetDepthFormat()
             * @note Returns the depth format we need for image depth usage.
             * @note By returning the depth format we also describe how our memory is laid out.
            */
            VkFormat GetDepthFormat();
            
            //! @note Returns boolean if we pass a valid extension name.
            bool IsExtensionSupported(const std::string& ext_name);
        private:
            //! @note Returns the queue family indices to which queue family indices are supported and valid.
            VulkanPhysicalDevice::QueueFamilyIndices GetQueueFamilyIndices(int flag);
            
            //! @note Used for validating if depth format is available or valid on our current machine.
            VkFormat SearchAvailableDepthFormat();
            
            //! @note print debugging information of our physical device properties.
            void print_physical_device_property(VkPhysicalDeviceProperties properties);

            //! @note Fetching our devices that are compatible to use.
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
