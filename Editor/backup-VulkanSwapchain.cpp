#define NOMINMAX // Used for disabling WinDef.h max() conflicting with std::numeric_limits<>::max()
#include <Core/EngineLogger.h>
#include <Core/backend/Vulkan/VulkanDevice.h>
#include <Core/backend/Vulkan/Vulkan.h>
#include <map>
#include <optional>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <vulkan/vulkan_core.h>

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

    /**
     * @note Checks for device extensions are supported within Vulkan drivers.
     * @note  We enumerate to retrieve properties for extensions on our physical device.
     * @note Other words determining extensions implemented by layer that returns set pLayerName by vulkan.
    */
    static bool CheckDeviceExtensionSupported(VkPhysicalDevice device){
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(VulkanPipeline::DeviceExtensions().begin(), VulkanPipeline::DeviceExtensions().end());

        for(const auto& extension : availableExtensions){
            requiredExtensions.erase(extension.extensionName);
        }


        return requiredExtensions.empty();
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

    /**
     * @name Query Swapchain Supported Details
     * @note Three basic properties we need to check for querying swapchain support
     * @note First, basic surface capabilities (min/max nums of imgs in swapchain, min/max width and height of those imgs)
     * @note Second, surface formats (pixel formats, color spaces)
     * @note Third, Available presentation modes
    */
    struct SwapChainSupportDetails{
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
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

    static SwapChainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device){
        SwapChainSupportDetails details;

        //! @note Querying and returning VkSurfaceCapabilitiesKHR
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, VulkanPipeline::GetVulkanProperties().surface, &details.capabilities);

        //! @note Querying supported surface formats
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanPipeline::GetVulkanProperties().surface, &formatCount, nullptr);

        if(formatCount != 0){
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanPipeline::GetVulkanProperties().surface, &formatCount, details.formats.data());
        }

        //! @note Querying the supported presentation modes.
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanPipeline::GetVulkanProperties().surface, &presentModeCount, nullptr);

        if(presentModeCount != 0){
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanPipeline::GetVulkanProperties().surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    //! @note passing available formats for our swapchain.
    //! @note VkSurfaceFormatKHR contains following parameters: format, colorspace
    //! @param format specifies color channels. Example would be VK_FORMAT_B8G8R8A8_SRGB which means we store the B, G, R, and alpha channels in order of 8-bit unsigned ints for total 32-bit per pixel.
    //! @param colorSpace indicates if SRGB  color space is supported or not using the VK_COLOR_SPACE_SRGB_NONLINEAR_KHR flag.
    //! @note Keep in mind, this flag used to be referred to as VK_COLORSPACE_SRGB_NONLINEAR_KHR
    static VkSurfaceFormatKHR SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats){
        for(const auto& format : availableFormats){
            if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
                return format;
            }
        }

        return availableFormats[0];
    }

    /**
     * @name Presentation Modes
     * @note Important setting for swapchain because it represents for showing imgs to screen.
     * @note There are four possible modes available in VUlkan.
     * 
     * @param VK_PRESENT_MODE_IMMEDIATE_KHR: Images submitted by your application are transferred to the screen right away, which may result in tearing.
     * @param VK_PRESENT_MODE_FIFO_KHR: Swapchain is a queue where display takes an image from font when displayed and updated inserting rendered images to back of queue
     * @note Keep in mind if queue is full, application must wait. This is similar to vertical sync as found in modern games.
     * @param VK_PRESENT_MODE_FIFO_RELAXED_KHR: Differs from previous one if application is late and queue was empty at last vertical blank
     * @note Other words, instead of waiting for next vertical blank, img is transferred right waway when it finally arrives, may result in visible tearing.
     * @param VK_PRESENT_MODE_MAILBOX_KHR: Another variation of VK_PRESENT_MODE_FIFO_KHR. Instead of blocking application when queue full, images already in queue are replaced with newer ones.
     * @note This mode can be used to render frames as fast as possible while still avoiding tearing, resulting in fewer latency issues than standard vertical sync.
    */
    VkPresentModeKHR SelectSwapchainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes){
        for(const auto& presentMode : availablePresentModes){
            if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR){
                return presentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    /**
     * @name Swap Extent
     * @note Is the resolution of swapchain images and almost always exactly equal to resolution of the application window.
     * @note Range of possible resolutions defined in VkSurfaceCapabilitiesKHR.
     * @note Vulkan tells us to match the resolution of the window by setting width/height to "currentExtent" member
    */
    VkExtent2D SelectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities){
        if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
            return capabilities.currentExtent;
        }
        else{
            int width, height;
            glfwGetFramebufferSize(VulkanPipeline::GetCurrentWindow(), &width, &height);

            VkExtent2D actualExtent{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
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
        bool extensionSupported = CheckDeviceExtensionSupported(device);

        //! @note Verify that swapchain suport is adequate.
        //! @note Swapchain support is sufficient if there is at least one supportyed image format and one supported presentation modes with our given surface
        bool swapchainAdequate = false;
        if(extensionSupported){
            SwapChainSupportDetails swapchainSupport = QuerySwapchainSupport(device);
            swapchainAdequate = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
        }

        return indices.IsComplete() and extensionSupported and swapchainAdequate;
    }

    void VulkanDevice::InitializeVulkanDevices(){
        InitializeSurface();
        PickPhysicalDevice();
        InitializeLogicalDevice();
        InitializeSwapchain();
    }

    void VulkanDevice::CleanupVulkanDevice(){
        vkDestroySwapchainKHR(VulkanPipeline::GetVulkanProperties().logicalDevice, VulkanPipeline::GetVulkanProperties().swapChain, nullptr);
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

        //! @note To enable swapchains we enable our device extensions.
        createInfo.enabledExtensionCount = static_cast<uint32_t>(VulkanPipeline::DeviceExtensions().size());
        createInfo.ppEnabledExtensionNames = VulkanPipeline::DeviceExtensions().data();
         
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

    void VulkanDevice::InitializeSwapchain(){
        auto properties = VulkanPipeline::GetVulkanProperties();
        //! @note Creating our swapchain.
        SwapChainSupportDetails swapChainSupport = QuerySwapchainSupport(properties.physicalDevice);

        VkSurfaceFormatKHR surfaceFormat = SelectSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = SelectSwapchainPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = SelectSwapExtent(swapChainSupport.capabilities);

        //! @note Deciding how many images we'd like our swapchain to have.
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

        //! 2note Make sure to not exceed max num of images 
        if(swapChainSupport.capabilities.maxImageCount > 0 and imageCount > swapChainSupport.capabilities.maxImageCount){
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        //! @note Creating swapchain properties
        //! @param imageArrayLayers: specifies amoung of layers each image consists of
        //! @param imageUsage: bit field specifies what kind of operations we'll use the images in swapchain for.
        //! @param VK_IMAGE_USAGE_TRANSFER_DST_BIT: its possible to render images to a separate image first to perform operations like post-processing. In this case use this flag valuie instead of memory operations.
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = properties.surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = FindQueueFamilies(properties.physicalDevice);
        uint32_t queueFamIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        /**
         * @param VK_SHARING_MODE_EXCLUSIVE: : An image is owned by one queue family at a time and ownership must be explicitly transferred before using it in another queue family. This option offers the best performance
         * @param VK_SHARING_MODE_CONCURRENT: Images can be used across multiple queue families without explicit ownership transfers.
        */
        if(indices.graphicsFamily != indices.presentFamily){
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamIndices;
        }
        else{
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if(vkCreateSwapchainKHR(properties.logicalDevice, &createInfo, nullptr, &properties.swapChain) != VK_SUCCESS){
            CoreLogError("Failed to create swapchain!");
        }

        vkGetSwapchainImagesKHR(properties.logicalDevice, properties.swapChain, &imageCount, nullptr);
        vkGetSwapchainImagesKHR(properties.logicalDevice, properties.swapChain, &imageCount, properties.swapchainImages.data());
        properties.swapChainImageFormat = surfaceFormat.format;
        properties.swapChainExtent = extent;
    }
};