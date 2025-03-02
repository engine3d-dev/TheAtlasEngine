#pragma once
#include <drivers/vulkan/vulkan_physical_driver.hpp>
#include <vulkan/vulkan.hpp>

namespace atlas::vk {
    class vk_driver {
    public:
        vk_driver() = default;
        vk_driver(const vk_physical_driver& p_physical);

        VkQueue get_graphics_queue() const { return m_graphics_queue; }

        operator VkDevice() { return m_driver; }

        //! @note Returns -1 if there are no flags available/compatible/valid
        uint32_t select_memory_type(uint32_t p_type_filter,
                                    VkMemoryPropertyFlags p_property_flag);

    private:
        VkDevice m_driver;
        VkQueue m_graphics_queue;
    };
};