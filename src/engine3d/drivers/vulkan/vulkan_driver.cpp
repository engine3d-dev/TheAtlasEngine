#include <drivers/vulkan/vulkan_driver.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <core/engine_logger.hpp>

namespace engine3d::vk{
    VulkanDriver::VulkanDriver(VulkanPhysicalDriver p_PhysicalDevice){
        ConsoleLogInfo("Vulkan2Showcase: Begin Vulkan Driver Initialization!");

        float queue_priority[1] = { 0.0f };
        
        std::vector<const char*> device_extension = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        VkDeviceQueueCreateInfo queue_create_info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = p_PhysicalDevice.GetQueueIndices().Graphics,
            .queueCount = 1,
            .pQueuePriorities = queue_priority,
        };

        VkDeviceCreateInfo create_info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &queue_create_info,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = static_cast<uint32_t>(device_extension.size()),
            .ppEnabledExtensionNames = device_extension.data(),
        };

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(p_PhysicalDevice, &features);
        features.robustBufferAccess = false;
        create_info.pEnabledFeatures = &features;

        vk_check(vkCreateDevice(p_PhysicalDevice, &create_info, nullptr, &m_CurrentDriver), "vkCreateDevice", __FILE__, __LINE__, __FUNCTION__);
        ConsoleLogWarn("Vulkan2Showcase: Vulkan Driver Initialized Complete!!!");

        //! @note Initializes our queue for graphics (to render basic primitives)
        ConsoleLogInfo("Vulkan2Showcase: In Vulkan Driver, Initializes Queue");

        /*
        //! @note TODO: This should be in VulkanSwapchain
        if( p_PhysicalDevice.GetQueueIndices().Graphics == m_PresentationIndex){
            m_PresentationIndex = p_PhysicalDevice.GetQueueIndices().Graphics;
        }
        else{
            vkGetDeviceQueue(m_, 0, CurrentDriver, m_PresentationIndex, &m_PresentationQueue);
        }
        */

        vkGetDeviceQueue(m_CurrentDriver, p_PhysicalDevice.GetQueueIndices().Graphics, 0, &m_GraphicsQueue);
    }
};