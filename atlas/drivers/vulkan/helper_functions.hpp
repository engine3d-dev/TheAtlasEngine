#pragma once
#include <string>
#include <vulkan/vulkan_core.h>

namespace atlas::vk {
    // const char* vk_to_string(VkResult res);
    //! @note Terminates if the result was unsuccessful!
    //! @note TODO --- We shouldn't std::terminate, look into alternatives.
    void vk_check(VkResult p_result,
                  const char* p_tag,
                  const char* p_filepath,
                  uint32_t p_line,
                  const char* p_function_name);

    void vk_check_format(VkFormat p_format,
                         const char* p_filepath,
                         uint32_t p_line,
                         const char* p_function_name);

    std::string vk_queue_flags_to_string(VkQueueFlagBits p_flags);
};