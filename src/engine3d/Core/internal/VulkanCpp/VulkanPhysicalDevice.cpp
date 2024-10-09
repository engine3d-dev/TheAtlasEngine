#include "EngineLogger.hpp"
#include <internal/VulkanCpp/VulkanPhysicalDevice.hpp>
#include <internal/VulkanCpp/Vulkan.hpp>
#include <vector>
#include <sstream>

#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan_win32.h>
#include <vulkan/vulkan_core.h>
#else
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#endif

namespace engine3d::vk{

    VulkanPhysicalDevice::VulkanPhysicalDevice(){
        auto& instance = Vulkan::GetVkInstance();
        std::vector<VkPhysicalDevice> devices = get_available_devices();
        
        // printf("available devices size() = %zu\n", devices.size());
        uint32_t gpu_count = devices.size();
        
        //! @note Validating our physical device is a discrete GPU.
        //! @note Using our discrete GPU that has been found on our machine.
        //! @note vkGetPhysicalDeviceProperties is how we get information about our physical device.
        for(VkPhysicalDevice device : devices){
            vkGetPhysicalDeviceProperties(device, &m_PhysicalDeviceProperties);

            if(m_PhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
                m_SelectedPhysicalDevice = device;
                break;
            }
        }

        if(!m_SelectedPhysicalDevice){
            m_SelectedPhysicalDevice = devices.back();
        }

        m_PhysicalDeviceHandler = m_SelectedPhysicalDevice;
        
        vkGetPhysicalDeviceFeatures(m_PhysicalDeviceHandler, &m_PhysicalDeviceFeatures);
        vkGetPhysicalDeviceMemoryProperties(m_PhysicalDeviceHandler, &m_PhysicalMemoryProperties);
        
        //! @note This is how we know what queue families are available to us by checking through our physical devices.
        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDeviceHandler, &queue_family_count, nullptr);
        /* printf("queue_family_count === %i\n", queue_family_count); */
        // assert(queue_family_count > 0);
        if(queue_family_count <= 0) throw std::runtime_error("queue_family_count === 0");

        m_QueueFamilyProperties.resize(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDeviceHandler, &queue_family_count, m_QueueFamilyProperties.data()); // reads in the queue family properties from our physical devices
        
        //! @note Validating our extensions and getting the extensinos that we support.
        uint32_t extension_count = 0;
        vkEnumerateDeviceExtensionProperties(m_PhysicalDeviceHandler, nullptr, &extension_count, nullptr); // just used to read how many extensions are available.

        if(extension_count > 0){
            std::vector<VkExtensionProperties> extensions(extension_count);
            if(vkEnumerateDeviceExtensionProperties(m_PhysicalDeviceHandler, nullptr, &extension_count, &extensions.front()) == VK_SUCCESS){
                // ConsoleLogInfo("Selected physical device {0} extensions", extensions.size());

                for(const auto& ext : extensions){
                    m_SupportedExtensions.emplace(ext.extensionName);
                    // ConsoleLogInfo("{0}", ext.extensionName);
                }
            }
        }

        /**
         * @name Queue Families
         * @note The desired queues needed to be requested from logical device creation
         *  @note Because there are differing queues family configurations of Vulkan implementation this can be tricky, especially if Engine3D's applicatio
         *       can request different queue types.
         * @note Getting queue family indices for the requested family types.
         * @note indices may also overlap depending on the implementation.
        */
        static const float defauly_queue_priority = 0.0f;
        int requested_queue_t = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
        m_QueueIndices = GetQueueFamilyIndices(requested_queue_t);

        // VK_QUEUE_GRAPHICS_BIT
        //! @note Checking queue family for graphics operations. ( Such as vkCmdDraw*(...) )
        if(requested_queue_t & VK_QUEUE_GRAPHICS_BIT){
            VkDeviceQueueCreateInfo createInfo{
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = (uint32_t)m_QueueIndices.Graphics,
                .queueCount = 1,
                .pQueuePriorities = &defauly_queue_priority,
            };
            m_DeviceQueueCreateInfos.push_back(createInfo);
        }

        // VK_QUEUE_COMPUTE_BIT
        //! @note Specifies queues in this queue family support compute operations.
        if(requested_queue_t & VK_QUEUE_COMPUTE_BIT){
            if (m_QueueIndices.Compute != m_QueueIndices.Graphics){
				// If compute family index differs, we need an additional queue create info for the compute queue
				VkDeviceQueueCreateInfo createInfo{
                    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .queueFamilyIndex = (uint32_t)m_QueueIndices.Compute,
                    .queueCount = 1,
                    .pQueuePriorities = &defauly_queue_priority
                };
				m_DeviceQueueCreateInfos.push_back(createInfo);
			}
        }
        
