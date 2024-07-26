#include "Core/EngineLogger.h"
#include <Core/backend/Vulkan/VulkanDevice.h>
#include <Core/backend/Vulkan/Vulkan.h>
#include <map>
#include <optional>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan_win32.h>

namespace engine3d{

    //! @note Checking how much our suitability is.
    static int RateDeviceSuitability(VkPhysicalDevice device){
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;

        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        //! @note Discrete GPU's have significant performance advantage.
        int score = 0;
        if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
            score += 1000;
        }

        // Mac possible size of textures affects graphics quality.
        score += deviceProperties.limits.maxImageDimension2D;

        //! @note Applications cant function without geom shaders
        if(!deviceFeatures.geometryShader){
            return 0;
        }

        return score;
    }

    struct QueueFamilyIndices{
        std::optional<uint32_t> graphicsFamily;

        bool IsComplete(){
            return graphicsFamily.has_value();
        }
    };

    static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device){
        // searching for graphics queue families
        QueueFamilyIndices indices;

        //! @note Queue family checks
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int id = 0;
        for(const auto& queueFam : queueFamilies){
            if(queueFam.queueFlags & VK_QUEUE_GRAPHICS_BIT){
                indices.graphicsFamily = id;
            }

            if(indices.IsComplete()){
                break;
            }

            id++;
        }



        return indices;
    }

    //! @param device_t is to check if our supported devices on our hardware is a GPU.
    //! @param geometryShader allows enabling options for GPU's that allows you to use geometry shaders
    static bool IsDeviceSuitable(VkPhysicalDevice device){
        // VkPhysicalDeviceProperties deviceProperties;
        // VkPhysicalDeviceFeatures deviceFeatures;

        // vkGetPhysicalDeviceProperties(device, &deviceProperties);
        // vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        // return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
        QueueFamilyIndices indices = FindQueueFamilies(device);
        return indices.IsComplete();
    }

    void VulkanDevice::InitializeVulkanDevices(){
        PickPhysicalDevice();
        InitializeLogicalDevice();
    }

    void VulkanDevice::CleanupVulkanDevice(){
        vkDestroyDevice(VulkanPipeline::GetVulkanProperties().logicalDevice, nullptr);
    }


    void VulkanDevice::PickPhysicalDevice(){
        //! @note Searching if there are suitable physica GPU devices with vulkan support.
        uint32_t deviceCount = 0;
        auto& properties = VulkanPipeline::GetVulkanProperties();

        vkEnumeratePhysicalDevices(properties.instance, &deviceCount, nullptr);

        if(deviceCount == 0){
            CoreLogError("Failed to find any GPU's with vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(properties.instance, &deviceCount, devices.data());

        for(const auto& device : devices){
            if(IsDeviceSuitable(device)){
                properties.physicalDevice = device;
                break;
            }
        }

        if(properties.physicalDevice == VK_NULL_HANDLE){
            CoreLogError("Failed to find suitable GPU for Vulkan");
        }

        //! @note Setting up support for optional features like texture compression, 64-bit floats, and multi-viewport rendering (useful for VR)
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(properties.physicalDevice, &deviceFeatures);


        //! @note ordered map to automatically sort candidates by increasing score.
        std::multimap<int, VkPhysicalDevice> candidates;

        for(const auto& device : devices){
            int score = RateDeviceSuitability(device);
            candidates.insert({score, device});
        }

        // Checking if best candidate is suitable at all.
        if(candidates.rbegin()->first > 0){
            properties.physicalDevice = candidates.begin()->second;
        }
        else{
            CoreLogError("Cannot find suitable GPU!");
        }
    }

    void VulkanDevice::InitializeLogicalDevice(){
        auto& properties = VulkanPipeline::GetVulkanProperties();
        //! @note Initiate logical device creation information.
        QueueFamilyIndices indices = FindQueueFamilies(properties.physicalDevice);

        //! @note Handling Queue families creation properties here.
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;

        //! @note Curent avail drivers will only allow creating small nums of queues for each queue family.
        //! @note Vulkan lets you assign priorities to queues to influence the scheduling of command buffers executions using floats
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;







        //! @note Creating logical devices information properties.
        VkPhysicalDeviceFeatures deviceFeatures{};
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = 0;
         
        if(VulkanPipeline::EnableValidationLayer()){
            createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanPipeline::ValidationLayers().size());
            createInfo.ppEnabledLayerNames = VulkanPipeline::ValidationLayers().data();
        }
        else{
            createInfo.enabledLayerCount = 0;
        }

        VkResult res = vkCreateDevice(properties.physicalDevice, &createInfo, nullptr, &properties.logicalDevice);

        if(res != VK_SUCCESS){
            CoreLogError("Failed to create logical device");
        }
        
        //! @note Retrieve queue handles
        //! @note Gets our queue handler for each queue family.
        VkQueue graphicsQueue;
        vkGetDeviceQueue(properties.logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
    }

    void VulkanDevice::InitializeSurface(){
        // VkWin32SurfaceCreateInfoKHR createInfo{};
        // createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        // createInfo.hwnd = glfwGetWin32Window(VulkanPipeline::GetCurrentWindow());

    }
};