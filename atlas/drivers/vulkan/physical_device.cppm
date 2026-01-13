module;

#include <cstdint>
#include <vector>

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

export module atlas.drivers.vulkan.physical_device;

import atlas.logger;
import atlas.drivers.vulkan.utilities;

export namespace atlas::vulkan {
    struct surface_properties {
        VkSurfaceCapabilitiesKHR surface_capabilities;
        VkSurfaceFormatKHR surface_format;
    };

    /**
     * @brief vulkan-specific implementation wrapper around VkPhysicalDevice
     *
     * Wrapper that constructs a single physical device that provides API's that
     * can be used to query specific information for your specific physical
     * device
     *
     * A physical device represents your current hardware GPU and allows for the
     * ability to enumerate information to check for compatibility on the
     * current GPU
     */
    class physical_device {
        struct queue_family_indices {
            uint32_t graphics = -1;
            uint32_t compute = -1;
            uint32_t transfer = -1;
        };

    public:
        physical_device() = default;

        /**
         * @brief constructs a new vulkan physical device
         *
         * @param p_instance requires a VkInstance to create a VkPhysicalDevice
         * handle
         */
        physical_device(const VkInstance& p_instance) {
            uint32_t device_count = 0;
            vkEnumeratePhysicalDevices(p_instance, &device_count, nullptr);

            if (device_count == 0) {
                console_log_fatal("Device Count is {} and no devices found!!!",
                                device_count);
                return;
            }

            std::vector<VkPhysicalDevice> physical_drivers(device_count);
            vkEnumeratePhysicalDevices(
            p_instance, &device_count, physical_drivers.data());

            for (const auto& device : physical_drivers) {
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
            vkGetPhysicalDeviceQueueFamilyProperties(
            m_physical_driver, &queue_family_count, nullptr);
            m_queue_family_properties.resize(queue_family_count);

            vkGetPhysicalDeviceQueueFamilyProperties(
            m_physical_driver,
            &queue_family_count,
            m_queue_family_properties.data());

            m_queue_indices = select_queue_family_indices();
        }

        ~physical_device() = default;

        /**
         * @brief gives you the queue families that are supported
         *
         * @return queue_family_indices are the indices of the specific queue's
         * that are compatible on current hardware specifications
         */
        [[nodiscard]] queue_family_indices read_queue_family_indices() const {
            return m_queue_indices;
        }

        [[nodiscard]] VkPhysicalDeviceMemoryProperties memory_properties() const {
            VkPhysicalDeviceMemoryProperties physical_memory_properties;
            vkGetPhysicalDeviceMemoryProperties(m_physical_driver,
                                                &physical_memory_properties);
            return physical_memory_properties;
        }

        /**
         * @return uint32_t is the index to the presentation index of the
         * specific presentation queue
         */
        [[nodiscard]] uint32_t read_presentation_index(const VkSurfaceKHR& p_surface) {
            uint32_t presentation_index = -1;
            VkBool32 compatible = VK_FALSE;
            uint32_t i = 0;
            for (const auto& queue_family : m_queue_family_properties) {
                if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    vk_check(vkGetPhysicalDeviceSurfaceSupportKHR(
                            m_physical_driver, i, p_surface, &compatible),
                            "vkGetPhysicalDeviceSurfaceSupportKHR");

                    if (compatible) {
                        presentation_index = i;
                    }
                }
                i++;
            }

            return presentation_index;
        }

        /**
         * @brief querying surface properties based on the currently specified
         * VkSurfaceKHR handle created
         */
        [[nodiscard]] surface_properties get_surface_properties(const VkSurfaceKHR& p_surface) {
            vk_check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
                   m_physical_driver,
                   p_surface,
                   &m_surface_properties.surface_capabilities),
                 "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");

            uint32_t format_count = 0;
            std::vector<VkSurfaceFormatKHR> formats;
            vk_check(vkGetPhysicalDeviceSurfaceFormatsKHR(
                    m_physical_driver, p_surface, &format_count, nullptr),
                    "vkGetPhysicalDeviceSurfaceFormatsKHR");

            formats.resize(format_count);

            vk_check(vkGetPhysicalDeviceSurfaceFormatsKHR(
                    m_physical_driver, p_surface, &format_count, formats.data()),
                    "vkGetPhysicalDeviceSurfaceFormatsKHR");

            for (const auto& format : formats) {
                if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
                    format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                    m_surface_properties.surface_format = format;
                }
            }

            m_surface_properties.surface_format = formats[0];

            return m_surface_properties;
        }

        /**
         * @brief Allows for treating vk_physical_device as a VkPhysicalDevice
         * handle
         *
         * Simplifies using this same class for creating other vulkan
         * metaobjects
         */
        operator VkPhysicalDevice() { return m_physical_driver; }

        /**
         * @brief Allows for treating vk_physical_device as a VkPhysicalDevice
         * handle
         *
         * Simplifies using this same class for creating other vulkan
         * metaobjects
         */
        operator VkPhysicalDevice() const { return m_physical_driver; }

    private:
        queue_family_indices select_queue_family_indices() {
            VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
            vkGetPhysicalDeviceMemoryProperties(m_physical_driver,
                                                &physical_device_memory_properties);
            physical_device::queue_family_indices indices;
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

    private:
        VkPhysicalDevice m_physical_driver = nullptr;
        queue_family_indices m_queue_indices{};
        std::vector<VkQueueFamilyProperties> m_queue_family_properties{};
        surface_properties m_surface_properties{};
    };
};