#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace engine3d{
    namespace vk{
        struct PhysicalDeviceAttribute{
            VkPhysicalDevice device; // usage is to read/write or creating our actual physical object representation of our physical device
            VkPhysicalDeviceProperties deviceProperties; // which is a structure that represent the properties about our GPU
            std::vector<VkQueueFamilyProperties> queueFamProperties; // for each device there might be different queue families properties
            std::vector<uint32_t> queueSupportPresent; // different queue support that are present
            std::vector<VkSurfaceFormatKHR> surfaceFormats; // different formats such as pixels and color spaces
            VkSurfaceCapabilitiesKHR surfaceCapabilities;
            VkPhysicalDeviceMemoryProperties memoryProperties;
            std::vector<VkPresentModeKHR> presentModes;
            VkPhysicalDeviceFeatures phsicalDeviceFeatures;
        };

        /**
         * @name VulkanPhysicalDevice
         * @note Abstraction of how we interact with our current physical device
         * @note Checks for driver compatibilities, device suitability checks, and available queue families
         * 
         * @note Used to expose what queue families are presented on your specific GPU.
         * @note In vulkan you submid draw instructions as command buffers in queues
         * @note Queues have queue families that represent different workloads the physical device exposes to you (the application developer)
         * 
         * 
         * @name Requirements that would be looked for within on our current physical device hardware (GPU)
         * @name Presentation Mode Topic
         * @note Mentioned more when doing swap chains
         * @note Typically in OpenGL handles double buffering by doing glfwSwapBuffers
         * @note Double-buffering is basically having a back buffer for updating and the other buffer for displaying to the screen.
         * @note Initially is how its done when you render scenes to the screen.
        */
        class VulkanPhysicalDevice{
        public:
            void InitializePhysicalDevice();
            void CleanupPhysicalDevice();
            //! @note Choosing if there are multiple devices
            //! @note Function is used to also check if our current device supports presenting.
            //! @note Something to consider is based on num of queues, size of memory, or present all devices to devs from a GUI. 
            uint32_t SelectDevice(VkQueueFlags ReqQueueFlag_t, bool IsSupportPresent);

            static VkSurfaceCapabilitiesKHR GetSurfaceCapabilities();
            static std::vector<VkPresentModeKHR> GetPresentationModes();
            static std::vector<VkSurfaceFormatKHR> GetSurfaceFormats();
            VkPhysicalDevice Selected();
            VkBool32 IsGeometryShaderSupported();
            VkBool32 IsTesselationSupported();

            VkSurfaceCapabilitiesKHR SurfaceCapabilities();
            std::vector<VkSurfaceFormatKHR> SurfaceFormats();
            std::vector<VkPresentModeKHR> PresentationModes();

        private:
            // VkPhysicalDevice Selected();
            PhysicalDeviceAttribute SelectedDevice();
        private:
            std::vector<PhysicalDeviceAttribute> m_PhysicalDevices;
            int m_DeviceIdx = 0; // index to selected device
        };

        /**
         * @name Vulkan Logical Device
         * @note [instance] -> [physical device] -> [logical device]
         * @note logical device in Vulkan means we do not interact to the physical hardware device,but the driver.
         * @note 
        */
        class VulkanLogicalDevice{
        public:
            void InitializeLogicalDevice();
            void CleanupLogicalDevice();

            VkDevice LogicalDeviceInstance();

            //! @note Value that stores index to the queue family
            uint32_t& QueueFamily();
        private:
            uint32_t m_queueFamily;
            VkDevice m_Device;
        };

        /**
         * @name VulkanDevice
         * @note Represent our abstraction layers for physical and logical devices that are dealt within Vulkan
        */
        class VulkanDevice{
        public:
            //! @note Initiating our devices through Vulkan that are available
            void InitializeDevice();

            //! @note Cleaning up making sure things get deallocated cleanly (if there are any)
            void CleanupDevice();

            static VkPhysicalDevice GetVkPhysicalDeviceInstance();
            static VkDevice GetVkLogicalDeviceInstance();
            static VulkanPhysicalDevice GetPhysicalDevice();
            static VulkanLogicalDevice GetLogicalDevice();
        };
    };
};