#include <drivers/vulkan-cpp/vulkan-imports.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <core/engine_logger.hpp>
#include <iostream>
#include <drivers/vulkan/helper_functions.hpp>

namespace atlas::vk {
    std::vector<const char*> initialize_instance_extensions() {
        std::vector<const char*> extension_names;

        extension_names.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);

        // An additional surface extension needs to be loaded. This extension is
        // platform-specific so needs to be selected based on the platform the
        // example is going to be deployed to. Preprocessor directives are used
        // here to select the correct platform.
#ifdef VK_USE_PLATFORM_WIN32_KHR
        extension_names.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
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

        return extension_names;
    }
    const std::vector<const char*> validation_layers = {
        "VK_LAYER_KHRONOS_validation",
        "VK_LAYER_KHRONOS_synchronization2"
    };

    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
      [[maybe_unused]] VkDebugUtilsMessageSeverityFlagBitsEXT
        p_message_severity,
      [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT p_message_type,
      const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
      [[maybe_unused]] void* p_user_data) {
        std::cerr << "validation layer:\t\t" << p_callback_data->pMessage
                  << std::endl;
        return false;
    }

    vk_context* vk_context::s_instance = nullptr;

    vk_context::vk_context(const std::string& p_tag) {
        console_log_manager::create_new_logger(p_tag);

        console_log_info("vk_context begin initialization!!!");

        VkApplicationInfo app_info = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .applicationVersion = 1,
            .pEngineName = "TheAtlasEngine",
            .engineVersion = 1,
            .apiVersion = VK_API_VERSION_1_3,
        };

        VkInstanceCreateInfo create_info = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &app_info
        };

        //! @note Setting up the required extensions for vulkan
        std::vector<const char*> extensions = initialize_instance_extensions();
        extensions.push_back("VK_EXT_debug_utils");
        create_info.enabledExtensionCount =
          static_cast<uint32_t>(extensions.size());
        create_info.ppEnabledExtensionNames = extensions.data();

        // by default we enable validation layers used for debugging!
        create_info.enabledLayerCount =
          static_cast<uint32_t>(validation_layers.size());
        create_info.ppEnabledLayerNames = validation_layers.data();

        // printing out available validation layers
        uint32_t layer_count;
        std::vector<VkLayerProperties> available_validation_layers;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

        available_validation_layers.resize(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count,
                                           available_validation_layers.data());

        console_log_trace("================================================");
        console_log_trace("\tValidation Layers Available");
        console_log_trace("================================================");
        for (VkLayerProperties properties : available_validation_layers) {
            console_log_trace("Validation Layer:\t\t{}", properties.layerName);
            console_log_trace("Description\t\t{}", properties.description);
            console_log_trace("Version\t\t\t{}", (int)properties.specVersion);
        }

        console_log_trace("================================================");

        VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = debug_callback,
        };

        create_info.pNext =
          (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;

        vk_check(vkCreateInstance(&create_info, nullptr, &m_instance_handler),
                 "vkCreateInstance",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        console_log_info("vk_context end initialization!!!\n\n");

        s_instance = this;

        m_physical = vk_physical_driver(m_instance_handler);
        m_driver = vk_driver(m_physical);
    }

    VkInstance vk_context::handler() {
        return s_instance->m_instance_handler;
    }

    void vk_context::resource_free(const std::function<void()>& p_resource) {
        m_resources_free.push_front(p_resource);
    }

    void vk_context::submit_resource_free(const std::function<void()>& p_resource) {
        s_instance->resource_free(p_resource);
    }

    void vk_context::destroy_context() {
        console_log_info("vk_context::destroy_context() called!!");
        for(auto& callback : m_resources_free) {
            callback();
        }
    }
};