#pragma once
#include <string>
#include <vulkan/vulkan_core.h>
#include <source_location>

namespace atlas::vk {

    /**
     * @param p_result checks if the result of a vulkan handler was created
     * correctly
     * @param p_name used for debugging of which handler failed
     * @param p_source is the location of the call-site that invoked vk_check
     */
    void vk_check(
      const VkResult& p_result,
      const std::string& p_name,
      const std::source_location& p_source = std::source_location::current());

};