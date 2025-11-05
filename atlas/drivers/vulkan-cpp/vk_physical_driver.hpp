#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace atlas::vk {

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
    class vk_physical_driver {
        struct queue_family_indices {
            uint32_t graphics = -1;
            uint32_t compute = -1;
            uint32_t transfer = -1;
        };

    public:
        vk_physical_driver() = default;

        /**
         * @brief constructs a new vulkan physical device
         *
         * @param p_instance requires a VkInstance to create a VkPhysicalDevice
         * handle
         */
        vk_physical_driver(const VkInstance& p_instance);

        ~vk_physical_driver();

        /**
         * @brief gives you the queue families that are supported
         *
         * @return queue_family_indices are the indices of the specific queue's
         * that are compatible on current hardware specifications
         */
        [[nodiscard]] queue_family_indices read_queue_family_indices() const {
            return m_queue_indices;
        }

        [[nodiscard]] VkPhysicalDeviceMemoryProperties memory_properties()
          const;

        /**
         * @return uint32_t is the index to the presentation index of the
         * specific presentation queue
         */
        [[nodiscard]] uint32_t read_presentation_index(
          const VkSurfaceKHR& p_surface);

        /**
         * @brief querying surface properties based on the currently specified
         * VkSurfaceKHR handle created
         */
        [[nodiscard]] surface_properties get_surface_properties(
          const VkSurfaceKHR& p_surface);

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
        queue_family_indices select_queue_family_indices();

    private:
        VkPhysicalDevice m_physical_driver = nullptr;
        queue_family_indices m_queue_indices{};
        std::vector<VkQueueFamilyProperties> m_queue_family_properties{};
        surface_properties m_surface_properties{};
    };
};