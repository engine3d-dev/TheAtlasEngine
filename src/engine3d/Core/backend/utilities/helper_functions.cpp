#include <Core/backend/utilities/helper_functions.h>
#include <GLFW/glfw3.h>
#include <Core/EngineLogger.h>

namespace engine3d{
    //! @param currentWindowCtx
    //! @note Our current window context for passing in our window.
    //! @param isWindowOpen
    //! @note We can't gurantee that the window will be open, and should check for this to prevent errors in centering an unopened window.
    void CenterCurrentWindow(GLFWwindow* currentWindowCtx, int currentWindowWidth, int currentWindowHeight, bool isWindowOpened){

        //! @note If window unopened, we dont center our window at all.
        if(!isWindowOpened){
            return;
        }

        //! @note Getting our current monitor
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        //! @note Getting our current window data
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        //! @note Based on our offset for window dimensions setting the application by default to the center of the screen.
        //! @note TODO --- Should cache our dimensions and window locs so when users open up engine3d again, it will appear back in it's previous last location when it was opened.
        glfwSetWindowPos(currentWindowCtx, (mode->width - currentWindowWidth) / 2, (mode->height / currentWindowHeight) / 2);
    }

    namespace vk{
        std::string VkResultToString(VkResult res){
            switch (res){
                case VkResult::VK_SUCCESS: return "VkResult::VK_SUCCESS";
                case VkResult::VK_NOT_READY: return "VkResult::VK_NOT_READY";
                case VkResult::VK_TIMEOUT: return "VkResult::VK_TIMEOUT";
                case VkResult::VK_EVENT_SET: return "VkResult::VK_EVENT_SET";
                case VkResult::VK_EVENT_RESET: return "VkResult::VK_EVENT_RESET";
                case VkResult::VK_INCOMPLETE: return "VkResult::VK_INCOMPLETE";
                case VkResult::VK_ERROR_OUT_OF_HOST_MEMORY: return "VkResult::VK_ERROR_OUT_OF_HOST_MEMORY";
                case VkResult::VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VkResult::VK_ERROR_OUT_OF_DEVICE_MEMORY";
                case VkResult::VK_ERROR_INITIALIZATION_FAILED: return "VkResult::VK_ERROR_INITIALIZATION_FAILED";
                case VkResult::VK_ERROR_DEVICE_LOST: return "VkResult::VK_ERROR_DEVICE_LOST";
                case VkResult::VK_ERROR_MEMORY_MAP_FAILED: return "VkResult::VK_ERROR_MEMORY_MAP_FAILED";
                case VkResult::VK_ERROR_LAYER_NOT_PRESENT: return "VkResult::VK_ERROR_LAYER_NOT_PRESENT";
                case VkResult::VK_ERROR_EXTENSION_NOT_PRESENT: return "VkResult::VK_ERROR_EXTENSION_NOT_PRESENT";
                case VkResult::VK_ERROR_FEATURE_NOT_PRESENT: return "VkResult::VK_ERROR_FEATURE_NOT_PRESENT";
                case VkResult::VK_ERROR_INCOMPATIBLE_DRIVER: return "VkResult::VK_ERROR_INCOMPATIBLE_DRIVER";
                case VkResult::VK_ERROR_TOO_MANY_OBJECTS: return "VkResult::VK_ERROR_TOO_MANY_OBJECTS";
                case VkResult::VK_ERROR_FORMAT_NOT_SUPPORTED: return "VkResult::VK_ERROR_FORMAT_NOT_SUPPORTED";
                case VkResult::VK_ERROR_FRAGMENTED_POOL: return "VkResult::VK_ERROR_FRAGMENTED_POOL";
                case VkResult::VK_ERROR_UNKNOWN: return "VkResult::VK_ERROR_UNKNOWN";
                case VkResult::VK_ERROR_OUT_OF_POOL_MEMORY: return "VkResult::VK_ERROR_OUT_OF_POOL_MEMORY";
                case VkResult::VK_ERROR_INVALID_EXTERNAL_HANDLE: return "VkResult::VK_ERROR_INVALID_EXTERNAL_HANDLE";
                case VkResult::VK_ERROR_FRAGMENTATION: return "VkResult::VK_ERROR_FRAGMENTATION";
                case VkResult::VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "VkResult::VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
                case VkResult::VK_PIPELINE_COMPILE_REQUIRED: return "VkResult::VK_PIPELINE_COMPILE_REQUIRED";
                case VkResult::VK_ERROR_SURFACE_LOST_KHR: return "VkResult::VK_ERROR_SURFACE_LOST_KHR";
                case VkResult::VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VkResult::VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
                case VkResult::VK_SUBOPTIMAL_KHR: return "VkResult::VK_SUBOPTIMAL_KHR";
                case VkResult::VK_ERROR_OUT_OF_DATE_KHR: return "VkResult::VK_ERROR_OUT_OF_DATE_KHR";
                case VkResult::VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "VkResult::VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
                case VkResult::VK_ERROR_VALIDATION_FAILED_EXT: return "VkResult::VK_ERROR_VALIDATION_FAILED_EXT";
                case VkResult::VK_ERROR_INVALID_SHADER_NV: return "VkResult::VK_ERROR_INVALID_SHADER_NV";
                case VkResult::VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR: return "VkResult::VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
                case VkResult::VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR: return "VkResult::VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
                case VkResult::VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR: return "VkResult::VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
                case VkResult::VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR: return "VkResult::VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
                case VkResult::VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR: return "VkResult::VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
                case VkResult::VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR: return "VkResult::VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
                case VkResult::VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return "VkResult::VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
                case VkResult::VK_ERROR_NOT_PERMITTED_KHR: return "VkResult::VK_ERROR_NOT_PERMITTED_KHR";
                case VkResult::VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return "VkResult::VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
                case VkResult::VK_THREAD_IDLE_KHR: return "VkResult::VK_THREAD_IDLE_KHR";
                case VkResult::VK_THREAD_DONE_KHR: return "VkResult::VK_THREAD_DONE_KHR";
                case VkResult::VK_OPERATION_DEFERRED_KHR: return "VkResult::VK_OPERATION_DEFERRED_KHR";
                case VkResult::VK_OPERATION_NOT_DEFERRED_KHR: return "VkResult::VK_OPERATION_NOT_DEFERRED_KHR";
            #ifdef VK_ENABLE_BETA_EXTENSIONS
                case VkResult::VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR: return "VkResult::";
            #endif
                case VkResult::VK_ERROR_COMPRESSION_EXHAUSTED_EXT: return "VkResult::VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
                case VkResult::VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT: return "VkResult::VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT";
                // case VkResult::VK_ERROR_OUT_OF_POOL_MEMORY_KHR: return "VkResult::";
                // case VkResult::VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR: return "VkResult::";
                // case VkResult::VK_ERROR_FRAGMENTATION_EXT: return "VkResult::";
                // case VkResult::VK_ERROR_NOT_PERMITTED_EXT: return "VkResult::";
                // case VkResult::VK_ERROR_INVALID_DEVICE_ADDRESS_EXT: return "VkResult::";
                // case VkResult::VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR: return "VkResult::";
                // case VkResult::VK_PIPELINE_COMPILE_REQUIRED_EXT: return "VkResult::";
                // case VkResult::VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT: return "VkResult::";
                // case VkResult::VK_RESULT_MAX_ENUM: return "VkResult::";
            default:
                return "Returning default value for VkResult:: string!";
            }
        }
        
