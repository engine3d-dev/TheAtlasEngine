#include <drivers/vulkan-cpp/utilities.hpp>
#include <vulkan/vulkan_core.h>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <core/engine_logger.hpp>
#include <filesystem>

namespace atlas::vk {

    void vk_check(const VkResult& p_result,
                  const std::string& p_name,
                  const std::source_location& p_source) {
        if (p_result != VK_SUCCESS) {
            console_log_error_tagged(
              "vulkan",
              "File {} on line {} failed VkResult check",
              std::filesystem::relative(p_source.file_name()).string(),
              p_source.line());
            console_log_error_tagged("vulkan",
                                     "Current Function Location = {}",
                                     p_source.function_name());
            console_log_error_tagged(
              "vulkan", "{} VkResult returned: {}", p_name, (int)p_result);
        }
    }
};