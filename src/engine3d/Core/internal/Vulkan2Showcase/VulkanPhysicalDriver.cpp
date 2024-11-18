#include "ApplicationInstance.hpp"
#include <internal/Vulkan2Showcase/VulkanPhysicalDriver.hpp>
#include <internal/Vulkan2Showcase/helper_functions.hpp>
#include <Core/EngineLogger.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    VulkanPhysicalDriver::VulkanPhysicalDriver(VkInstance p_Instance){
        ConsoleLogInfo("Vulkan2Showcase: Begin VulkanPhysicalDriver Initialization!!");
        uint32_t gpu_count;
        std::vector<VkPhysicalDevice> available_gpus;

        //! @note Querying number of available GPU's (Physical Devices) on our machine
        vk_check(vkEnumeratePhysicalDevices(p_Instance, &gpu_count, nullptr), "vkEnumeratePhysicalDevices", __FILE__, __LINE__, __FUNCTION__);

        available_gpus.resize(gpu_count);
        vk_check(vkEnumeratePhysicalDevices(p_Instance, &gpu_count, available_gpus.data()), "vkEnumeratePhysicalDevices", __FILE__, __LINE__, __FUNCTION__);

        // we populate the compatible device.
        //! @note Checking for compatible GPU's on our machine. (Init the physical device structs)
        for(const auto& device : available_gpus){
            VkPhysicalDeviceProperties device_properties;
            VkPhysicalDeviceFeatures device_features;
            vkGetPhysicalDeviceProperties(device, &device_properties);
            vkGetPhysicalDeviceFeatures(device, &device_features);

            //! @note VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU checks if our device is an external device.
            if(device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU || device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU){
                m_PhysicalDevice = device;
                break;
            }
        }

        if(available_gpus.size() == 0){
            m_PhysicalDevice = available_gpus[0];
        }

        if(available_gpus.empty()){
            ConsoleLogError("There was no available compatible GPU's found!");
            m_PhysicalDevice = VK_NULL_HANDLE;
        }

        //! @note Initialized and Identifying Queue Families.
        uint32_t queue_family_count;
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queue_family_count, nullptr);
        m_QueueFamilyProperties.resize(queue_family_count);

        //! @note Loading queue family data from the physical device that we have selected.
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queue_family_count, m_QueueFamilyProperties.data());

        m_QueueFamilyIndices = SelectQueueFamilyIndices();

        ConsoleLogWarn("Vulkan2Showcase: VulkanPhysicalDriver Initialized Completed!!!");
    }

    VulkanPhysicalDriver::PhysicalQueueFamilyIndices VulkanPhysicalDriver::SelectQueueFamilyIndices(){
        VulkanPhysicalDriver::PhysicalQueueFamilyIndices indices;
        int i = 0;
        VkBool32 compatible = VK_FALSE;

        for(const auto& queue_family : m_QueueFamilyProperties){
            if(queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT){
                indices.Graphics = i;
                break;
            }
            i++;
        }

        // Graphics if its -1 meaning it could not find any graphics queue available.
        // ConsoleLogWarn("Graphics Index === {}", (int)indices.Graphics);

        return indices;
    }
    
    uint32_t VulkanPhysicalDriver::GetPresentationIndex(VkSurfaceKHR p_Surface){
        uint32_t PresentationIndex = -1;
        VkBool32 compatible = VK_FALSE;
        uint32_t i = 0;
        for(const auto& queue_family : m_QueueFamilyProperties){
            if(queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT){
                vk_check(vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, p_Surface, &compatible), "vkGetPhysicalDeviceSurfaceSupportKHR", __FILE__, __LINE__, __FUNCTION__);

                if(compatible){
                    PresentationIndex = i;
                }
            }
            i++;
        }

        ConsoleLogWarn("Presentation Index === {}", PresentationIndex);
        return PresentationIndex;
    }

};