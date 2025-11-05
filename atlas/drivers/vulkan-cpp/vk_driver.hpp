#pragma once
#include <vulkan/vulkan.h>
#include <drivers/vulkan-cpp/vk_physical_driver.hpp>
#include <drivers/vulkan-cpp/vk_types.hpp>

namespace atlas::vk {

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
    class vk_driver {
        struct device_queue_family {
            VkQueue graphics_queue;
            VkQueue transfer_queue;
            VkQueue compute_queue;
        };

    public:
        vk_driver() = default;

        /**
         * @brief construct a new logical device
         * @param p_physical is the physical device required for the creation of
         * the logical device
         */
        vk_driver(const vk_physical_driver& p_physical);
        ~vk_driver() = default;

        /**
         * @brief returns the specified graphics queue from this logical device
         */
        [[nodiscard]] VkQueue graphics_queue() const {
            return m_device_queues.graphics_queue;
        }

        /**
         * @return -1 if there are no flags available/compatible/valid
         */
        uint32_t select_memory_type(uint32_t p_type_filter,
                                    VkMemoryPropertyFlags p_property_flag);

        // This is just for specifically getting presentation queue
        // Now that I think about this, I may as well use this to get our
        // specific queue family from this logical device
        VkQueue get_queue(const vk_queue_options& p_present_queue);

        void destroy();

        /**
         * @brief gives you the depth format from the logical device
         * 
         * @return VkFormat
        */
        [[nodiscard]] VkFormat depth_format() const;

        /**
         * @brief allows to treat vk_driver as a VkDevice handle
         * 
         * For vulkan API's that accept only taking in VKDevice, this simplifies the need to not have a getter API
        */
        operator VkDevice() const { return m_driver; }

        /**
         * @brief allows to treat vk_driver as a VkDevice handle
         * 
         * For vulkan API's that accept only taking in VKDevice, this simplifies the need to not have a getter API
        */
        operator VkDevice() { return m_driver; }

    private:
        vk_physical_driver m_physical{};
        VkDevice m_driver = nullptr;
        device_queue_family m_device_queues{};
        VkFormat m_depth_format_selected;
    };

};