        // VK_QUEUE_TRANSFER_BIT
        //! @note specifies queues in this family only has support for transfer operations.
        if(requested_queue_t & VK_QUEUE_TRANSFER_BIT){
            if((m_QueueIndices.Transfer != m_QueueIndices.Graphics) && (m_QueueIndices.Transfer != m_QueueIndices.Compute)){
				// If compute family index differs, we need an additional queue create info for the compute queue
				VkDeviceQueueCreateInfo createInfo{
                    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .queueFamilyIndex = (uint32_t)m_QueueIndices.Transfer,
                    .queueCount = 1,
                    .pQueuePriorities = &defauly_queue_priority
                };
				m_DeviceQueueCreateInfos.push_back(createInfo);
			}
        }
    }

    VulkanPhysicalDevice::QueueFamilyIndices VulkanPhysicalDevice::GetQueueFamilyIndices(int flag){
        QueueFamilyIndices indices;

        /**
         * @note Dedicated for computing queues-only.
         * @note Searching for a queue family at specific indices that support computing and not graphics.
        */
        if(flag & VK_QUEUE_COMPUTE_BIT){
            for(uint32_t i = 0; i < m_QueueFamilyProperties.size(); i++){
                auto& queue_family_properties = m_QueueFamilyProperties[i];
                if((queue_family_properties.queueFlags & VK_QUEUE_COMPUTE_BIT) and ((queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)){
                    indices.Compute = i;
                    break;
                }
            }
        }

        /**
         * @note Searching for dedicated queues for transfer-specifically.
         * @note Attempting searching for queue family index that supports transfering but no graphics/compute.
        */
        if(flag & VK_QUEUE_TRANSFER_BIT){
            for(uint32_t i = 0; i < m_QueueFamilyProperties.size(); i++){
                auto& queue_family_properties = m_QueueFamilyProperties[i];
                if((queue_family_properties.queueFlags & VK_QUEUE_TRANSFER_BIT) and ((queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)){
                    indices.Transfer = i;
                    break;
                }
            }
        }

        /**
         * @note For any other queue types or if no separate compute queue is present, return irst one to support the requested flags instead.
        */
        if(flag & VK_QUEUE_GRAPHICS_BIT){
            for(uint32_t i = 0; i < m_QueueFamilyProperties.size(); i++){
                if((flag & VK_QUEUE_TRANSFER_BIT) and indices.Transfer == -1){
                    if(m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
                        indices.Transfer = i;
                }

                if((flag & VK_QUEUE_COMPUTE_BIT) and indices.Compute == -1){
                    if(m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
                        indices.Compute = i;
                }

                if(flag & VK_QUEUE_GRAPHICS_BIT){
                    if(m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                        indices.Graphics = i;
                }
            }
        }

        return indices;
    }

    VkFormat VulkanPhysicalDevice::SearchAvailableDepthFormat(){
        // Since all depth formats may be optional, we need to find a suitable depth format to use
		// Start with the highest precision packed format
		std::vector<VkFormat> depthFormats = {
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM
		};

		// TODO: Move to VulkanPhysicalDevice
		for (auto& format : depthFormats)
		{
			VkFormatProperties formatProps;
			vkGetPhysicalDeviceFormatProperties(m_PhysicalDeviceHandler, format, &formatProps);
			// Format must support depth stencil attachment for optimal tiling
			if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
				return format;
		}

		return VK_FORMAT_UNDEFINED;
    }

    bool VulkanPhysicalDevice::IsExtensionSupported(const std::string& ext_name){
        return m_SupportedExtensions.find(ext_name) != m_SupportedExtensions.end();
    }
    
    std::vector<VkPhysicalDevice> VulkanPhysicalDevice::get_available_devices(){
        uint32_t gpu_devices_count = 0;
        printf("Before enumerating physical devices\n");
        if(Vulkan::GetVkInstance() == VK_NULL_HANDLE){
            printf("GetVkInstance() == nullptr!\n");
        }
        VkResult res = vkEnumeratePhysicalDevices(Vulkan::GetVkInstance(), &gpu_devices_count, nullptr);
        // printf("gpu_devices_count (afterwards) === %i\n", gpu_devices_count);
        // static_assert(gpu_devices_count > 0);
        if(res != VK_SUCCESS){
            printf("get_available_devices error number ===>>> %i\n", res);
        }
        std::vector<VkPhysicalDevice> devices(gpu_devices_count);
        // printf("vector size (afterwards) === %zu\n", devices.size());        
        res = vkEnumeratePhysicalDevices(Vulkan::GetVkInstance(), &gpu_devices_count, devices.data());
        return devices;
    }

    void VulkanPhysicalDevice::print_physical_device_property(VkPhysicalDeviceProperties properties){
        printf("==================================================\n");
        std::stringstream ss;
        ss << "Device ID: " << properties.deviceID << '\n';
        ss << "Device Name: " << properties.deviceName << '\n';
        ss << "Driver Version: " << properties.driverVersion << '\n';
        ss << "API Version: " << properties.apiVersion << '\n';
    }


    VkPhysicalDevice& VulkanPhysicalDevice::GetVkPhysicalDevice(){ return m_PhysicalDeviceHandler; }

    VkFormat VulkanPhysicalDevice::GetDepthFormat(){ return SearchAvailableDepthFormat(); }
};