        std::string GetDebugSeverity(VkDebugUtilsMessageSeverityFlagBitsEXT Severity){
            switch (Severity){
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: return "Verbose";
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: return "Info";
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: return "Warning";
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: return "Error";
            default:
                CoreLogError("Invalid severity Code");
                return "";
            }
        }
        
        std::string GetDebugType(VkDebugUtilsMessageTypeFlagsEXT Type){
            switch(Type){
            case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: return "General";
            case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: return "Validation";
            case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: return "Performance";
            case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT: return "Device address binding";
            default:
                CoreLogError("Invalid type code!");
                return "";
            }
        }

        std::string VkImageUsageFlagsToString(const VkImageUsageFlags& flags){
            if(flags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) return "Image usage transfer src supported";
            if(flags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) return "Image usage transfer dest is supported";
            if(flags & VK_IMAGE_USAGE_SAMPLED_BIT) return "Image usage color attachment is supported";
            if(flags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) return "Image usage depth stencil attachment is supported";
            if(flags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) return "Image usage transient attachment is supported";
            if(flags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) return "Image usage input attachment is supported";

            return "flags was either compared with a flag that was not added in VkImageUsageFlagsToString function or it didnt because that flag was not supported!";
        }
        
        std::string VkPresentModeToString(const VkPresentModeKHR& modes){
            if(modes & VK_PRESENT_MODE_IMMEDIATE_KHR) return "Present mode immediate is supported";
            if(modes & VK_PRESENT_MODE_MAILBOX_KHR) return "Present mode mailbox is supported";
            if(modes & VK_PRESENT_MODE_FIFO_KHR) return "Present mode fifo is supported";
            if(modes & VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR) return "Present mode shared demand refresh is supported";
            if(modes & VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR) return "Present mode shared continuous refresh is supported";
            if(modes & VK_PRESENT_MODE_MAX_ENUM_KHR) return "Present mode max enum is supported";

            return "modes reached to default value meaning that it did not match any of the modes that is supported!";
        }
    };
};