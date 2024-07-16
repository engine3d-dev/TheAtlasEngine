#include <Core/EngineLogger.h>
#include <Core/backend/Vulkan/VulkanDevice.h>
#include <Core/backend/Vulkan/Vulkan.h>
#include <map>
#include <optional>
#include <set>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan_win32.h>

namespace engine3d{

    static std::string VkResultToString(VkResult res){
        switch (res){
            case VkResult::VK_SUCCESS: return "VkResult::VK_SUCCESS";
            case VkResult::VK_NOT_READY: return "VkResult::VK_NOT_READY";
            case VkResult::VK_TIMEOUT: return "VkResult::VK_TIMEOUT";
            case VkResult::VK_EVENT_SET: return "VkResult::VK_EVENT_SET";
            case VkResult::VK_EVENT_RESET: return "VkResult::VK_EVENT_RESET";
            case VkResult::VK_INCOMPLETE: return "VkResult::VK_INCOMPLETE";
            case VkResult::VK_ERROR_OUT_OF_HOST_MEMORY: return "VkResult::VK_ERROR_OUT_OF_HOST_MEMORY";
            case VkResult::VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VkResult::VK_ERROR_OUT_OF_DEVICE_MEMORY";
            case VkResult::VK_ERROR_INITIALIZATION_FAILED: return "VkResult::VK_ERROR_INITIALIZATION_FAILED";
            case VkResult::VK_ERROR_DEVICE_LOST: return "VkResult::VK_ERROR_DEVICE_LOST";
            case VkResult::VK_ERROR_MEMORY_MAP_FAILED: return "VkResult::VK_ERROR_MEMORY_MAP_FAILED";
            case VkResult::VK_ERROR_LAYER_NOT_PRESENT: return "VkResult::VK_ERROR_LAYER_NOT_PRESENT";
            case VkResult::VK_ERROR_EXTENSION_NOT_PRESENT: return "VkResult::VK_ERROR_EXTENSION_NOT_PRESENT";
            case VkResult::VK_ERROR_FEATURE_NOT_PRESENT: return "VkResult::VK_ERROR_FEATURE_NOT_PRESENT";
            case VkResult::VK_ERROR_INCOMPATIBLE_DRIVER: return "VkResult::VK_ERROR_INCOMPATIBLE_DRIVER";
            case VkResult::VK_ERROR_TOO_MANY_OBJECTS: return "VkResult::VK_ERROR_TOO_MANY_OBJECTS";
            case VkResult::VK_ERROR_FORMAT_NOT_SUPPORTED: return "VkResult::VK_ERROR_FORMAT_NOT_SUPPORTED";
            case VkResult::VK_ERROR_FRAGMENTED_POOL: return "VkResult::VK_ERROR_FRAGMENTED_POOL";
            case VkResult::VK_ERROR_UNKNOWN: return "VkResult::VK_ERROR_UNKNOWN";
            case VkResult::VK_ERROR_OUT_OF_POOL_MEMORY: return "VkResult::VK_ERROR_OUT_OF_POOL_MEMORY";
            case VkResult::VK_ERROR_INVALID_EXTERNAL_HANDLE: return "VkResult::VK_ERROR_INVALID_EXTERNAL_HANDLE";
            case VkResult::VK_ERROR_FRAGMENTATION: return "VkResult::VK_ERROR_FRAGMENTATION";
            case VkResult::VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "VkResult::VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
            case VkResult::VK_PIPELINE_COMPILE_REQUIRED: return "VkResult::VK_PIPELINE_COMPILE_REQUIRED";
            case VkResult::VK_ERROR_SURFACE_LOST_KHR: return "VkResult::VK_ERROR_SURFACE_LOST_KHR";
            case VkResult::VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VkResult::VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
            case VkResult::VK_SUBOPTIMAL_KHR: return "VkResult::VK_SUBOPTIMAL_KHR";
            case VkResult::VK_ERROR_OUT_OF_DATE_KHR: return "VkResult::VK_ERROR_OUT_OF_DATE_KHR";
            case VkResult::VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "VkResult::VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
            case VkResult::VK_ERROR_VALIDATION_FAILED_EXT: return "VkResult::VK_ERROR_VALIDATION_FAILED_EXT";
            case VkResult::VK_ERROR_INVALID_SHADER_NV: return "VkResult::VK_ERROR_INVALID_SHADER_NV";
            case VkResult::VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR: return "VkResult::VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
            case VkResult::VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR: return "VkResult::VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
            case VkResult::VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR: return "VkResult::VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
            case VkResult::VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR: return "VkResult::VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
            case VkResult::VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR: return "VkResult::VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
            case VkResult::VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR: return "VkResult::VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
            case VkResult::VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return "VkResult::VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
            case VkResult::VK_ERROR_NOT_PERMITTED_KHR: return "VkResult::VK_ERROR_NOT_PERMITTED_KHR";
            case VkResult::VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return "VkResult::VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
            case VkResult::VK_THREAD_IDLE_KHR: return "VkResult::VK_THREAD_IDLE_KHR";
            case VkResult::VK_THREAD_DONE_KHR: return "VkResult::VK_THREAD_DONE_KHR";
            case VkResult::VK_OPERATION_DEFERRED_KHR: return "VkResult::VK_OPERATION_DEFERRED_KHR";
            case VkResult::VK_OPERATION_NOT_DEFERRED_KHR: return "VkResult::VK_OPERATION_NOT_DEFERRED_KHR";
        #ifdef VK_ENABLE_BETA_EXTENSIONS
            case VkResult::VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR: return "VkResult::";
        #endif
            case VkResult::VK_ERROR_COMPRESSION_EXHAUSTED_EXT: return "VkResult::VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
            case VkResult::VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT: return "VkResult::VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT";
            // case VkResult::VK_ERROR_OUT_OF_POOL_MEMORY_KHR: return "VkResult::";
            // case VkResult::VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR: return "VkResult::";
            // case VkResult::VK_ERROR_FRAGMENTATION_EXT: return "VkResult::";
            // case VkResult::VK_ERROR_NOT_PERMITTED_EXT: return "VkResult::";
            // case VkResult::VK_ERROR_INVALID_DEVICE_ADDRESS_EXT: return "VkResult::";
            // case VkResult::VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR: return "VkResult::";
            // case VkResult::VK_PIPELINE_COMPILE_REQUIRED_EXT: return "VkResult::";
            // case VkResult::VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT: return "VkResult::";
            // case VkResult::VK_RESULT_MAX_ENUM: return "VkResult::";
        default:
            return "Returning default value for VkResult:: string!";
        }
    }

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
        std::optional<uint32_t> presentFamily;

