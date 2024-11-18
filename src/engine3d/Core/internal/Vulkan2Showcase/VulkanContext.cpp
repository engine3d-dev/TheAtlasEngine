#include <Core/EngineLogger.hpp>
#include <Core/internal/Vulkan2Showcase/Vulkan.hpp>
#include <internal/Vulkan2Showcase/VulkanContext.hpp>
#include <internal/Vulkan2Showcase/helper_functions.hpp>

namespace engine3d::vk{
    const std::vector<std::string> InstanceLayers = {
// #ifdef DEBUG
	// Khronos Validation is a layer which encompasses all of the functionality that used to be contained in VK_LAYER_GOOGLE_threading,
	// VK_LAYER_LUNARG_parameter_validation, VK_LAYER_LUNARG_object_tracker, VK_LAYER_LUNARG_core_validation, and VK_LAYER_GOOGLE_unique_objects
	"VK_LAYER_KHRONOS_validation",
	// Standard Validation is a (now deprecated) meta-layer managed by the LunarG Loader.
	// Using Standard Validation will cause the loader to load a standard set of validation layers in an optimal order:
	// * VK_LAYER_GOOGLE_threading.
	// * VK_LAYER_LUNARG_parameter_validation.
	// * VK_LAYER_LUNARG_object_tracker.
	// * VK_LAYER_LUNARG_core_validation.
	// * VK_LAYER_GOOGLE_unique_objects.
	"VK_LAYER_LUNARG_standard_validation",
	// PerfDoc is a Vulkan layer which attempts to identify API usage that may be discouraged, primarily by validating applications
	// against the rules set out in the Mali Application Developer Best Practices document.
	"VK_LAYER_ARM_mali_perf_doc"
// #else
// 	""
// #endif
};
    VkInstance VulkanContext::s_Instance = VK_NULL_HANDLE;
    VulkanPhysicalDriver VulkanContext::s_PhysicalDriver;
    VulkanDriver VulkanContext::s_Driver;

    void VulkanContext::Initialize(){
        ConsoleLogInfo("Vulkan2Showcase: Begin VulkanContext::Initialize()!!");
        VkApplicationInfo app_info = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .applicationVersion = 1,
            .pEngineName = "Engine3D",
            .engineVersion = 1,
            .apiVersion = VK_API_VERSION_1_0,
        };

        VkInstanceCreateInfo instance_create_info = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &app_info
        };

        std::vector<const char*> validation_layers=  InitializeValidationLayers();
        std::vector<const char*> instance_extensions = InitializeInstanceExtensions();

        instance_create_info.enabledExtensionCount = static_cast<uint32_t>(validation_layers.size());
        instance_create_info.ppEnabledLayerNames = validation_layers.data();

        //! @note Setting our instance extensions that will get applied to instance-level objects like VkInstance, VkPhysicalDevice, etc.
        instance_create_info.enabledExtensionCount = static_cast<uint32_t>(instance_extensions.size());
        instance_create_info.ppEnabledExtensionNames = instance_extensions.data();

        vk_check(vkCreateInstance(&instance_create_info, nullptr, &s_Instance), "vkCreateInstance", __FILE__, __LINE__, __FUNCTION__);
        ConsoleLogWarn("Vulkan2Showcase: VkInstance Initialized Completed!!!");
        
        s_PhysicalDriver = VulkanPhysicalDriver(s_Instance);
        s_Driver = VulkanDriver(s_PhysicalDriver);
    }

    //! @note Returns the validation layers that will be utilized by the vulkan instance.
    std::vector<const char*> VulkanContext::InitializeValidationLayers(){
        uint32_t layer_count;

        //! @note Validation layers that will be returned.
        std::vector<const char*> layer_names;

        //! @note Enumerating the layer size
        vk_check(vkEnumerateInstanceLayerProperties(&layer_count, nullptr), "vkEnumerateInstanceLayerProperties (1)", __FILE__, __LINE__, __FUNCTION__);
        std::vector<VkLayerProperties> layer_properties;
        layer_properties.resize(layer_count);

        vk_check(vkEnumerateInstanceLayerProperties(&layer_count, layer_properties.data()), "vkEnumerateInstanceLayerProperties (1)", __FILE__, __LINE__, __FUNCTION__);

        for(const auto& layer : layer_properties){
            for(uint32_t j = 0; j < layer_count; j++){
                // if(layer_properties[j].layerName == layer.layerName)
                if(strcmp(layer_properties[j].layerName, layer.layerName)){
                    layer_names.emplace_back(layer_properties[j].layerName);
                }
            }
        }
        return layer_names;
    }

    std::vector<const char*> VulkanContext::InitializeInstanceExtensions(){
        std::vector<const char*> extensionNames;

        extensionNames.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);

        // An additional surface extension needs to be loaded. This extension is platform-specific so needs to be selected based on the
        // platform the example is going to be deployed to.
        // Preprocessor directives are used here to select the correct platform.
    #ifdef VK_USE_PLATFORM_WIN32_KHR
        extensionNames.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    #endif
    #ifdef VK_USE_PLATFORM_XLIB_KHR
        extensionNames.emplace_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
    #endif
    #ifdef VK_USE_PLATFORM_XCB_KHR
        extensionNames.emplace_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
    #endif
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
        extensionNames.emplace_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
    #endif
    #ifdef VK_USE_PLATFORM_WAYLAND_KHR
        extensionNames.emplace_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
    #endif
    #ifdef VK_USE_PLATFORM_MACOS_MVK
        extensionNames.emplace_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
    #endif
    #ifdef USE_PLATFORM_NULLWS
        extensionNames.emplace_back(VK_KHR_DISPLAY_EXTENSION_NAME);
    #endif

        return extensionNames;
    }
};