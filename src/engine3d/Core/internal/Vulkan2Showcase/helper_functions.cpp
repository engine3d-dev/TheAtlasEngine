#include <Core/EngineLogger.hpp>
#include <Core/internal/Vulkan2Showcase/helper_functions.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    void vk_check(VkResult result, const char* p_Tag, const char* p_Filename, uint32_t p_LineName, const char* p_FunctionName){
        if(result != VK_SUCCESS){
            ConsoleLogError("VkResult failed taking in {0} file: {1} --- Line: {2} --- In Function: {3}",p_Tag, p_Filename, p_LineName, p_FunctionName);
            ConsoleLogError("VkResult returned: {}", (int)result);
            // std::terminate();
            // exit(0);
        }
    }

    void vk_check_format(VkFormat p_Format, const char* p_Filename, uint32_t p_LineName, const char* p_FunctionName){
        if(p_Format == VK_FORMAT_UNDEFINED){
            ConsoleLogError("VkFormat failed in file: {0} --- Line: {1} --- In Function: {2}", p_Filename, p_LineName, p_FunctionName);
            ConsoleLogError("VkFormat was assigned to VK_FORMAT_UNDEFINED");
        }
    }

    std::string vk_queue_flags_to_string(VkQueueFlagBits flags){
        switch (flags){
        case VK_QUEUE_GRAPHICS_BIT: return "VK_QUEUE_GRAPHICS_BIT";
        case VK_QUEUE_COMPUTE_BIT: return "VK_QUEUE_COMPUTE_BIT";
        case VK_QUEUE_TRANSFER_BIT: return "VK_QUEUE_TRANSFER_BIT";
        case VK_QUEUE_SPARSE_BINDING_BIT: return "VK_QUEUE_SPARSE_BINDING_BIT";
        case VK_QUEUE_PROTECTED_BIT: return "VK_QUEUE_PROTECTED_BIT";
        case VK_QUEUE_VIDEO_DECODE_BIT_KHR: return "VK_QUEUE_VIDEO_DECODE_BIT_KHR";
        case VK_QUEUE_VIDEO_ENCODE_BIT_KHR: return "VK_QUEUE_VIDEO_ENCODE_BIT_KHR";
        case VK_QUEUE_OPTICAL_FLOW_BIT_NV: return "VK_QUEUE_OPTICAL_FLOW_BIT_NV";
        case VK_QUEUE_FLAG_BITS_MAX_ENUM: return "VK_QUEUE_FLAG_BITS_MAX_ENUM";
        }

        return "VkQueueFlag not Selected";
    }
};