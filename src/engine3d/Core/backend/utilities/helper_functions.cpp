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
        //! @note We go through our selected surface formats
        //! @note Checking for both the srgb formats and srgb non linear color space.
        //! @note If these aren't found in our selected surface formats and colorspaces then we use the default in our surface formats.
        VkSurfaceFormatKHR SelectSurfaceFormatAndColorspace(const std::vector<VkSurfaceFormatKHR>& surfaceFormats){
            for(int i = 0; i < surfaceFormats.size(); i++){
                if((surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB) and (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)){
                    return surfaceFormats[i];
                }
            }

            return surfaceFormats[0];
        }

        //! @note TODO -- Probably do a specialized template for this so we dont need to make a function call, instead when we receive VkResult type we print the corresponding std::string.
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
                ConsoleLogError("Invalid severity Code");
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
                ConsoleLogError("Invalid type code!");
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

        std::string VkFormatToString(const VkFormat& format){
            switch (format){
            case VK_FORMAT_UNDEFINED: return "VK_FORMAT_UNDEFINED";
            case VK_FORMAT_R4G4_UNORM_PACK8: return "VK_FORMAT_R4G4_UNORM_PACK8";
            case VK_FORMAT_R4G4B4A4_UNORM_PACK16: return "VK_FORMAT_R4G4B4A4_UNORM_PACK16";
            case VK_FORMAT_B4G4R4A4_UNORM_PACK16: return "VK_FORMAT_B4G4R4A4_UNORM_PACK16";
            case VK_FORMAT_R5G6B5_UNORM_PACK16: return "VK_FORMAT_R5G6B5_UNORM_PACK16";
            case VK_FORMAT_B5G6R5_UNORM_PACK16: return "VK_FORMAT_B5G6R5_UNORM_PACK16";
            case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return "VK_FORMAT_R5G5B5A1_UNORM_PACK16";
            case VK_FORMAT_B5G5R5A1_UNORM_PACK16: return "VK_FORMAT_B5G5R5A1_UNORM_PACK16";
            case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return "VK_FORMAT_A1R5G5B5_UNORM_PACK16";
            case VK_FORMAT_R8_UNORM: return "VK_FORMAT_R8_UNORM";
            case VK_FORMAT_R8_SNORM: return "VK_FORMAT_R8_SNORM";
            case VK_FORMAT_R8_USCALED: return "VK_FORMAT_R8_USCALED";
            case VK_FORMAT_R8_SSCALED: return "VK_FORMAT_R8_SSCALED";
            case VK_FORMAT_R8_UINT: return "VK_FORMAT_R8_UINT";
            case VK_FORMAT_R8_SINT: return "VK_FORMAT_R8_SINT";
            case VK_FORMAT_R8_SRGB: return "VK_FORMAT_R8_SRGB";
            case VK_FORMAT_R8G8_UNORM: return "VK_FORMAT_R8G8_UNORM";
            case VK_FORMAT_R8G8_SNORM: return "VK_FORMAT_R8G8_SNORM";
            case VK_FORMAT_R8G8_USCALED: return "VK_FORMAT_R8G8_USCALED";
            case VK_FORMAT_R8G8_SSCALED: return "VK_FORMAT_R8G8_SSCALED";
            case VK_FORMAT_R8G8_UINT: return "VK_FORMAT_R8G8_UINT";
            case VK_FORMAT_R8G8_SINT: return "VK_FORMAT_R8G8_SINT";
            case VK_FORMAT_R8G8_SRGB: return "VK_FORMAT_R8G8_SRGB";
            case VK_FORMAT_R8G8B8_UNORM: return "VK_FORMAT_R8G8B8_UNORM";
            case VK_FORMAT_R8G8B8_SNORM: return "VK_FORMAT_R8G8B8_SNORM";
            case VK_FORMAT_R8G8B8_USCALED: return "VK_FORMAT_R8G8B8_USCALED";
            case VK_FORMAT_R8G8B8_SSCALED: return "VK_FORMAT_R8G8B8_SSCALED";
            case VK_FORMAT_R8G8B8_UINT: return "VK_FORMAT_R8G8B8_UINT";
            case VK_FORMAT_R8G8B8_SINT: return "VK_FORMAT_R8G8B8_SINT";
            case VK_FORMAT_R8G8B8_SRGB: return "VK_FORMAT_R8G8B8_SRGB";
            case VK_FORMAT_B8G8R8_SNORM: return "VK_FORMAT_B8G8R8_SNORM";
            case VK_FORMAT_B8G8R8_USCALED: return "VK_FORMAT_B8G8R8_USCALED";
            case VK_FORMAT_B8G8R8_SSCALED: return "VK_FORMAT_B8G8R8_SSCALED";
            case VK_FORMAT_B8G8R8_UINT: return "VK_FORMAT_B8G8R8_UINT";
            case VK_FORMAT_B8G8R8_SINT: return "VK_FORMAT_B8G8R8_SINT";
            case VK_FORMAT_B8G8R8_SRGB: return "VK_FORMAT_B8G8R8_SRGB";
            case VK_FORMAT_R8G8B8A8_UNORM: return "VK_FORMAT_R8G8B8A8_UNORM";
            case VK_FORMAT_R8G8B8A8_SNORM: return "VK_FORMAT_R8G8B8A8_SNORM";
            case VK_FORMAT_R8G8B8A8_USCALED: return "VK_FORMAT_R8G8B8A8_USCALED";
            case VK_FORMAT_R8G8B8A8_SSCALED: return "VK_FORMAT_R8G8B8A8_SSCALED";
            case VK_FORMAT_R8G8B8A8_UINT: return "VK_FORMAT_R8G8B8A8_UINT";
            case VK_FORMAT_R8G8B8A8_SINT: return "VK_FORMAT_R8G8B8A8_SINT";
            case VK_FORMAT_R8G8B8A8_SRGB: return "VK_FORMAT_R8G8B8A8_SRGB";
            case VK_FORMAT_B8G8R8A8_UNORM: return "VK_FORMAT_B8G8R8A8_UNORM";
            case VK_FORMAT_B8G8R8A8_SNORM: return "VK_FORMAT_B8G8R8A8_SNORM";
            case VK_FORMAT_B8G8R8A8_USCALED: return "VK_FORMAT_B8G8R8A8_USCALED";
            case VK_FORMAT_B8G8R8A8_SSCALED: return "VK_FORMAT_B8G8R8A8_SSCALED";
            case VK_FORMAT_B8G8R8A8_UINT: return "VK_FORMAT_B8G8R8A8_UINT";
            case VK_FORMAT_B8G8R8A8_SINT: return "VK_FORMAT_B8G8R8A8_SINT";
            case VK_FORMAT_B8G8R8A8_SRGB: return "VK_FORMAT_B8G8R8A8_SRGB";
            case VK_FORMAT_A8B8G8R8_UNORM_PACK32: return "VK_FORMAT_A8B8G8R8_UNORM_PACK32";
            case VK_FORMAT_A8B8G8R8_SNORM_PACK32: return "VK_FORMAT_A8B8G8R8_SNORM_PACK32";
            case VK_FORMAT_A8B8G8R8_USCALED_PACK32: return "VK_FORMAT_A8B8G8R8_USCALED_PACK32";
            case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: return "VK_FORMAT_A8B8G8R8_SSCALED_PACK32";
            case VK_FORMAT_A8B8G8R8_UINT_PACK32: return "VK_FORMAT_A8B8G8R8_UINT_PACK32";
            case VK_FORMAT_A8B8G8R8_SINT_PACK32: return "VK_FORMAT_A8B8G8R8_SINT_PACK32";
            case VK_FORMAT_A8B8G8R8_SRGB_PACK32: return "VK_FORMAT_A8B8G8R8_SRGB_PACK32";
            case VK_FORMAT_A2R10G10B10_UNORM_PACK32: return "VK_FORMAT_A2R10G10B10_UNORM_PACK32";
            case VK_FORMAT_A2R10G10B10_SNORM_PACK32: return "VK_FORMAT_A2R10G10B10_SNORM_PACK32";
            case VK_FORMAT_A2R10G10B10_USCALED_PACK32: return "VK_FORMAT_A2R10G10B10_USCALED_PACK32";
            case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: return "VK_FORMAT_A2R10G10B10_SSCALED_PACK32";
            case VK_FORMAT_A2R10G10B10_UINT_PACK32: return "VK_FORMAT_A2R10G10B10_UINT_PACK32";
            case VK_FORMAT_A2R10G10B10_SINT_PACK32: return "VK_FORMAT_A2R10G10B10_SINT_PACK32";
            case VK_FORMAT_A2B10G10R10_UNORM_PACK32: return "VK_FORMAT_A2B10G10R10_UNORM_PACK32";
            case VK_FORMAT_A2B10G10R10_SNORM_PACK32: return "VK_FORMAT_A2B10G10R10_SNORM_PACK32";
            case VK_FORMAT_A2B10G10R10_USCALED_PACK32: return "VK_FORMAT_A2B10G10R10_USCALED_PACK32";
            case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: return "VK_FORMAT_A2B10G10R10_SSCALED_PACK32";
            case VK_FORMAT_A2B10G10R10_UINT_PACK32: return "VK_FORMAT_A2B10G10R10_UINT_PACK32";
            case VK_FORMAT_A2B10G10R10_SINT_PACK32: return "VK_FORMAT_A2B10G10R10_SINT_PACK32";
            case VK_FORMAT_R16_UNORM: return "VK_FORMAT_R16_UNORM";
            case VK_FORMAT_R16_SNORM: return "VK_FORMAT_R16_SNORM";
            case VK_FORMAT_R16_USCALED: return "VK_FORMAT_R16_USCALED";
            case VK_FORMAT_R16_SSCALED: return "VK_FORMAT_R16_SSCALED";
            case VK_FORMAT_R16_UINT: return "VK_FORMAT_R16_UINT";
            case VK_FORMAT_R16_SINT: return "VK_FORMAT_R16_SINT";
            case VK_FORMAT_R16_SFLOAT: return "VK_FORMAT_R16_SFLOAT";
            case VK_FORMAT_R16G16_UNORM: return "VK_FORMAT_R16G16_UNORM";
            case VK_FORMAT_R16G16_SNORM: return "VK_FORMAT_R16G16_SNORM";
            case VK_FORMAT_R16G16_USCALED: return "VK_FORMAT_R16G16_USCALED";
            case VK_FORMAT_R16G16_SSCALED: return "VK_FORMAT_R16G16_SSCALED";
            case VK_FORMAT_R16G16_UINT: return "VK_FORMAT_R16G16_UINT";
            case VK_FORMAT_R16G16_SINT: return "VK_FORMAT_R16G16_SINT";
            case VK_FORMAT_R16G16_SFLOAT: return "VK_FORMAT_R16G16_SFLOAT";
            case VK_FORMAT_R16G16B16_UNORM: return "VK_FORMAT_R16G16B16_UNORM";
            case VK_FORMAT_R16G16B16_SNORM: return "VK_FORMAT_R16G16B16_SNORM";
            case VK_FORMAT_R16G16B16_USCALED: return "VK_FORMAT_R16G16B16_USCALED";
            case VK_FORMAT_R16G16B16_SSCALED: return "VK_FORMAT_R16G16B16_SSCALED";
            case VK_FORMAT_R16G16B16_UINT: return "VK_FORMAT_R16G16B16_UINT";
            case VK_FORMAT_R16G16B16_SINT: return "VK_FORMAT_R16G16B16_SINT";
            case VK_FORMAT_R16G16B16_SFLOAT: return "VK_FORMAT_R16G16B16_SFLOAT";
            case VK_FORMAT_R16G16B16A16_UNORM: return "VK_FORMAT_R16G16B16A16_UNORM";
            case VK_FORMAT_R16G16B16A16_SNORM: return "VK_FORMAT_R16G16B16A16_SNORM";
            case VK_FORMAT_R16G16B16A16_USCALED: return "VK_FORMAT_R16G16B16A16_USCALED";
            case VK_FORMAT_R16G16B16A16_SSCALED: return "VK_FORMAT_R16G16B16A16_SSCALED";
            case VK_FORMAT_R16G16B16A16_UINT: return "VK_FORMAT_R16G16B16A16_UINT";
            case VK_FORMAT_R16G16B16A16_SINT: return "VK_FORMAT_R16G16B16A16_SINT";
            case VK_FORMAT_R16G16B16A16_SFLOAT: return "VK_FORMAT_R16G16B16A16_SFLOAT";
            case VK_FORMAT_R32_UINT: return "VK_FORMAT_R32_UINT";
            case VK_FORMAT_R32_SINT: return "VK_FORMAT_R32_SINT";
            case VK_FORMAT_R32_SFLOAT: return "VK_FORMAT_R32_SFLOAT";
            case VK_FORMAT_R32G32_UINT: return "VK_FORMAT_R32G32_UINT";
            case VK_FORMAT_R32G32_SINT: return "VK_FORMAT_R32G32_SINT";
            case VK_FORMAT_R32G32_SFLOAT: return "VK_FORMAT_R32G32_SFLOAT";
            case VK_FORMAT_R32G32B32_UINT: return "VK_FORMAT_R32G32B32_UINT";
            case VK_FORMAT_R32G32B32_SINT: return "VK_FORMAT_R32G32B32_SINT";
            case VK_FORMAT_R32G32B32_SFLOAT: return "VK_FORMAT_R32G32B32_SFLOAT";
            case VK_FORMAT_R32G32B32A32_UINT: return "VK_FORMAT_R32G32B32A32_UINT";
            case VK_FORMAT_R32G32B32A32_SINT: return "VK_FORMAT_R32G32B32A32_SINT";
            case VK_FORMAT_R32G32B32A32_SFLOAT: return "VK_FORMAT_R32G32B32A32_SFLOAT";
            case VK_FORMAT_R64_UINT: return "VK_FORMAT_R64_UINT";
            case VK_FORMAT_R64_SINT: return "VK_FORMAT_R64_SINT";
            case VK_FORMAT_R64_SFLOAT: return "VK_FORMAT_R64_SFLOAT";
            case VK_FORMAT_R64G64_UINT: return "VK_FORMAT_R64G64_UINT";
            case VK_FORMAT_R64G64_SINT: return "VK_FORMAT_R64G64_SINT";
            case VK_FORMAT_R64G64_SFLOAT: return "VK_FORMAT_R64G64_SFLOAT";
            case VK_FORMAT_R64G64B64_UINT: return "VK_FORMAT_R64G64B64_UINT";
            case VK_FORMAT_R64G64B64_SINT: return "VK_FORMAT_R64G64B64_SINT";
            case VK_FORMAT_R64G64B64_SFLOAT: return "VK_FORMAT_R64G64B64_SFLOAT";
            case VK_FORMAT_R64G64B64A64_UINT: return "VK_FORMAT_R64G64B64A64_UINT";
            case VK_FORMAT_R64G64B64A64_SINT: return "VK_FORMAT_R64G64B64A64_SINT";
            case VK_FORMAT_R64G64B64A64_SFLOAT: return "VK_FORMAT_R64G64B64A64_SFLOAT";
            case VK_FORMAT_B10G11R11_UFLOAT_PACK32: return "VK_FORMAT_B10G11R11_UFLOAT_PACK32";
            case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: return "VK_FORMAT_E5B9G9R9_UFLOAT_PACK32";
            case VK_FORMAT_D16_UNORM: return "VK_FORMAT_D16_UNORM";
            case VK_FORMAT_X8_D24_UNORM_PACK32: return "VK_FORMAT_X8_D24_UNORM_PACK32";
            case VK_FORMAT_D32_SFLOAT: return "VK_FORMAT_D32_SFLOAT";
            case VK_FORMAT_S8_UINT: return "VK_FORMAT_S8_UINT";
            case VK_FORMAT_D16_UNORM_S8_UINT: return "VK_FORMAT_D16_UNORM_S8_UINT";
            case VK_FORMAT_D24_UNORM_S8_UINT: return "VK_FORMAT_D24_UNORM_S8_UINT";
            case VK_FORMAT_D32_SFLOAT_S8_UINT: return "VK_FORMAT_D32_SFLOAT_S8_UINT";
            case VK_FORMAT_BC1_RGB_UNORM_BLOCK: return "VK_FORMAT_BC1_RGB_UNORM_BLOCK";
            case VK_FORMAT_BC1_RGB_SRGB_BLOCK: return "VK_FORMAT_BC1_RGB_SRGB_BLOCK";
            case VK_FORMAT_BC1_RGBA_UNORM_BLOCK: return "VK_FORMAT_BC1_RGBA_UNORM_BLOCK";
            case VK_FORMAT_BC1_RGBA_SRGB_BLOCK: return "VK_FORMAT_BC1_RGBA_SRGB_BLOCK";
            case VK_FORMAT_BC2_UNORM_BLOCK: return "VK_FORMAT_BC2_UNORM_BLOCK";
            case VK_FORMAT_BC2_SRGB_BLOCK: return "VK_FORMAT_BC2_SRGB_BLOCK";
            case VK_FORMAT_BC3_UNORM_BLOCK: return "VK_FORMAT_BC3_UNORM_BLOCK";
            case VK_FORMAT_BC3_SRGB_BLOCK: return "VK_FORMAT_BC3_SRGB_BLOCK";
            case VK_FORMAT_BC4_UNORM_BLOCK: return "VK_FORMAT_BC4_UNORM_BLOCK";
            case VK_FORMAT_BC4_SNORM_BLOCK: return "VK_FORMAT_BC4_SNORM_BLOCK";
            case VK_FORMAT_BC5_UNORM_BLOCK: return "VK_FORMAT_BC5_UNORM_BLOCK";
            case VK_FORMAT_BC5_SNORM_BLOCK: return "VK_FORMAT_BC5_SNORM_BLOCK";
            case VK_FORMAT_BC6H_UFLOAT_BLOCK: return "VK_FORMAT_BC6H_UFLOAT_BLOCK";
            case VK_FORMAT_BC6H_SFLOAT_BLOCK: return "VK_FORMAT_BC6H_SFLOAT_BLOCK";
            case VK_FORMAT_BC7_UNORM_BLOCK: return "VK_FORMAT_BC7_UNORM_BLOCK";
            case VK_FORMAT_BC7_SRGB_BLOCK: return "VK_FORMAT_BC7_SRGB_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK: return "VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK: return "VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK: return "VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK: return "VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK: return "VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK: return "VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK";
            case VK_FORMAT_EAC_R11_UNORM_BLOCK: return "VK_FORMAT_EAC_R11_UNORM_BLOCK";
            case VK_FORMAT_EAC_R11_SNORM_BLOCK: return "VK_FORMAT_EAC_R11_SNORM_BLOCK";
            case VK_FORMAT_EAC_R11G11_UNORM_BLOCK: return "VK_FORMAT_EAC_R11G11_UNORM_BLOCK";
            case VK_FORMAT_EAC_R11G11_SNORM_BLOCK: return "VK_FORMAT_EAC_R11G11_SNORM_BLOCK";
            case VK_FORMAT_ASTC_4x4_UNORM_BLOCK: return "VK_FORMAT_ASTC_4x4_UNORM_BLOCK";
            case VK_FORMAT_ASTC_4x4_SRGB_BLOCK: return "VK_FORMAT_ASTC_4x4_SRGB_BLOCK";
            case VK_FORMAT_ASTC_5x4_UNORM_BLOCK: return "VK_FORMAT_ASTC_5x4_UNORM_BLOCK";
            case VK_FORMAT_ASTC_5x4_SRGB_BLOCK: return "VK_FORMAT_ASTC_5x4_SRGB_BLOCK";
            case VK_FORMAT_ASTC_5x5_UNORM_BLOCK: return "VK_FORMAT_ASTC_5x5_UNORM_BLOCK";
            case VK_FORMAT_ASTC_5x5_SRGB_BLOCK: return "VK_FORMAT_ASTC_5x5_SRGB_BLOCK";
            case VK_FORMAT_ASTC_6x5_UNORM_BLOCK: return "VK_FORMAT_ASTC_6x5_UNORM_BLOCK";
            case VK_FORMAT_ASTC_6x5_SRGB_BLOCK: return "VK_FORMAT_ASTC_6x5_SRGB_BLOCK";
            case VK_FORMAT_ASTC_6x6_UNORM_BLOCK: return "VK_FORMAT_ASTC_6x6_UNORM_BLOCK";
            case VK_FORMAT_ASTC_6x6_SRGB_BLOCK: return "VK_FORMAT_ASTC_6x6_SRGB_BLOCK";
            case VK_FORMAT_ASTC_8x5_UNORM_BLOCK: return "VK_FORMAT_ASTC_8x5_UNORM_BLOCK";
            case VK_FORMAT_ASTC_8x5_SRGB_BLOCK: return "VK_FORMAT_ASTC_8x5_SRGB_BLOCK";
            case VK_FORMAT_ASTC_8x6_UNORM_BLOCK: return "VK_FORMAT_ASTC_8x6_UNORM_BLOCK";
            case VK_FORMAT_ASTC_8x6_SRGB_BLOCK: return "VK_FORMAT_ASTC_8x6_SRGB_BLOCK";
            case VK_FORMAT_ASTC_8x8_UNORM_BLOCK: return "VK_FORMAT_ASTC_8x8_UNORM_BLOCK";
            case VK_FORMAT_ASTC_8x8_SRGB_BLOCK: return "VK_FORMAT_ASTC_8x8_SRGB_BLOCK";
            case VK_FORMAT_ASTC_10x5_UNORM_BLOCK: return "VK_FORMAT_ASTC_10x5_UNORM_BLOCK";
            case VK_FORMAT_ASTC_10x5_SRGB_BLOCK: return "VK_FORMAT_ASTC_10x5_SRGB_BLOCK";
            case VK_FORMAT_ASTC_10x6_UNORM_BLOCK: return "VK_FORMAT_ASTC_10x6_UNORM_BLOCK";
            case VK_FORMAT_ASTC_10x6_SRGB_BLOCK: return "VK_FORMAT_ASTC_10x6_SRGB_BLOCK";
            case VK_FORMAT_ASTC_10x8_UNORM_BLOCK: return "VK_FORMAT_ASTC_10x8_UNORM_BLOCK";
            case VK_FORMAT_ASTC_10x8_SRGB_BLOCK: return "VK_FORMAT_ASTC_10x8_SRGB_BLOCK";
            case VK_FORMAT_ASTC_10x10_UNORM_BLOCK: return "VK_FORMAT_ASTC_10x10_UNORM_BLOCK";
            case VK_FORMAT_ASTC_10x10_SRGB_BLOCK: return "VK_FORMAT_ASTC_10x10_SRGB_BLOCK";
            case VK_FORMAT_ASTC_12x10_UNORM_BLOCK: return "VK_FORMAT_ASTC_12x10_UNORM_BLOCK";
            case VK_FORMAT_ASTC_12x10_SRGB_BLOCK: return "VK_FORMAT_ASTC_12x10_SRGB_BLOCK";
            case VK_FORMAT_ASTC_12x12_UNORM_BLOCK: return "VK_FORMAT_ASTC_12x12_UNORM_BLOCK";
            case VK_FORMAT_ASTC_12x12_SRGB_BLOCK: return "VK_FORMAT_ASTC_12x12_SRGB_BLOCK";
            case VK_FORMAT_G8B8G8R8_422_UNORM: return "VK_FORMAT_G8B8G8R8_422_UNORM";
            case VK_FORMAT_B8G8R8G8_422_UNORM: return "VK_FORMAT_B8G8R8G8_422_UNORM";
            case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM: return "VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM";
            case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM: return "VK_FORMAT_G8_B8R8_2PLANE_420_UNORM";
            case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM: return "VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM";
            case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM: return "VK_FORMAT_G8_B8R8_2PLANE_422_UNORM";
            case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM: return "VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM";
            case VK_FORMAT_R10X6_UNORM_PACK16: return "VK_FORMAT_R10X6_UNORM_PACK16";
            case VK_FORMAT_R10X6G10X6_UNORM_2PACK16: return "VK_FORMAT_R10X6G10X6_UNORM_2PACK16";
            case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16: return "VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16";
            case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16: return "VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16";
            case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16: return "VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16";
            case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16: return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16";
            case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16: return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16";
            case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16: return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16";
            case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16: return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16";
            case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16: return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16";
            case VK_FORMAT_R12X4_UNORM_PACK16: return "VK_FORMAT_R12X4_UNORM_PACK16";
            case VK_FORMAT_R12X4G12X4_UNORM_2PACK16: return "VK_FORMAT_R12X4G12X4_UNORM_2PACK16";
            case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16: return "VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16";
            case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16: return "VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16";
            case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16: return "VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16";
            case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16: return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16";
            case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16: return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16";
            case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16: return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16";
            case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16: return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16";
            case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16: return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16";
            case VK_FORMAT_G16B16G16R16_422_UNORM: return "VK_FORMAT_G16B16G16R16_422_UNORM";
            case VK_FORMAT_B16G16R16G16_422_UNORM: return "VK_FORMAT_B16G16R16G16_422_UNORM";
            case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM: return "VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM";
            case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM: return "VK_FORMAT_G16_B16R16_2PLANE_420_UNORM";
            case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM: return "VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM";
            case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM: return "VK_FORMAT_G16_B16R16_2PLANE_422_UNORM";
            case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM: return "VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM";
            case VK_FORMAT_G8_B8R8_2PLANE_444_UNORM: return "VK_FORMAT_G8_B8R8_2PLANE_444_UNORM";
            case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16: return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16";
            case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16: return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16";
            case VK_FORMAT_G16_B16R16_2PLANE_444_UNORM: return "VK_FORMAT_G16_B16R16_2PLANE_444_UNORM";
            case VK_FORMAT_A4R4G4B4_UNORM_PACK16: return "VK_FORMAT_A4R4G4B4_UNORM_PACK16";
            case VK_FORMAT_A4B4G4R4_UNORM_PACK16: return "VK_FORMAT_A4B4G4R4_UNORM_PACK16";
            case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK: return "VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK: return "VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK: return "VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK: return "VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK: return "VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK: return "VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK: return "VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK: return "VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK: return "VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK: return "VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK: return "VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK: return "VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK: return "VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK: return "VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK";
            case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG: return "VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG";
            case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG: return "VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG";
            case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG: return "VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG";
            case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG: return "VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG";
            case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG: return "VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG";
            case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG: return "VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG";
            case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG: return "VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG";
            case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG: return "VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG";
            case VK_FORMAT_R16G16_S10_5_NV: return "VK_FORMAT_R16G16_S10_5_NV";
            case VK_FORMAT_A1B5G5R5_UNORM_PACK16_KHR: return "VK_FORMAT_A1B5G5R5_UNORM_PACK16_KHR";
            case VK_FORMAT_A8_UNORM_KHR: return "VK_FORMAT_A8_UNORM_KHR";
            default: return "VkFormatString return default edge case which is VkFormat = VK_FORMAT_UNDEFINED";
            // case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT: return "";
            // case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT: return "";
            // case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT: return "";
            // case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT: return "";
            // case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT: return "";
            // case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT: return "";
            // case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT: return "";
            // case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT: return "";
            // case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT: return "";
            // case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT: return "";
            // case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT: return "";
            // case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT: return "";
            // case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT: return "";
            // case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT: return "";
            // case VK_FORMAT_G8B8G8R8_422_UNORM_KHR: return "";
            // case VK_FORMAT_B8G8R8G8_422_UNORM_KHR: return "";
            // case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR: return "";
            // case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR: return "";
            // case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR: return "";
            // case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR: return "";
            // case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR: return "";
            // case VK_FORMAT_R10X6_UNORM_PACK16_KHR: return "";
            // case VK_FORMAT_R10X6G10X6_UNORM_2PACK16_KHR: return "";
            // case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR: return "";
            // case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR: return "";
            // case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR: return "";
            // case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR: return "";
            // case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR: return "";
            // case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR: return "";
            // case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR: return "";
            // case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR: return "";
            // case VK_FORMAT_R12X4_UNORM_PACK16_KHR: return "";
            // case VK_FORMAT_R12X4G12X4_UNORM_2PACK16_KHR: return "";
            // case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR: return "";
            // case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR: return "";
            // case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR: return "";
            // case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR: return "";
            // case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR: return "";
            // case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR: return "";
            // case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR: return "";
            // case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR: return "";
            // case VK_FORMAT_G16B16G16R16_422_UNORM_KHR: return "";
            // case VK_FORMAT_B16G16R16G16_422_UNORM_KHR: return "";
            // case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR: return "";
            // case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR: return "";
            // case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR: return "";
            // case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR: return "";
            // case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR: return "";
            // case VK_FORMAT_G8_B8R8_2PLANE_444_UNORM_EXT: return "";
            // case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT: return "";
            // case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT: return "";
            // case VK_FORMAT_G16_B16R16_2PLANE_444_UNORM_EXT: return "";
            // case VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT: return "";
            // case VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT: return "";
            // case VK_FORMAT_MAX_ENUM: return "";
            }
        }

        std::string VkColorspaceToString(const VkColorSpaceKHR& colorspace){
            switch (colorspace){
                case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR: return "VK_COLOR_SPACE_SRGB_NONLINEAR_KHR";
                case VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT: return "VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT";
                case VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT: return "VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT";
                case VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT: return "VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT";
                case VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT: return "VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT";
                case VK_COLOR_SPACE_BT709_LINEAR_EXT: return "VK_COLOR_SPACE_BT709_LINEAR_EXT";
                case VK_COLOR_SPACE_BT709_NONLINEAR_EXT: return "VK_COLOR_SPACE_BT709_NONLINEAR_EXT";
                case VK_COLOR_SPACE_BT2020_LINEAR_EXT: return "VK_COLOR_SPACE_BT2020_LINEAR_EXT";
                case VK_COLOR_SPACE_HDR10_ST2084_EXT: return "VK_COLOR_SPACE_HDR10_ST2084_EXT";
                case VK_COLOR_SPACE_DOLBYVISION_EXT: return "VK_COLOR_SPACE_DOLBYVISION_EXT";
                case VK_COLOR_SPACE_HDR10_HLG_EXT: return "VK_COLOR_SPACE_HDR10_HLG_EXT";
                case VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT: return "VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT";
                case VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT: return "VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT";
                case VK_COLOR_SPACE_PASS_THROUGH_EXT: return "VK_COLOR_SPACE_PASS_THROUGH_EXT";
                case VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT: return "VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT";
                case VK_COLOR_SPACE_DISPLAY_NATIVE_AMD: return "VK_COLOR_SPACE_DISPLAY_NATIVE_AMD";
                // case VK_COLORSPACE_SRGB_NONLINEAR_KHR: return "";
                // case VK_COLOR_SPACE_DCI_P3_LINEAR_EXT: return "";
                case VK_COLOR_SPACE_MAX_ENUM_KHR: return "VK_COLOR_SPACE_MAX_ENUM_KHR";
            }

            return "VkColorspaceToString undefined string";
        }

        std::string VkMemoryPropertyFlagToString(const VkMemoryPropertyFlags& flags){
            switch(flags){
                case VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT: return "VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT";
                case VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT: return "VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT";
                case VK_MEMORY_PROPERTY_HOST_COHERENT_BIT: return "VK_MEMORY_PROPERTY_HOST_COHERENT_BIT";
                case VK_MEMORY_PROPERTY_HOST_CACHED_BIT: return "VK_MEMORY_PROPERTY_HOST_CACHED_BIT";
                case VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT: return "VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT";
                case VK_MEMORY_PROPERTY_PROTECTED_BIT: return "VK_MEMORY_PROPERTY_PROTECTED_BIT";
                case VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD: return "VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD";
                case VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD: return "VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD";
                case VK_MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV: return "VK_MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV";
                case VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM: return "VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM";
                default: return "VkMemoryPropertyFlagToString Undefined String Returned!";
            }
            return "VkMemoryPropertyFlagToString Undefined String Returned!";
        }
    }; // end of vk namespace
};