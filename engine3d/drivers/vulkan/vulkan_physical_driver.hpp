#pragma once
#include <cstdint>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    /**
     * @name Vulkan Physical Driver
     * @note physical devices in vulkan are a concept to interact with the actual hardware on your system
     * @note enables you to directly submit tasks to the GPU using physical devices is a concept defined by vulkan 
    */
    class VulkanPhysicalDriver{
    public:
        struct PhysicalQueueFamilyIndices{
            uint32_t Graphics = -1;
            uint32_t Compute = -1;
            uint32_t Transfer = -1;
        };

        VulkanPhysicalDriver() = default;
        VulkanPhysicalDriver(VkInstance p_Instance);

        uint32_t GetPresentationIndex(VkSurfaceKHR p_Surface);

        PhysicalQueueFamilyIndices GetQueueIndices() { return m_QueueFamilyIndices; }

        VkPhysicalDeviceProperties GetProperties() { return m_CurrentPhysicalDeviceProperties; }

        operator VkPhysicalDevice(){
            return m_PhysicalDevice;
        }

    private:
        PhysicalQueueFamilyIndices SelectQueueFamilyIndices();

    private:
        VkPhysicalDevice m_PhysicalDevice;
        VkPhysicalDeviceProperties m_CurrentPhysicalDeviceProperties;
        std::vector<VkQueueFamilyProperties> m_QueueFamilyProperties;

        // checking if queue families have queues and they are graphical, compute (optional atm), transfer(optional atm)
        PhysicalQueueFamilyIndices m_QueueFamilyIndices;
    };
};