#include <drivers/vulkan-cpp/vk_driver.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <span>
#include <core/engine_logger.hpp>
#include <drivers/vulkan/helper_functions.hpp>

namespace atlas::vk {
    [[maybe_unused]] static VkFormat search_supported_depth_format(
      const VkPhysicalDevice& p_physical,
      const std::span<VkFormat>& p_formats,
      VkImageTiling p_tiling,
      VkFormatFeatureFlags p_feature_flag) {
        VkFormat format = VK_FORMAT_UNDEFINED;

        for (size_t i = 0; i < p_formats.size(); i++) {
            VkFormat current_format = p_formats[i];
            VkFormatProperties format_properties;
            vkGetPhysicalDeviceFormatProperties(
              p_physical, current_format, &format_properties);

            if (p_tiling == VK_IMAGE_TILING_LINEAR){
                if(format_properties.linearTilingFeatures & p_feature_flag) {
                    format = current_format;
                }
            }
            else if (p_tiling == VK_IMAGE_TILING_OPTIMAL and
                     format_properties.optimalTilingFeatures & p_feature_flag) {
                format = current_format;
            }
        }

        return format;
    }

    [[maybe_unused]] static VkFormat search_depth_format(const VkPhysicalDevice& p_physical) {
        std::vector<VkFormat> candidate_formats = {
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D24_UNORM_S8_UINT
        };

        VkFormat format = search_supported_depth_format(
          p_physical,
          candidate_formats,
          VK_IMAGE_TILING_OPTIMAL,
          VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
        return format;
    }

    vk_driver* vk_driver::s_instance = nullptr;

    vk_driver::vk_driver(const vk_physical_driver& p_physical) : m_physical(p_physical) {
        console_log_info("vk_driver::vk_driver Begin Initialization!!!");
        m_depth_format_selected = search_depth_format(m_physical);

        float queue_priority[1] = { 0.0f };
        
        std::vector<const char*> device_extension = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        uint32_t graphics_index = m_physical.read_queue_family_indices().graphics;

        console_log_trace("Graphics Queue Indices = {}", graphics_index);

        VkDeviceQueueCreateInfo queue_create_info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = graphics_index,
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
        vkGetPhysicalDeviceFeatures(m_physical, &features);
        features.robustBufferAccess = false;
        create_info.pEnabledFeatures = &features;

        vk_check(vkCreateDevice(m_physical, &create_info, nullptr, &m_driver),
                 "vkCreateDevice",
                 __FILE__, __LINE__, __FUNCTION__);

        vkGetDeviceQueue(
          m_driver, graphics_index, 0, &m_device_queues.graphics_queue);
        
        s_instance = this;

        // vk_context::submit_resource_free([this](){
        //     console_log_trace("vk_driver called by vk_contexgt::submit_resource_free!!!");
        //     vkDestroyDevice(m_driver, nullptr);
        // });

        console_log_info("vk_driver::vk_driver end initialization!!!\n\n");
    }

    //! @note Returns -1 if there are no flags available/compatible/valid
    uint32_t vk_driver::select_memory_type(uint32_t p_type_filter,
                                VkMemoryPropertyFlags p_property_flag) {
        VkPhysicalDeviceMemoryProperties mem_props;
        vkGetPhysicalDeviceMemoryProperties(m_physical, &mem_props);

        for (uint32_t i = 0; i < mem_props.memoryTypeCount; i++) {
            if ((p_type_filter & (1 << i)) and
                (mem_props.memoryTypes[i].propertyFlags & p_property_flag) ==
                  p_property_flag) {
                return i;
            }
        }

        return -1;
    }

    VkQueue vk_driver::get_queue(const vk_queue_options& p_present_queue) {
        VkQueue queue_handler=nullptr;
        vkGetDeviceQueue(m_driver,
                         p_present_queue.family_index,
                         p_present_queue.queue_index,
                         &queue_handler);

        return queue_handler;
    }
    
    VkFormat vk_driver::depth_format() const {
        return m_depth_format_selected;
    }

    vk_driver::~vk_driver() {
        console_log_warn("~vk_driver is called!!!");
        // vkDestroyDevice(m_driver, nullptr);
    }
};