#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace engine3d{
    /**
    @name VulkanDevice
    @note Abstracting how we handle physical, logical devices and queue familties
    @note Checks for driver compatibilities, device suitability checks, and available queue families
    */
    namespace vk{
        struct PhysicalDevice{
            VkPhysicalDevice device; // usage is to read/write or creating our actual physical object representation of our physical device
            VkPhysicalDeviceProperties deviceProperties; // which is a structure that represent the properties about our GPU
            std::vector<VkQueueFamilyProperties> queueFamProperties; // for each device there might be different queue families properties
            std::vector<uint32_t> queueSupportPresent; // different queue support that are present
            std::vector<VkSurfaceFormatKHR> surfaceFormats; // different formats such as pixels and color spaces
            VkSurfaceCapabilitiesKHR surfaceCapabilities;
            VkPhysicalDeviceMemoryProperties memoryProperties;
            std::vector<VkPresentModeKHR> presentModes;
        };

        /**
         * @name VulkanPhysicalDevice
         * @note Used to expose what queue families are presented on your specific GPU.
         * @note In vulkan you submid draw instructions as command buffers in queues
         * @note Queues have queue families that represent different workloads the physical device exposes to you (the application developer)
         * @note TODO -- having a way of knowing how many queues and queue families are in our system, and features they have.
         * 
         * 
         * @note Presentation Modes (and diff types)
         * @name Presentation Mode Topic
         * @note Mentioned more when doing swap chains
         * @note Typically in OpenGL handles double buffering by doing glfwSwapBuffers
         * @note Double-buffering is basically having a back buffer for updating and the other buffer for displaying to the screen.
         * @note Initially is how its done when you render scenes to the screen.
        */
        class VulkanPhysicalDevice{
        public:

        private:
        };

        class VulkanLogicalDevice{};
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

            //! @note Choosing if there are multiple devices
            //! @note Function is used to also check if our current device supports presenting.
            //! @note Something to consider is based on num of queues, size of memory, or present all devices to devs from a GUI. 
            uint32_t SelectDevice(VkQueueFlags ReqQueueFlag_t, bool IsSupportPresent);
            
            //! @note Selected is the device that we want to use for engine3d.
            const PhysicalDevice Selected();
        private:
            std::vector<PhysicalDevice> m_PhysicalDevices;
            int m_DeviceIdx = 0; // index to selected device
        };
    };
};