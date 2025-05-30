#pragma once
#include <vulkan/vulkan.h>
#include <drivers/vulkan-cpp/vk_physical_driver.hpp>
#include <drivers/vulkan-cpp/vk_types.hpp>

namespace atlas::vk {

    class vk_driver {
        struct device_queue_family {
            VkQueue graphics_queue;
            VkQueue transfer_queue;
            VkQueue compute_queue;
        };

    public:
        vk_driver() = default;
        vk_driver(const vk_physical_driver& p_physical);
        ~vk_driver() = default;

        [[nodiscard]] VkQueue graphics_queue() const {
            return m_device_queues.graphics_queue;
        }

        //! @note Returns -1 if there are no flags available/compatible/valid
        uint32_t select_memory_type(uint32_t p_type_filter,
                                    VkMemoryPropertyFlags p_property_flag);

        // This is just for specifically getting presentation queue
        // Now that I think about this, I may as well use this to get our
        // specific queue family from this logical device
        VkQueue get_queue(const vk_queue_options& p_present_queue);

        void destroy();

        [[nodiscard]] VkFormat depth_format() const;

        operator VkDevice() const { return m_driver; }

        operator VkDevice() { return m_driver; }

    private:
        static vk_driver* s_instance;
        vk_physical_driver m_physical{};
        VkDevice m_driver = nullptr;
        device_queue_family m_device_queues{};
        VkFormat m_depth_format_selected;
    };

};