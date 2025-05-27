#include <drivers/vulkan-cpp/vk_physical_driver.hpp>
#include <core/engine_logger.hpp>
#include <drivers/vulkan/helper_functions.hpp>

namespace atlas::vk {

    vk_physical_driver* vk_physical_driver::s_instance = nullptr;

    vk_physical_driver::vk_physical_driver(const VkInstance& p_instance) {
        console_log_info("vk_physical_driver::vk_physical_driver Begin Initialization!!!");

        uint32_t device_count=0;
        vkEnumeratePhysicalDevices(p_instance, &device_count, nullptr);

        if(device_count == 0) {
            console_log_fatal("Device Count is {} and no devices found!!!", device_count);
            return;
        }

        std::vector<VkPhysicalDevice> physical_drivers(device_count);
        vkEnumeratePhysicalDevices(p_instance, &device_count, physical_drivers.data());

        for(const auto& device : physical_drivers) {
            VkPhysicalDeviceProperties device_properties;
            VkPhysicalDeviceFeatures device_features;
            vkGetPhysicalDeviceProperties(device, &device_properties);
            vkGetPhysicalDeviceFeatures(device, &device_features);
            if (device_properties.deviceType ==
                VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                m_physical_driver = device;
                break;
            }
        }

        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_physical_driver, &queue_family_count, nullptr);
        m_queue_family_properties.resize(queue_family_count);

        vkGetPhysicalDeviceQueueFamilyProperties(m_physical_driver, &queue_family_count, m_queue_family_properties.data());

        m_queue_indices = select_queue_family_indices();

        s_instance = this;
        console_log_info("vk_physical_driver::vk_physical_driver End Initialization!!!\n\n");
    }

    vk_physical_driver::~vk_physical_driver() = default;

    surface_properties vk_physical_driver::get_surface_properties(const VkSurfaceKHR& p_surface) {
        vk_check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
                   m_physical_driver,
                   p_surface,
                   &m_surface_properties.surface_capabilities),
                 "vkGetPhysicalDeviceSurfaceCapabilitiesKHR",
                 __FILE__, __LINE__, __FUNCTION__);

        uint32_t format_count = 0;
        std::vector<VkSurfaceFormatKHR> formats;
        vk_check(vkGetPhysicalDeviceSurfaceFormatsKHR(
                   m_physical_driver, p_surface, &format_count, nullptr),
                 "vkGetPhysicalDeviceSurfaceFormatsKHR",
                 __FILE__, __LINE__, __FUNCTION__);

        formats.resize(format_count);

        vk_check(vkGetPhysicalDeviceSurfaceFormatsKHR(
                   m_physical_driver, p_surface, &format_count, formats.data()),
                 "vkGetPhysicalDeviceSurfaceFormatsKHR",
                 __FILE__, __LINE__, __FUNCTION__);

        for (const auto& format : formats) {
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
                format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                m_surface_properties.surface_format = format;
            }
        }

        m_surface_properties.surface_format = formats[0];

        return m_surface_properties;
    }

    vk_physical_driver::queue_family_indices vk_physical_driver::select_queue_family_indices() {
        VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
        vkGetPhysicalDeviceMemoryProperties(m_physical_driver,
                                            &physical_device_memory_properties);
        vk_physical_driver::queue_family_indices indices;
        int i = 0;

        for (const auto& queue_family : m_queue_family_properties) {
            if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics = i;
                break;
            }
            i++;
        }

        return indices;
    }

    uint32_t vk_physical_driver::read_presentation_index(const VkSurfaceKHR& p_surface) {
        uint32_t presentation_index = -1;
        VkBool32 compatible = VK_FALSE;
        uint32_t i = 0;
        for (const auto& queue_family : m_queue_family_properties) {
            if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                vk_check(vkGetPhysicalDeviceSurfaceSupportKHR(
                           m_physical_driver, i, p_surface, &compatible),
                         "vkGetPhysicalDeviceSurfaceSupportKHR",
                         __FILE__, __LINE__, __FUNCTION__);

                if (compatible) {
                    presentation_index = i;
                }
            }
            i++;
        }

        return presentation_index;
    }
};