#include <core/engine_logger.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <drivers/vulkan/vulkan_driver.hpp>
#include <vulkan/vulkan_core.h>
#include <drivers/vulkan/vulkan_context.hpp>

namespace atlas::vk {
    vk_driver::vk_driver(const vk_physical_driver& p_physical) {
        // console_log_info("Vulkan2Showcase: Begin Vulkan Driver
        // Initialization!");
        console_log_warn_tagged("vulkan",
                                "Begin Vulkan Driver Initialization!");

        float queue_priority[1] = { 0.0f };
        /*

            VK_EXT_descriptor_buffer
                - Simplifies and maps more directly to how hardware sees
           descriptors
                - Simplifies the programming model, by not needing to create
           descriptor pools up front
        */
        std::vector<const char*> device_extension = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_EXT_descriptor_buffer"
        };

        VkDeviceQueueCreateInfo queue_create_info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = p_physical.get_queue_indices().Graphics,
            .queueCount = 1,
            .pQueuePriorities = queue_priority,
        };

        VkDeviceCreateInfo create_info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &queue_create_info,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount =
              static_cast<uint32_t>(device_extension.size()),
            .ppEnabledExtensionNames = device_extension.data(),
        };

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(p_physical, &features);
        features.robustBufferAccess = false;
        create_info.pEnabledFeatures = &features;

        vk_check(vkCreateDevice(p_physical, &create_info, nullptr, &m_driver),
                 "vkCreateDevice",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);
        // console_log_warn("Vulkan2Showcase: Vulkan Driver Initialized
        // Complete!!!");
        console_log_warn_tagged("vulkan",
                                "Vulkan Driver Initialized Complete!!!");

        //! @note Initializes our queue for graphics (to render basic
        //! primitives)
        // console_log_info("Vulkan2Showcase: In Vulkan Driver, Initializes
        // Queue");
        console_log_warn_tagged("vulkan",
                                "In Vulkan Driver, Initializes Queue");

        /*
        //! @note TODO: This should be in VulkanSwapchain
        if( p_PhysicalDevice.get_queue_indices().Graphics ==
        m_PresentationIndex){ m_PresentationIndex =
        p_PhysicalDevice.get_queue_indices().Graphics;
        }
        else{
            vkGetDeviceQueue(m_, 0, CurrentDriver, m_PresentationIndex,
        &m_PresentationQueue);
        }
        */

        vkGetDeviceQueue(m_driver,
                         p_physical.get_queue_indices().Graphics,
                         0,
                         &m_graphics_queue);
        vk_context::submit_context_free([this](){
            vkDestroyDevice(m_driver, nullptr);
        });
    }

    uint32_t vk_driver::select_memory_type(
      uint32_t p_type_filter,
      VkMemoryPropertyFlags p_property_flag) {
        VkPhysicalDeviceMemoryProperties mem_props;
        vkGetPhysicalDeviceMemoryProperties(
          vk_context::current_physical_driver(), &mem_props);

        for (uint32_t i = 0; i < mem_props.memoryTypeCount; i++) {
            if ((p_type_filter & (1 << i)) and
                (mem_props.memoryTypes[i].propertyFlags & p_property_flag) ==
                  p_property_flag) {
                return i;
            }
        }

        return -1;
    }
};