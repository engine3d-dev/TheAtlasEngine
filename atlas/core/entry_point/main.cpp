// For glfwInit call
#include <GLFW/glfw3.h>
#include <memory_resource>
#include <memory>
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
import atlas.core.utilities;
import atlas.drivers.vulkan;
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
        .severity =
          vk::message::verbose | vk::message::warning | vk::message::error,
        .message_type =
          vk::debug::general | vk::debug::validation | vk::debug::performance,
        .callback = debug_callback
    };

    vk::application_params config = {
        .name = "vulkan instance",
        // specify to using vulkan 1.4
        .version = vk::api_version::vk_1_4,
        // .validation takes in a std::span<const char*>
        .validations =
          validation_layers,
        // .extensions also takes in std::span<const char*>
        .extensions =
          global_extensions
    };

    // 1. Setting up vk instance
    vk::instance api_instance(config, debug_callback_info);
#if defined(__APPLE__)
    std::expected<vk::physical_device, VkResult> physical_device_expected =
      api_instance.enumerate_physical_device(vk::physical_gpu::integrated);
#else
    std::expected<vk::physical_device, VkResult> physical_device_expected =
      api_instance.enumerate_physical_device(vk::physical_gpu::discrete);
#endif
    vk::physical_device physical_device = physical_device_expected.value();

    std::array<float, 1> priorities = { 0.f };
#if defined(__APPLE__)
    std::array<const char*, 3> extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        "VK_KHR_portability_subset",
        "VK_KHR_dynamic_rendering",
    };
#else
    std::array<const char*, 2> extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        "VK_KHR_dynamic_rendering",
    };
#endif

    vk::device_features device_features{
        vk::dynamic_rendering_feature{ {
          .dynamicRendering = true,
        } },
        vk::descriptor_indexing_feature{ {
          .shaderSampledImageArrayNonUniformIndexing = true,
          .descriptorBindingSampledImageUpdateAfterBind = true,
          .descriptorBindingPartiallyBound = true,
          .descriptorBindingVariableDescriptorCount = true,
          .runtimeDescriptorArray = true,
        } },
        vk::buffer_device_address{ {
          .bufferDeviceAddress = true,
        } },
    };

    vk::device_params logical_device_params = {
        .features = device_features.data(),
        .queue_priorities = priorities,
        .extensions = extensions,
        .queue_family_index = 0,
    };

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

    logical_device->wait();

    app->post_destroy();

    context->post_cleanup();
    return 0;
}