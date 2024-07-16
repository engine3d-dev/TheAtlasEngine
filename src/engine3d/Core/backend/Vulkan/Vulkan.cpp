#include <Core/backend/Vulkan/Vulkan.h>
#include <Core/EngineLogger.h>
#include <Core/backend/utilities/helper_functions.h>
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace engine3d{
    static std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    //! @note Used for checking for swapchain support.
    //! @note Not all graphics cards have capabilities of presenting images directly to screen.
    static const std::vector<const char*> deviceExtensions{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    //! @note Checking for validation layers
    //! @note A way for Vulkan to have minimal driver overhead and limited error checking by default for the API.
#ifdef DEBUG_BUILD
        static const bool enableValidationLayers = false;
#else
    static const bool enableValidationLayers = true;
#endif

    //! @note Check if requested validation layers are available
    //! @note Using vkEnumerate* to list available validation layers.
    static bool CheckValidationLayerSupport(){
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for(const char* name : validationLayers){
            bool layerFound = false;

            for(const auto& layerProperties : availableLayers){
                // if(name == layerProperties.layerName){
                if(strcmp(name, layerProperties.layerName) == 0){
                    layerFound = true;
                    break;
                }
            }

            if(!layerFound){
                return false;
            }
        }
        return true;
    }

    //! @note Vulkan message callback
    std::vector<const char*> GetRequiredExtensions(){
        // uint32_t  glfwExtensionCount = 0;
        // const char** glfwExtensions;
        // glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        // std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        // if(enableValidationLayers){
        //     extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        // }

        // return extensions;
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    /**
    @param messageSeverity parameter specifies the following severity of the message.
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: Diagnostic message
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: Informational message like the creation of a resource
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: Message about behavior that is not necessarily an error, but very likely a bug in your application
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: Message about behavior that is invalid and may cause crashes

    @param messageType parameter can have following values
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: Some event has happened that is unrelated to the specification or performance
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: Something has happened that violates the specification or indicates a possible mistake
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: Potential non-optimal use of Vulkan

    @param pCallbackData refer to VkDebugUtilsMessengerCallbackDataEXT, details of message itself.
    pMessage: The debug message as a null-terminated string
    pObjects: Array of Vulkan object handles related to the message
    objectCount: Number of objects in array

    @param pUserData parameter, contains a pointer that was specified during setup of callbacks and allows to pass your own data to it.
    */
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT message_t, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData){
        CoreLogWarn("Validation Layers: {}", pCallbackData->pMessage);

        if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT){
        }
        
        return VK_FALSE;
    }

    //! @note Used for creating our debug utils messenger handler
    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger){
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

        if(func != nullptr){
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else{
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    //! @note Uesd for cleaning up our debug utils messenger handler
    static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator){
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

        if(func != nullptr){
            func(instance, debugMessenger, pAllocator);
        }
    }

    static VulkanProperties g_properties;

    //! @note Getters
    VulkanProperties& VulkanPipeline::GetVulkanProperties() { return g_properties; }

    GLFWwindow* VulkanPipeline::GetCurrentWindow(){ return g_properties.glfwWindowInstance; }

    WindowApplicationProperties VulkanPipeline::GetWindowData() { return g_properties.windowProperties; }

    void VulkanPipeline::InitializeVulkan(){
        if(!glfwInit()){
            CoreLogError("glfwInit() failed to initialize GLFW!");
            assert(false);
        }

        int vulkanLoaderStatus = glfwVulkanSupported();
        if(!vulkanLoaderStatus){
            CoreLogError("vulkan loader and ICD was not found on this current device!");
        }

        //! @note TODO --- Setting our window dimensions to be based on our current window's monitor dimensions.
        // glfwGetMonitorP
        // GLFWvidmode currentWindow;
        // glfwGetDesktopMode(&currentWindow);

        g_properties.windowProperties.width = 800;
        g_properties.windowProperties.height = 600;
        g_properties.windowProperties.title = "Engine3D";

        // GLFWvidmode* mode = (GLFWvidmode *)glfwGetVideoMode(glfwGetPrimaryMonitor());

        // glfwInitHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        g_properties.glfwWindowInstance = glfwCreateWindow(g_properties.windowProperties.width, g_properties.windowProperties.height, g_properties.windowProperties.title.c_str(), nullptr, nullptr);

        glfwMakeContextCurrent(g_properties.glfwWindowInstance);

        if(!g_properties.glfwWindowInstance){
            CoreLogError("glfwWindowInstance window pointer was nullptr! Make sure glfwInit() initialized!");
            assert(false);
        }

        SetupVulkanInstance();
    }

    void VulkanPipeline::CleanVulkanPipeline(){
        if(enableValidationLayers){
            DestroyDebugUtilsMessengerEXT(g_properties.instance, g_properties.debugMessenger, nullptr);
        }
        
        //! @note Before destroying the instance make sure other vulkan API things have been destroyed prior.
        vkDestroyInstance(g_properties.instance, nullptr);
        
        glfwDestroyWindow(g_properties.glfwWindowInstance);
        glfwTerminate();
    }

    void VulkanPipeline::SetupVulkanInstance(){
        //! @note Initiating Vulkan to let the API know what our applications initiating phase will be.
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Engine3D";
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_API_VERSION(1, 3, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        //! @note Validation layers print debug mesages to stdout by default, these messages are also how we can handle them ourselves
        //! @note This should be called to be handled before instantiating in vulkan.
        auto extensions = GetRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        //! @note Setting up Vulkan instance that will be for initiating the Vulkan API.
        //! @note We do not have a customized allocator for now we set that param to a nullptr.
        VkResult res = vkCreateInstance(&createInfo, nullptr, &g_properties.instance);
        if(res != VK_SUCCESS){
            CoreLogError("vkCreateInstance() was not successful in Vulkan.cpp");
            assert(false);
        }

        //! @note Checking enumeration extension support.
        //! @note We should enable this during debug build, so we are not constantly running specific code portions during release builds.
#ifdef DEBUG_BUILD
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extensions(extensionCount);

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        CoreLogWarn("Debug Build -- available extensions = ");
        for(const auto& extension : extensions){
            CoreLogInfo("\t{}", extension.extensionName);
        }
#endif
        if(enableValidationLayers && !CheckValidationLayerSupport()){
            CoreLogError("Requested validation layers not available!");
            assert(false);
        }

        
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        //! @note Checking if validation layers are enabled for debugging
        if(enableValidationLayers){
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data(); // Deprecated

            //! @note Setting debugCreateInfo params
            debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
            debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debugCreateInfo.pfnUserCallback = DebugCallback;
            debugCreateInfo.pUserData = nullptr; // Optional

            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
        }
        else{
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        SetupDebugMessenger();
    }

    void VulkanPipeline::SetupDebugMessenger(){
        if(!enableValidationLayers) return;

        /**
        @param messageSeverity allows to specifies types of severities options the callback to be called for.
        @param messageType filters types of messages the callbacks notified for.
        @param pfnUserCallback is the field to specify the callback function.
        @param pUserData field is used to ppassed along the callback function.
        */
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugCallback;
        createInfo.pUserData = nullptr; // Optional

        //! @note Actual creation of our debug utils messenger handler
        VkResult res = CreateDebugUtilsMessengerEXT(g_properties.instance, &createInfo, nullptr, &g_properties.debugMessenger);

        if(res != VK_SUCCESS){
            CoreLogError("Failed to setup debug messenger");
        }
    }

    bool VulkanPipeline::EnableValidationLayer(){ return enableValidationLayers; }
    std::vector<const char*> VulkanPipeline::ValidationLayers(){ return validationLayers; }
    std::vector<const char*> VulkanPipeline::DeviceExtensions(){ return deviceExtensions; }
};