#include <core/engine_logger.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <vulkan/vulkan_core.h>

namespace atlas::vk {
    void vk_check(VkResult result,
                  const char* p_tag,
                  const char* p_filepath,
                  uint32_t p_line,
                  const char* p_function_name) {
        if (result != VK_SUCCESS) {
            console_log_info_tagged("vulkan",
                                    "VkResult failed taking in {0} file: {1} "
                                    "--- Line: {2} --- In Function: {3}",
                                    p_tag,
                                    p_filepath,
                                    p_line,
                                    p_function_name);
            console_log_info_tagged(
              "vulkan", "VkResult returned: {}", (int)result);
        }
    }

    void vk_check_format(VkFormat p_format,
                         const char* p_filepath,
                         uint32_t p_line,
                         const char* p_function_name) {
        if (p_format == VK_FORMAT_UNDEFINED) {
            console_log_info_tagged(
              "vulkan",
              "VkFormat failed in file: {0} --- Line: {1} --- In Function: {2}",
              p_filepath,
              p_line,
              p_function_name);
            console_log_info_tagged(
              "vulkan", "VkFormat was assigned to VK_FORMAT_UNDEFINED");
        }
    }

    std::string vk_queue_flags_to_string(VkQueueFlagBits p_flags) {
        switch (p_flags) {
            case VK_QUEUE_GRAPHICS_BIT:
                return "VK_QUEUE_GRAPHICS_BIT";
            case VK_QUEUE_COMPUTE_BIT:
                return "VK_QUEUE_COMPUTE_BIT";
            case VK_QUEUE_TRANSFER_BIT:
                return "VK_QUEUE_TRANSFER_BIT";
            case VK_QUEUE_SPARSE_BINDING_BIT:
                return "VK_QUEUE_SPARSE_BINDING_BIT";
            case VK_QUEUE_PROTECTED_BIT:
                return "VK_QUEUE_PROTECTED_BIT";
            case VK_QUEUE_VIDEO_DECODE_BIT_KHR:
                return "VK_QUEUE_VIDEO_DECODE_BIT_KHR";
            case VK_QUEUE_VIDEO_ENCODE_BIT_KHR:
                return "VK_QUEUE_VIDEO_ENCODE_BIT_KHR";
            case VK_QUEUE_OPTICAL_FLOW_BIT_NV:
                return "VK_QUEUE_OPTICAL_FLOW_BIT_NV";
            case VK_QUEUE_FLAG_BITS_MAX_ENUM:
                return "VK_QUEUE_FLAG_BITS_MAX_ENUM";
        }

        return "VkQueueFlag not Selected";
    }
};