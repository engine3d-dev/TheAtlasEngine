module;

#include <string>
#include <vulkan/vulkan_core.h>

export module atlas.drivers.vulkan.utilities;
import atlas.logger;

export namespace atlas {
    namespace vulkan {

        void vk_check(const VkResult& p_result,
                  const std::string& p_name) {
            if (p_result != VK_SUCCESS) {
                console_log_error_tagged("vulkan", "{} VkResult returned: {}", p_name, (int)p_result);
            }
        }
    };
};