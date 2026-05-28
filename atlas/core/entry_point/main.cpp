// For glfwInit call
#include <GLFW/glfw3.h>
#include <memory_resource>
#include <print>
#include <vector>
#include <expected>

#define GLFW_INCLUDE_VULKAN
#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#else
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#endif

import atlas.application;
import atlas.common;
import atlas.logger;
import atlas.graphics_api;
import atlas.drivers.graphics_context;
import atlas.renderer.context_loader;
import atlas.core.event;
import vk;

// Defined in the user-application side
[[nodiscard]] atlas::ref<atlas::application> initialize_application(
  std::shared_ptr<atlas::graphics_context> p_context,
  atlas::event::bus& p_bus);

static VKAPI_ATTR VkBool32 VKAPI_CALL
debug_callback(
  [[maybe_unused]] VkDebugUtilsMessageSeverityFlagBitsEXT p_message_severity,
  [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT p_message_type,
  const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
  [[maybe_unused]] void* p_user_data) {
    std::print("validation layer:\t\t{}\n\n", p_callback_data->pMessage);
    return false;
}

std::vector<const char*>
get_instance_extensions() {
    std::vector<const char*> extension_names;
    uint32_t extension_count = 0;
    const char** required_extensions =
      glfwGetRequiredInstanceExtensions(&extension_count);

    for (uint32_t i = 0; i < extension_count; i++) {
        std::println("Required Extension = {}", required_extensions[i]);
        extension_names.emplace_back(required_extensions[i]);
    }

    extension_names.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

#if defined(__APPLE__)
    extension_names.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    extension_names.emplace_back(
      VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#endif

    return extension_names;
}

int
main() {
    // We should not have our core system start up during testing environment
    // execution
    atlas::console_log_manager manager = atlas::console_log_manager();
    if (!glfwInit()) {
        console_log_fatal("GLFW: Initialization failed!!");
        return -1;
    }

    if (!glfwVulkanSupported()) {
        std::println("GLFW: Vulkan is not supported!");
        return -1;
    }

    std::array<const char*, 1> validation_layers = {
        "VK_LAYER_KHRONOS_validation",
    };

    // Retrieving the Vulkan instance-level extensions
    std::vector<const char*> global_extensions = get_instance_extensions();

    vk::debug_message_utility debug_callback_info = {
        // .severity essentially takes in vk::message::verbose,
        // vk::message::warning, vk::message::error
        .severity =
          vk::message::verbose | vk::message::warning | vk::message::error,
        // .message_type essentially takes in vk::debug. Like:
        // vk::debug::general, vk::debug::validation, vk::debug::performance
        .message_type =
          vk::debug::general | vk::debug::validation | vk::debug::performance,
        .callback = debug_callback
    };

    vk::application_params config = {
        .name = "vulkan instance",
        .version = vk::api_version::vk_1_3, // specify to using vulkan 1.3
        .validations =
          validation_layers, // .validation takes in a std::span<const char*>
        .extensions =
          global_extensions // .extensions also takes in std::span<const char*>
    };

    // 1. Setting up vk instance
    vk::instance api_instance(config, debug_callback_info);
    std::expected<vk::physical_device, VkResult> physical_device_expected =
      api_instance.enumerate_physical_device(vk::physical_gpu::integrated);
    vk::physical_device physical_device = physical_device_expected.value();

    std::array<float, 1> priorities = { 0.f };
#if defined(__APPLE__)
    std::array<const char*, 2> extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        "VK_KHR_portability_subset",
    };
#else
    std::array<const char*, 1> extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };
#endif
    std::array<vk::format, 3> format_support = {
        vk::format::d32_sfloat,
        vk::format::d32_sfloat_s8_uint,
        vk::format::d24_unorm_s8_uint
    };

    // We provide a selection of format support that we want to check is
    // supported on current hardware device.
    VkFormat depth_format =
      physical_device.request_depth_format(format_support);

    vk::device_features device_features{
        vk::dynamic_rendering_feature{ {
          .dynamicRendering = true,
        } },
    };

    vk::device_params logical_device_params = {
        .features = device_features.data(),
        .queue_priorities = priorities,
        .extensions = extensions,
        .queue_family_index = 0,
    };

    // pmr::monotonic_memory_resource for allocation

    std::array<uint8_t, 2048> bytes{};
    std::pmr::monotonic_buffer_resource memory_resource{ bytes.data(),
                                                         bytes.size() };
    std::pmr::polymorphic_allocator<uint8_t> allocator{ &memory_resource };

    std::shared_ptr<vk::device> logical_device =
      std::allocate_shared<vk::device>(
        allocator, physical_device, logical_device_params);

    std::shared_ptr<atlas::graphics_context> context =
      std::allocate_shared<atlas::graphics_context>(
        allocator, api_instance, physical_device, logical_device);

    atlas::event::bus event_bus;
    atlas::ref<atlas::application> app =
      initialize_application(context, event_bus);

    app->execute();

    app->post_destroy();

    context->post_cleanup();
    return 0;
}