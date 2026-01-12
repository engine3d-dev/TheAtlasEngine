module;

#include <cstdint>

#define GLFW_INCLUDE_VULKAN
#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#else
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#endif

#include <span>
#include <vector>

export module atlas.drivers.vulkan.device;

import atlas.logger;

import atlas.drivers.vulkan.utilities;
import atlas.drivers.vulkan.physical_device;

namespace atlas::vulkan {
    static VkFormat search_supported_depth_format(
      const VkPhysicalDevice& p_physical,
      std::span<VkFormat> p_formats,
      VkImageTiling p_tiling,
      VkFormatFeatureFlags p_feature_flag) {
        VkFormat format = VK_FORMAT_UNDEFINED;

        for (size_t i = 0; i < p_formats.size(); i++) {
            VkFormat current_format = p_formats[i];
            VkFormatProperties format_properties;
            vkGetPhysicalDeviceFormatProperties(
              p_physical, current_format, &format_properties);

            if (p_tiling == VK_IMAGE_TILING_LINEAR) {
                if (format_properties.linearTilingFeatures & p_feature_flag) {
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

    static VkFormat search_depth_format(const VkPhysicalDevice& p_physical) {
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

    /**
     * @brief logical device implementation wrapper around the VkDevice
     *
     * This class was a wrapper around VKDevice, that provided you with other
     * API's to do with that particular logical device.
     *
     * Logical devices are representation of virtual software ways to interact
     * with the GPU through Vulkan specifications
     *
     * TODO: Out-of-date. Using vulkan-cpp ::vk::device class to provide API's
     * that allow you to do more queries on specific attributes the logical
     * device gives you.
     */
    export class device {
        struct device_queue_family {
            VkQueue graphics_queue;
            VkQueue transfer_queue;
            VkQueue compute_queue;
        };

    public:
        device() = default;

        /**
         * @brief construct a new logical device
         * @param p_physical is the physical device required for the creation of
         * the logical device
         */
        device(const physical_device& p_physical) : m_physical(p_physical) {
            m_depth_format_selected = search_depth_format(m_physical);

            float queue_priority[1] = { 0.0f };

            std::vector<const char*> device_extension = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };

            uint32_t graphics_index =
            m_physical.read_queue_family_indices().graphics;

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
                    "vkCreateDevice");

            vkGetDeviceQueue(
            m_driver, graphics_index, 0, &m_device_queues.graphics_queue);

            console_log_warn("vulkan::device constructed successfully!!!");
        }

        // Use .destroy to explicitly invoke when to do proper vulkan cleanup
        ~device() = default;

        /**
         * @brief returns the specified graphics queue from this logical device
         */
        [[nodiscard]] VkQueue graphics_queue() const {
            return m_device_queues.graphics_queue;
        }

        /**
         * @return -1 if there are no flags available/compatible/valid
         */
        uint32_t select_memory_type(uint32_t p_type_filter, VkMemoryPropertyFlags p_property_flag) {
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

        void destroy() {
            vkDeviceWaitIdle(m_driver);
            vkDestroyDevice(m_driver, nullptr);
        }

        /**
         * @brief gives you the depth format from the logical device
         *
         * @return VkFormat
         */
        [[nodiscard]] VkFormat depth_format() const {
            return m_depth_format_selected;
        }

        /**
         * @brief allows to treat vk_driver as a VkDevice handle
         *
         * For vulkan API's that accept only taking in VKDevice, this simplifies
         * the need to not have a getter API
         */
        operator VkDevice() const { return m_driver; }

        /**
         * @brief allows to treat vk_driver as a VkDevice handle
         *
         * For vulkan API's that accept only taking in VKDevice, this simplifies
         * the need to not have a getter API
         */
        operator VkDevice() { return m_driver; }

    private:
        physical_device m_physical{};
        VkDevice m_driver = nullptr;
        device_queue_family m_device_queues{};
        VkFormat m_depth_format_selected;
    };
};