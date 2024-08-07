#include <Core/backend/Vulkan/Vulkan.h>
#include <Core/EngineLogger.h>
#include <Core/backend/utilities/helper_functions.h>
#include <vulkan/vulkan_core.h>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>

namespace engine3d::vk{
    // namespace vk{

    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT Severity,
        VkDebugUtilsMessageTypeFlagsEXT Type,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
    ){
        ConsoleLogWarn("Debug callback: {}\n", pCallbackData->pMessage);
        ConsoleLogWarn("\tSeverity {}", GetDebugSeverity(Severity));
        ConsoleLogWarn("\tType {}", GetDebugType(Type));
        ConsoleLogWarn("\tObjects");

        for(uint32_t i = 0; i < pCallbackData->objectCount; i++){
            ConsoleLogWarn("{} ", pCallbackData->pObjects[i].objectHandle);
        }

        return false;
    }

    static std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    static std::vector<const char *> extensions{
        VK_KHR_SURFACE_EXTENSION_NAME, // Used to bind our window to vulkan
#if defined(_WIN32)
        "VK_KHR_win32_surface",
#elif defined(__APPLE__)
        "VK_MVK_macos_surface",
#elif defined(__linux__)
        "VK_KHR_xcb_surface",
#endif
        // enables callback to our application when errors happen in vulkan
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    };

    //! @note Properties for our current window application.
    struct WindowApplicationProperties{
        uint32_t width, height;
        std::string title;
    };
    
    //! @note TODO -- Would be better to rename VulkanProperties to VulkanContext
    //! @note Since this is really what it is doing
    struct VulkanProperties{
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        VkInstance instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
        WindowApplicationProperties windowProperties;
        GLFWwindow* glfwWindowInstance;
    };

    static VulkanProperties g_properties;

    void VulkanPipeline::InitializePipeline(){
        if(!glfwInit()){
            ConsoleLogError("glfwInit() failed to initialize GLFW!");
            assert(false);
        }

        if(!glfwVulkanSupported()){
            ConsoleLogError("vulkan loader and ICD was not found on this current device!");
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
            ConsoleLogError("glfwWindowInstance window pointer was nullptr! Make sure glfwInit() initialized!");
            assert(false);
        }

        VkApplicationInfo appInfo{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = g_properties.windowProperties.title.c_str(),
            .pEngineName = g_properties.windowProperties.title.c_str(),
            .engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0),
            .apiVersion = VK_API_VERSION_1_0
        };

        VkInstanceCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0, // reserve for future use
            .pApplicationInfo = &appInfo,
            .enabledLayerCount = (uint32_t)validationLayers.size(), //! list of extensions. Up to app developer to make sure if these are supported
            .ppEnabledLayerNames = validationLayers.data(),
            .enabledExtensionCount = (uint32_t)extensions.size(),
            .ppEnabledExtensionNames = extensions.data()
        };

        //! @note Setting up Vulkan instance that will be for initiating the Vulkan API.
        //! @note We do not have a customized allocator for now we set that param to a nullptr.
        VkResult res = vkCreateInstance(&createInfo, nullptr, &g_properties.instance);
        
        if(res != VK_SUCCESS){
            ConsoleLogError("vkCreateInstance() was not successful in InitializePipeline() in file Vulkan.cpp");
            assert(false);
        }

        CreateDebugMessenger();
        CreateSurface();
    }

    void VulkanPipeline::CleanupPipeline(){
        // vkDestroyDebugUtilsMessenger(g_properties.instance, g_properties.debugMessenger, nullptr);
        PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessenger = VK_NULL_HANDLE;
        vkDestroyDebugUtilsMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(g_properties.instance, "vkDestroyDebugUtilsMessengerEXT");
        vkDestroyDebugUtilsMessenger(g_properties.instance, g_properties.debugMessenger, nullptr);
        if(!vkDestroyDebugUtilsMessenger){
            ConsoleLogError("Cannot fnid address of vkDestroyDebugUtilsMessenger");
        }
        
        PFN_vkDestroySurfaceKHR vkDestroySurface = VK_NULL_HANDLE;
        vkDestroySurface = (PFN_vkDestroySurfaceKHR)vkGetInstanceProcAddr(g_properties.instance, "vkDestroySurfaceKHR");

        if(!vkDestroySurface){
            ConsoleLogError("Cannot find address of vkDestroySurfaceKHR");
        }

        vkDestroyInstance(g_properties.instance, nullptr);
    }
    
    void VulkanPipeline::CreateDebugMessenger(){
        VkDebugUtilsMessengerCreateInfoEXT debugMsgCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext = nullptr,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = &DebugCallback,
            .pUserData = nullptr
        };
    }

    void VulkanPipeline::CreateSurface(){
        if(glfwCreateWindowSurface(g_properties.instance, g_properties.glfwWindowInstance, nullptr, &g_properties.surface) != VK_SUCCESS){}
    }

    // VulkanProperties& VulkanPipeline::GetVulkanProperties(){ return g_properties; }

    GLFWwindow* VulkanPipeline::GetCurrentWindow(){
        return g_properties.glfwWindowInstance;
    }

    VkInstance& VulkanPipeline::GetVkInstance(){ return g_properties.instance; }

    VkSurfaceKHR& VulkanPipeline::GetVkSurface(){ return g_properties.surface; }

    std::string& VulkanPipeline::GetApplicationTitle() {
        return g_properties.windowProperties.title;
    }

    uint32_t VulkanPipeline::GetWidth() {
        return g_properties.windowProperties.width;
    }

    uint32_t VulkanPipeline::GetHeight() {
        return g_properties.windowProperties.height;
    }

    // };

};