        bool IsComplete(){
            return graphicsFamily.has_value() and presentFamily.has_value();
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
            // if(queueFam.queueFlags & VK_QUEUE_GRAPHICS_BIT){
            if(queueFam.queueFlags & VK_QUEUE_GRAPHICS_BIT){
                indices.graphicsFamily = id;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, id, VulkanPipeline::GetVulkanProperties().surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = id;
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
        InitializeSurface();
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
        // VkPhysicalDeviceFeatures deviceFeatures;
        // vkGetPhysicalDeviceFeatures(properties.physicalDevice, &deviceFeatures);


        // //! @note ordered map to automatically sort candidates by increasing score.
        // std::multimap<int, VkPhysicalDevice> candidates;

        // for(const auto& device : devices){
        //     int score = RateDeviceSuitability(device);
        //     candidates.insert({score, device});
        // }

        // // Checking if best candidate is suitable at all.
        // if(candidates.rbegin()->first > 0){
        //     properties.physicalDevice = candidates.begin()->second;
        // }
        // else{
        //     CoreLogError("Cannot find suitable GPU!");
        // }
    }

    void VulkanDevice::InitializeLogicalDevice(){
        auto& properties = VulkanPipeline::GetVulkanProperties();
        //! @note Initiate logical device creation information.
        QueueFamilyIndices indices = FindQueueFamilies(properties.physicalDevice);

        //! @note Handling Queue families creation properties here.
        // VkDeviceQueueCreateInfo queueCreateInfo{};
        // queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        // queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        // queueCreateInfo.queueCount = 1;

        // //! @note Curent avail drivers will only allow creating small nums of queues for each queue family.
        // //! @note Vulkan lets you assign priorities to queues to influence the scheduling of command buffers executions using floats
        // float queuePriority = 1.0f;
        // queueCreateInfo.pQueuePriorities = &queuePriority;

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};
        // std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for(uint32_t queueFam : uniqueQueueFamilies){
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFam;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        // ----------------------------------------------------------------
        //! @note Creating logical devices information properties.
        VkPhysicalDeviceFeatures deviceFeatures{};
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        // createInfo.pQueueCreateInfos = &queueCreateInfo;
        // createInfo.queueCreateInfoCount = 1;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = 0;
         
        if(VulkanPipeline::EnableValidationLayer()){
            createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanPipeline::ValidationLayers().size());
            createInfo.ppEnabledLayerNames = VulkanPipeline::ValidationLayers().data();
        }
        else{
            createInfo.enabledLayerCount = 0;
        }

        if(vkCreateDevice(properties.physicalDevice, &createInfo, nullptr, &properties.logicalDevice) != VK_SUCCESS){
            CoreLogError("Failed to create logical device");
        }

        //! @note Retrieve queue handles
        //! @note Gets our queue handler for each queue family.
        vkGetDeviceQueue(properties.logicalDevice, indices.graphicsFamily.value(), 0, &properties.graphicsQueue);
        vkGetDeviceQueue(properties.logicalDevice, indices.presentFamily.value(), 0, &properties.presentQueue);
    }

    void VulkanDevice::InitializeSurface(){
        //! @note Might not need this...
        // VkWin32SurfaceCreateInfoKHR createInfo{};
        // createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        // createInfo.hwnd = glfwGetWin32Window(VulkanPipeline::GetCurrentWindow());
        // createInfo.hinstance = GetModuleHandle(nullptr);

        // if(vkCreateWin32SurfaceKHR(properties.instance, &createInfo, nullptr, &properties.surface) != VK_SUCCESS){
        //     CoreLogError("Failed to create window surface");
        // }

        auto& properties = VulkanPipeline::GetVulkanProperties();
        VkResult res = glfwCreateWindowSurface(properties.instance, VulkanPipeline::GetCurrentWindow(), nullptr, &properties.surface);

        if(res != VK_SUCCESS){
            CoreLogError("VkResult was {}", VkResultToString(res));
            CoreLogError("Failed to initiate Window surface!");
        }

    }
};