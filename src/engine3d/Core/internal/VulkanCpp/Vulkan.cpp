#include <Core/internal/VulkanCpp/Vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vector>
#include <stdexcept>
#include <Core/internal/VulkanCpp/VulkanDevice.h>

namespace engine3d::vk{
#if defined(_WIN32)
    static std::vector<const char *> extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        "VK_KHR_win32_surface"
    };
#elif defined(__APPLE__)
    static std::vector<const char*> extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        "VK_KHR_surface",
        "VK_EXT_metal_surface",

        VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
    };
#elif defined(__linux__)
    static std::vector<const char*> extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        "VK_KHR_xcb_surface",
    };
#else // this is if we aren't on Win32, Mac, or even linux for some odd reason.
#endif

    static std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation",
        // "VK_LAYER_LUNARG_api_dump",
        // "VK_LAYER_KHRONOS_profiles",
        // "VK_LAYER_KHRONOS_synchronization2",
        // "VK_LAYER_KHRONOS_shader_object",
    };

    VkInstance g_Instance;

    void Vulkan::InitializeVulkanCore(){
        //! @note to initialize vulkan we need to first specify our application properties.
        //! @note Initialize vulkan's instance information for instantiation.
        VkApplicationInfo appInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "Engine3D",
            .pEngineName = "Engine3D",
            .engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0),
            .apiVersion = VK_API_VERSION_1_0
        };

        VkInstanceCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &appInfo,
            .enabledLayerCount = static_cast<uint32_t>(validationLayers.size()),
            .ppEnabledLayerNames = validationLayers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data()
        };

        VkResult res = vkCreateInstance(&createInfo, nullptr, &g_Instance);
        if(res != VK_SUCCESS){
            throw std::runtime_error("vkCreateInstance errored message ===> ");
        }

        if(g_Instance == VK_NULL_HANDLE){
            printf("Vulkan VkInstance::g_Instance === nullptr\n");
        }

        //! @note Setting the debug callback
    }

    void Vulkan::CleanupVulkanCore(){
    }

    VkInstance& Vulkan::GetVkInstance(){
        if(g_Instance == VK_NULL_HANDLE){
            printf("Vulkan VkInstance::g_Instance (from GetVkInstance)=== nullptr\n");
        }
        return g_Instance;
    }
};