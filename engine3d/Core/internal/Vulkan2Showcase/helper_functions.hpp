#pragma once
#include <string>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    // const char* vk_to_string(VkResult res);
    //! @note Terminates if the result was unsuccessful!
    //! @note TODO --- We shouldn't std::terminate, look into alternatives.
    void vk_check(VkResult p_Result, const char* p_Tag, const char* p_Filename, uint32_t p_LineName, const char* p_FunctionName);

    void vk_check_format(VkFormat p_Format, const char* p_Filename, uint32_t p_LineName, const char* p_FunctionName);

    std::string vk_queue_flags_to_string(VkQueueFlagBits flags);
};