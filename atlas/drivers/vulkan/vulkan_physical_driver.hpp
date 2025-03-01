#pragma once
#include <cstdint>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

namespace atlas::vk{
    /**
     * @name Vulkan Physical Driver
     * @note physical devices in vulkan are a concept to interact with the actual hardware on your system
     * @note enables you to directly submit tasks to the GPU using physical devices is a concept defined by vulkan 
    */
    class vk_physical_driver{
    public:
        struct queue_family_indices{
            uint32_t Graphics = -1;
            uint32_t Compute = -1;
            uint32_t Transfer = -1;
        };

        vk_physical_driver() = default;
        vk_physical_driver(const VkInstance& p_instance);

        uint32_t get_presentation_index(VkSurfaceKHR p_surface) const;

        uint32_t search_memory_type(uint32_t p_type_filter, VkMemoryPropertyFlags p_properties);

        queue_family_indices get_queue_indices() const { return m_queue_family_indices; }

        VkPhysicalDeviceProperties get_properties() const { return m_physical_properties; }

        operator VkPhysicalDevice(){
            return m_physical;
        }

        operator VkPhysicalDevice() const{
            return m_physical;
        }

    private:
        queue_family_indices select_queue_family_indices();

    private:
        VkPhysicalDevice m_physical;
        VkPhysicalDeviceProperties m_physical_properties;
        std::vector<VkQueueFamilyProperties> m_queue_family_properties;

        // checking if queue families have queues and they are graphical, compute (optional atm), transfer(optional atm)
        queue_family_indices m_queue_family_indices;
    };
};