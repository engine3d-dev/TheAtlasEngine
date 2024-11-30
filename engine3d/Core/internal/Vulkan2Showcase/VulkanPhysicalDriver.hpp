#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace engine3d::vk{
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