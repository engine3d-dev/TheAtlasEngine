#include <Core/EngineLogger.h>
#include <Core/backend/Vulkan/VulkanDevice.h>
#include <Core/backend/Vulkan/Vulkan.h>
#include <Core/backend/utilities/helper_functions.h>
#include <map>
#include <optional>
#include <set>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan_win32.h>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    namespace vk{
        // static PhysicalDevice g_physicalDevices;
        /**
         * @note The process that i learned when trying to check if physical devices are available is first check based on the size count of the devices.
         * @note Then set the data to receive those properties to nullptr.
         * @note Once verified that we have the physical device properties available, then we set that same parameter from nullptr to our actual data receivers from these functions
         * @note When looking into this function, you'll see lots of repeats because they have to validate if there are those properties before uses.
        */
        void VulkanDevice::InitializeDevice(){
            uint32_t devicesCount = 0;
            VkResult res = vkEnumeratePhysicalDevices(VulkanPipeline::GetVulkanProperties().instance, &devicesCount, nullptr);

            if(res != VK_SUCCESS){
                CoreLogError("vkEnumeratePhysicalDevices Error (1) = {}", VkResultToString(res));
            }

            m_PhysicalDevices.resize(devicesCount);

            std::vector<VkPhysicalDevice> devices;
            devices.resize(devicesCount);

            // VkResult r = vkEnumeratePhysicalDevices(g_physicalDevices.device, &devicesCount, m_PhysicalDevices.data());
            res = vkEnumeratePhysicalDevices(VulkanPipeline::GetVulkanProperties().instance, &devicesCount, devices.data());

            if(res != VK_SUCCESS){
                CoreLogError("vkEnumeratePhysicalDevices Error (2) = {}", VkResultToString(res));
            }

            /**
             * @note First time we iterate we handle our physical device (that is our handler)
             * @note Then once we specify that, the second time we actually try to get the queue families that are available on our system.
            */
            for(uint32_t i = 0; i < devicesCount; i++){
                VkPhysicalDevice dev = devices[i];
                m_PhysicalDevices[i].device = dev;
                vkGetPhysicalDeviceProperties(m_PhysicalDevices[i].device, &m_PhysicalDevices[i].deviceProperties);

                CoreLogInfo("Device Name {}", m_PhysicalDevices[i].deviceProperties.deviceName);
                uint32_t apiVer = m_PhysicalDevices[i].deviceProperties.apiVersion;
                CoreLogInfo("API Version: {}.{}.{}.{}", VK_API_VERSION_VARIANT(apiVer), VK_API_VERSION_MAJOR(apiVer), VK_API_VERSION_MINOR(apiVer), VK_API_VERSION_PATCH(apiVer));

                uint32_t numQFamilies = 0;

                //! @note What first sets our physical device handler
                vkGetPhysicalDeviceQueueFamilyProperties(dev, &numQFamilies, nullptr);
                CoreLogInfo("Count of Family Queues: {}", numQFamilies);

                m_PhysicalDevices[i].queueFamProperties.resize(numQFamilies);
                m_PhysicalDevices[i].queueSupportPresent.resize(numQFamilies);

                //! @note Since in the GPU draw instructions are represented in queues
                //! @note Where there are different types of queue families that are specific to their respective use-cases.
                //! @note Physical Device is used to expose what queue family do we want to submit our command buffers to.
                //! @note What actually gives us our family properties
                vkGetPhysicalDeviceQueueFamilyProperties(dev, &numQFamilies, m_PhysicalDevices[i].queueFamProperties.data());
                
                //! @note What will iterate through our queue families
                //! @note We can check if queue families accept transfers
                for(uint32_t q = 0; q < numQFamilies; i++){
                    CoreLogWarn("Debugging for-loop! (1)");
                    //! @note Gives us the properties for our family properties.
                    const VkQueueFamilyProperties& queueFamProperties = m_PhysicalDevices[i].queueFamProperties[q];
                    CoreLogWarn("Debugging for-loop! (2)");
                    CoreLogInfo("Family {} Num Queues: {}", q, queueFamProperties.queueCount);
                    CoreLogWarn("Debugging for-loop! (3)");
                    VkQueueFlags flags = queueFamProperties.queueFlags;

                    CoreLogInfo("GFX {}, Compute {}, Transfer {}, Sparse Binding {}", (flags & VK_QUEUE_GRAPHICS_BIT) ? "Yes" : "No", (flags & VK_QUEUE_COMPUTE_BIT) ? "Yes" : "No", (flags & VK_QUEUE_TRANSFER_BIT) ? "Yes" : "No", (flags & VK_QUEUE_SPARSE_BINDING_BIT) ? "Yes" : "No");

                    res = vkGetPhysicalDeviceSurfaceSupportKHR(dev, q, VulkanPipeline::GetVulkanProperties().surface, &(m_PhysicalDevices[i].queueSupportPresent[q]));
                    
                    if(res != VK_SUCCESS){
                        CoreLogError("vkGetPhysicalDeviceSurfaceSupportKHR (1) Error in VulkanDevice on line {}", __LINE__);
                    }
                }

                // //! @note Getting our surface formats
                // //! @note Checking if we even have any available
                // uint32_t numFormats = 0;
                // res = vkGetPhysicalDeviceSurfaceFormatsKHR(dev, VulkanPipeline::GetVulkanProperties().surface, &numFormats, nullptr);

                // if(res != VK_SUCCESS){
                //     CoreLogError("vkGetPhysicalDeviceSurfaceSupportKHR (2) Error in VulkanDevice on line {}", __LINE__);
                // }

                // if(numFormats > 0){
                //     CoreLogError("vkGetPhysicalDeviceSurfaceSupportKHR (2) Error in VulkanDevice on line {}", __LINE__);
                //     return;
                // }
                
                // m_PhysicalDevices[i].surfaceFormats.resize(numFormats);

                // res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dev, VulkanPipeline::GetVulkanProperties().surface, &(m_PhysicalDevices[i].surfaceCapabilities));

                // if(res != VK_SUCCESS){
                //     CoreLogError("vkGetPhysicalDeviceSurfaceCapabilitiesKHR Error in VulkanDevice on line {}", __LINE__);
                // }

                // CoreLogInfo("Image usage flags {}", VkImageUsageFlagsToString(m_PhysicalDevices[i].surfaceCapabilities.supportedUsageFlags));

                // //! @note Checking for presentation mode support
                // uint32_t numPresentModes = 0;
                // res = vkGetPhysicalDeviceSurfacePresentModesKHR(dev, VulkanPipeline::GetVulkanProperties().surface, &numPresentModes, nullptr);

                // if(res != VK_SUCCESS){
                //     CoreLogError("vkGetPhysicalDeviceSurfacePresentModesKHR Error in VulkanDevice on line {}", __LINE__);
                // }

                // if(numPresentModes > 0){
                //     CoreLogError("vkGetPhysicalDeviceSurfacePresentModesKHR (2) Error in VulkanDevice on line {}", __LINE__);
                //     return;
                // }

                // m_PhysicalDevices[i].presentModes.resize(numPresentModes);

                // res = vkGetPhysicalDeviceSurfacePresentModesKHR(dev, VulkanPipeline::GetVulkanProperties().surface, &numPresentModes, m_PhysicalDevices[i].presentModes.data());
            }
        }

        void VulkanDevice::CleanupDevice(){
            // vkDestroyDevice(g_physicalDevices.device, nullptr);
        }
    }
};