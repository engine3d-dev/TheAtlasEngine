#include <drivers/vulkan/vulkan_physical_driver.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <core/engine_logger.hpp>

namespace atlas::vk{
    vk_physical_driver::vk_physical_driver(const VkInstance& p_instance){
        console_log_info("Vulkan: Begin Vulkan Physical Driver Initialization!!!");

        uint32_t gpu_count;
        std::vector<VkPhysicalDevice> available_gpus;

        //! @note Querying number of available GPU's (Physical Devices) on our machine
        vk_check(vkEnumeratePhysicalDevices(p_instance, &gpu_count, nullptr), "vkEnumeratePhysicalDevices", __FILE__, __LINE__, __FUNCTION__);

        available_gpus.resize(gpu_count);
        vk_check(vkEnumeratePhysicalDevices(p_instance, &gpu_count, available_gpus.data()), "vkEnumeratePhysicalDevices", __FILE__, __LINE__, __FUNCTION__);

        // we populate the compatible device.
        //! @note Checking for compatible GPU's on our machine. (Init the physical device structs)
        for(const auto& device : available_gpus){
            VkPhysicalDeviceProperties device_properties;
            VkPhysicalDeviceFeatures device_features;
            vkGetPhysicalDeviceProperties(device, &device_properties);
            vkGetPhysicalDeviceFeatures(device, &device_features);

            //! @note VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU checks if our device is an external device.
            if(device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
                m_physical = device;
                break;
            }
        }

        if(available_gpus.size() == 0){
            m_physical = available_gpus[0];
        }

        if(available_gpus.empty()){
            console_log_error("There was no available compatible GPU's found!");
            m_physical = VK_NULL_HANDLE;
        }

        //! @note Initialized and Identifying Queue Families.
        uint32_t queue_family_count;
        vkGetPhysicalDeviceQueueFamilyProperties(m_physical, &queue_family_count, nullptr);
        m_queue_family_properties.resize(queue_family_count);

        //! @note Loading queue family data from the physical device that we have selected.
        vkGetPhysicalDeviceQueueFamilyProperties(m_physical, &queue_family_count, m_queue_family_properties.data());


        //! @note Getting our actual physical device properties that have been currently selected.
        vkGetPhysicalDeviceProperties(m_physical, &m_physical_properties);

        m_queue_family_indices = select_queue_family_indices();

        console_log_warn("Vulkan: End Vulkan Physical Initialization Completed");
    }

    vk_physical_driver::queue_family_indices vk_physical_driver::select_queue_family_indices(){
        vk_physical_driver::queue_family_indices indices;
        int i = 0;

        for(const auto& queue_family : m_queue_family_properties){
            if(queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT){
                indices.Graphics = i;
                break;
            }
            i++;
        }

        // Graphics if its -1 meaning it could not find any graphics queue available.
        // console_log_warn("Graphics Index === {}", (int)indices.Graphics);

        return indices;
    }

    uint32_t vk_physical_driver::search_memory_type(uint32_t p_type_filter, VkMemoryPropertyFlags p_property_flags){
        VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
        vkGetPhysicalDeviceMemoryProperties(m_physical, &physical_device_memory_properties);

        for(uint32_t i = 0; i < physical_device_memory_properties.memoryTypeCount; i++){
            if((p_type_filter & (1 << i)) and (physical_device_memory_properties.memoryTypes[i].propertyFlags & p_property_flags)){
                return i;
            }
        }

        return -1;
    }

    
    uint32_t vk_physical_driver::get_presentation_index(VkSurfaceKHR p_surface) const{
        uint32_t PresentationIndex = -1;
        VkBool32 compatible = VK_FALSE;
        uint32_t i = 0;
        for(const auto& queue_family : m_queue_family_properties){
            if(queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT){
                vk_check(vkGetPhysicalDeviceSurfaceSupportKHR(m_physical, i, p_surface, &compatible), "vkGetPhysicalDeviceSurfaceSupportKHR", __FILE__, __LINE__, __FUNCTION__);

                if(compatible){
                    PresentationIndex = i;
                }
            }
            i++;
        }

        console_log_warn("Presentation Index === {}", PresentationIndex);
        return PresentationIndex;
    }
};