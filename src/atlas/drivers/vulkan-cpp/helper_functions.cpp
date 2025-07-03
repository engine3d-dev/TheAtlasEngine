#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <vulkan/vulkan_core.h>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <core/engine_logger.hpp>

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

    vk_buffer create_buffer(const vk_buffer_info& p_construct_info) {
        vk_driver driver = vk_context::driver_context();

        vk_buffer new_buffer{};
        new_buffer.allocation_size = p_construct_info.device_size;

        VkBufferCreateInfo buffer_ci = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = new_buffer.allocation_size, // size in bytes
            .usage = p_construct_info.usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        // 1. creating our buffer
        vk_check(
          vkCreateBuffer(driver, &buffer_ci, nullptr, &new_buffer.handler),
          "vkCreateBuffer",
          __FILE__,
          __LINE__,
          __FUNCTION__);

        // 2.  getting buffer memory requirements
        VkMemoryRequirements memory_requirements = {};
        vkGetBufferMemoryRequirements(
          driver, new_buffer.handler, &memory_requirements);

        // 3. get memory type index
        uint32_t memory_type_index =
          driver.select_memory_type(memory_requirements.memoryTypeBits,
                                    p_construct_info.memory_property_flag);

        // 4. allocate memory
        // Memory Type Index
        // - Physical device enumerate all the physical hardware on your machine

        VkMemoryAllocateInfo memory_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memory_requirements.size,
            .memoryTypeIndex = memory_type_index
        };

        vk_check(
          vkAllocateMemory(
            driver, &memory_alloc_info, nullptr, &new_buffer.device_memory),
          "vkAllocateMemory",
          __FILE__,
          __LINE__,
          __FUNCTION__);

        // 5. bind memory
        vk_check(vkBindBufferMemory(
                   driver, new_buffer.handler, new_buffer.device_memory, 0),
                 "vkBindBufferMemory",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        return new_buffer;
    }

    VkImageView create_image_view(const VkImage& p_image,
                                  VkFormat p_format,
                                  VkImageAspectFlags p_aspect_flags) {
        vk_driver driver = vk_context::driver_context();
        VkImageViewCreateInfo view_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = p_image,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = p_format,
            .components = { .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                            .a = VK_COMPONENT_SWIZZLE_IDENTITY },
            .subresourceRange = { .aspectMask = p_aspect_flags,
                                  .baseMipLevel = 0,
                                  .levelCount = 1,
                                  .baseArrayLayer = 0,
                                  .layerCount = 1 }
        };

        VkImageView image_view = nullptr;
        VkResult res =
          vkCreateImageView(driver, &view_info, nullptr, &image_view);
        vk_check(res, "vkCreateImageView", __FILE__, __LINE__, __FUNCTION__);
        return image_view;
    }

    VkImageView create_image_view(const VkDevice& p_driver,
                                  VkImage p_image,
                                  VkSurfaceFormatKHR p_surface_format,
                                  VkImageAspectFlags p_aspect_flags,
                                  VkImageViewType p_view_t,
                                  uint32_t p_layer_count,
                                  uint32_t p_mip_levels) {
        VkImageViewCreateInfo image_view_ci = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = p_image,
            .viewType = p_view_t,
            .format = p_surface_format.format,
            .components = { .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                            .a = VK_COMPONENT_SWIZZLE_IDENTITY },
            .subresourceRange = { .aspectMask = p_aspect_flags,
                                  .baseMipLevel = 0,
                                  .levelCount = p_mip_levels,
                                  .baseArrayLayer = 0,
                                  .layerCount = p_layer_count },
        };

        VkImageView image_view;
        vk_check(
          vkCreateImageView(p_driver, &image_view_ci, nullptr, &image_view),
          "vkCreateImageView",
          __FILE__,
          __LINE__,
          __FUNCTION__);

        return image_view;
    }

    vk_image create_image2d(uint32_t p_width,
                            uint32_t p_height,
                            VkFormat p_format,
                            VkImageUsageFlags p_usage,
                            VkMemoryPropertyFlagBits p_property) {
        vk_driver driver = vk_context::driver_context();

        VkImageCreateInfo image_ci = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .imageType = VK_IMAGE_TYPE_2D,
            .format = p_format,
            .extent = { .width = p_width, .height = p_height, .depth = 1 },
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = p_usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices = nullptr,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
        };

        // VkImage image=nullptr;
        vk_image image;

        vk_check(vkCreateImage(driver, &image_ci, nullptr, &image.image),
                 "vkCreateImage",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        // 2. get buffer memory requirements
        VkMemoryRequirements memory_requirements;
        vkGetImageMemoryRequirements(driver, image.image, &memory_requirements);

        // 3. get memory type index
        uint32_t memory_type_index = driver.select_memory_type(
          memory_requirements.memoryTypeBits, p_property);

        // 4. Allocate info
        VkMemoryAllocateInfo memory_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = nullptr,
            .allocationSize = memory_requirements.size,
            .memoryTypeIndex = memory_type_index
        };

        vk_check(vkAllocateMemory(
                   driver, &memory_alloc_info, nullptr, &image.device_memory),
                 "vkAllocateMemory",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        // 5. bind image memory
        vk_check(vkBindImageMemory(driver, image.image, image.device_memory, 0),
                 "vkBindImageMemory",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        return image;
    }

    VkSampler create_sampler(const vk_filter_range& p_range,
                             VkSamplerAddressMode p_address_mode) {
        VkDevice driver = vk_context::driver_context();

        VkSamplerCreateInfo sampler_info = {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .magFilter = p_range.min,
            .minFilter = p_range.max,
            .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
            .addressModeU = p_address_mode,
            .addressModeV = p_address_mode,
            .addressModeW = p_address_mode,
            .mipLodBias = 0.0f,
            .anisotropyEnable = false,
            .maxAnisotropy = 1,
            .compareEnable = false,
            .compareOp = VK_COMPARE_OP_ALWAYS,
            .minLod = 0.0f,
            .maxLod = 0.0f,
            .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
            .unnormalizedCoordinates = false
        };

        VkSampler sampler;
        VkResult res =
          vkCreateSampler(driver, &sampler_info, VK_NULL_HANDLE, &sampler);
        vk_check(res, "vkCreateSampler", __FILE__, __LINE__, __FUNCTION__);

        return sampler;
    }

    VkCommandBufferLevel to_vk_command_buffer_level(
      const command_buffer_levels& p_levels) {
        switch (p_levels) {
            case command_buffer_levels::primary:
                return VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            case command_buffer_levels::secondary:
                return VK_COMMAND_BUFFER_LEVEL_SECONDARY;
            case command_buffer_levels::max_enum:
                return VK_COMMAND_BUFFER_LEVEL_MAX_ENUM;
        }

        console_log_error("command_buffer_levels was invalid!!!");
    }

    VkShaderStageFlags to_vk_shader_stage(const shader_stage& p_stage) {
        switch (p_stage) {
            case shader_stage::vertex:
                return VK_SHADER_STAGE_VERTEX_BIT;
            case shader_stage::fragment:
                return VK_SHADER_STAGE_FRAGMENT_BIT;
            default:
                return 0;
        }
    }

    VkShaderStageFlagBits to_vk_shader_stage_bits(const shader_stage& p_stage) {
        switch (p_stage) {
            case shader_stage::vertex:
                return VK_SHADER_STAGE_VERTEX_BIT;
            case shader_stage::fragment:
                return VK_SHADER_STAGE_FRAGMENT_BIT;
            default:
                break;
        }

        return VK_SHADER_STAGE_VERTEX_BIT;
    }

    vk_buffer create_uniform_buffer(uint32_t p_size) {
        vk_buffer_info uniform_info = {
            .device_size = p_size,
            .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            .memory_property_flag = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        };
        vk_buffer buffer_data = create_buffer(uniform_info);

        return buffer_data;
    }

    bool has_stencil_attachment(VkFormat p_format) {
        return ((p_format == VK_FORMAT_D32_SFLOAT_S8_UINT) ||
                (p_format == VK_FORMAT_D24_UNORM_S8_UINT));
    }

    void vk_check_format(VkFormat p_format,
                         const char* p_function_name,
                         const char* p_filepath,
                         uint32_t p_line) {
        if (p_format == VK_FORMAT_UNDEFINED) {
            console_log_error_tagged(
              "vulkan",
              "VkFormat failed in file: {0} --- Line: {1} --- In Function: {2}",
              p_filepath,
              p_line,
              p_function_name);
            console_log_error_tagged(
              "vulkan", "VkFormat was assigned to VK_FORMAT_UNDEFINED");
        }
    }

    VkSemaphore create_semaphore(const VkDevice& p_driver) {
        // creating semaphores
        VkSemaphoreCreateInfo semaphore_ci = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0
        };

        VkSemaphore semaphore;
        vk_check(
          vkCreateSemaphore(p_driver, &semaphore_ci, nullptr, &semaphore),
          "vkCreateSemaphore",
          __FILE__,
          __LINE__,
          __FUNCTION__);
        return semaphore;
    }

    std::string vk_present_mode_to_string(VkPresentModeKHR p_present_mode) {
        switch (p_present_mode) {
            case VK_PRESENT_MODE_IMMEDIATE_KHR:
                return "VK_PRESENT_MODE_IMMEDIATE_KHR";
            case VK_PRESENT_MODE_MAILBOX_KHR:
                return "VK_PRESENT_MODE_MAILBOX_KHR";
            case VK_PRESENT_MODE_FIFO_KHR:
                return "VK_PRESENT_MODE_FIFO_KHR";
            case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
                return "VK_PRESENT_MODE_FIFO_RELAXED_KHR";
            case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
                return "VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR";
            case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
                return "VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR";
            case VK_PRESENT_MODE_MAX_ENUM_KHR:
                return "VK_PRESENT_MODE_MAX_ENUM_KHR";
            default:
                return "Present Mode Unspecified!";
        }
    }

    std::string vk_format_to_string(VkFormat p_format) {
        switch (p_format) {
            case VK_FORMAT_UNDEFINED:
                return "VK_FORMAT_UNDEFINED";
            case VK_FORMAT_R4G4_UNORM_PACK8:
                return "VK_FORMAT_R4G4_UNORM_PACK8";
            case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
                return "VK_FORMAT_R4G4B4A4_UNORM_PACK16";
            case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
                return "VK_FORMAT_B4G4R4A4_UNORM_PACK16";
            case VK_FORMAT_R5G6B5_UNORM_PACK16:
                return "VK_FORMAT_R5G6B5_UNORM_PACK16";
            case VK_FORMAT_B5G6R5_UNORM_PACK16:
                return "VK_FORMAT_B5G6R5_UNORM_PACK16";
            case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
                return "VK_FORMAT_R5G5B5A1_UNORM_PACK16";
            case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
                return "VK_FORMAT_B5G5R5A1_UNORM_PACK16";
            case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
                return "VK_FORMAT_A1R5G5B5_UNORM_PACK16";
            case VK_FORMAT_R8_UNORM:
                return "VK_FORMAT_R8_UNORM";
            case VK_FORMAT_R8_SNORM:
                return "VK_FORMAT_R8_SNORM";
            case VK_FORMAT_R8_USCALED:
                return "VK_FORMAT_R8_USCALED";
            case VK_FORMAT_R8_SSCALED:
                return "VK_FORMAT_R8_SSCALED";
            case VK_FORMAT_R8_UINT:
                return "VK_FORMAT_R8_UINT";
            case VK_FORMAT_R8_SINT:
                return "VK_FORMAT_R8_SINT";
            case VK_FORMAT_R8_SRGB:
                return "VK_FORMAT_R8_SRGB";
            case VK_FORMAT_R8G8_UNORM:
                return "VK_FORMAT_R8G8_UNORM";
            case VK_FORMAT_R8G8_SNORM:
                return "VK_FORMAT_R8G8_SNORM";
            case VK_FORMAT_R8G8_USCALED:
                return "VK_FORMAT_R8G8_USCALED";
            case VK_FORMAT_R8G8_SSCALED:
                return "VK_FORMAT_R8G8_SSCALED";
            case VK_FORMAT_R8G8_UINT:
                return "VK_FORMAT_R8G8_UINT";
            case VK_FORMAT_R8G8_SINT:
                return "VK_FORMAT_R8G8_SINT";
            case VK_FORMAT_R8G8_SRGB:
                return "VK_FORMAT_R8G8_SRGB";
            case VK_FORMAT_R8G8B8_UNORM:
                return "VK_FORMAT_R8G8B8_UNORM";
            case VK_FORMAT_R8G8B8_SNORM:
                return "VK_FORMAT_R8G8B8_SNORM";
            case VK_FORMAT_R8G8B8_USCALED:
                return "VK_FORMAT_R8G8B8_USCALED";
            case VK_FORMAT_R8G8B8_SSCALED:
                return "VK_FORMAT_R8G8B8_SSCALED";
            case VK_FORMAT_R8G8B8_UINT:
                return "VK_FORMAT_R8G8B8_UINT";
            case VK_FORMAT_R8G8B8_SINT:
                return "VK_FORMAT_R8G8B8_SINT";
            case VK_FORMAT_R8G8B8_SRGB:
                return "VK_FORMAT_R8G8B8_SRGB";
            case VK_FORMAT_B8G8R8_UNORM:
                return "VK_FORMAT_B8G8R8_UNORM";
            case VK_FORMAT_B8G8R8_SNORM:
                return "VK_FORMAT_B8G8R8_SNORM";
            case VK_FORMAT_B8G8R8_USCALED:
                return "VK_FORMAT_B8G8R8_USCALED";
            case VK_FORMAT_B8G8R8_SSCALED:
                return "VK_FORMAT_B8G8R8_SSCALED";
            case VK_FORMAT_B8G8R8_UINT:
                return "VK_FORMAT_B8G8R8_UINT";
            case VK_FORMAT_B8G8R8_SINT:
                return "VK_FORMAT_B8G8R8_SINT";
            case VK_FORMAT_B8G8R8_SRGB:
                return "VK_FORMAT_B8G8R8_SRGB";
            case VK_FORMAT_R8G8B8A8_UNORM:
                return "VK_FORMAT_R8G8B8A8_UNORM";
            case VK_FORMAT_R8G8B8A8_SNORM:
                return "VK_FORMAT_R8G8B8A8_SNORM";
            case VK_FORMAT_R8G8B8A8_USCALED:
                return "VK_FORMAT_R8G8B8A8_USCALED";
            case VK_FORMAT_R8G8B8A8_SSCALED:
                return "VK_FORMAT_R8G8B8A8_SSCALED";
            case VK_FORMAT_R8G8B8A8_UINT:
                return "VK_FORMAT_R8G8B8A8_UINT";
            case VK_FORMAT_R8G8B8A8_SINT:
                return "VK_FORMAT_R8G8B8A8_SINT";
            case VK_FORMAT_R8G8B8A8_SRGB:
                return "VK_FORMAT_R8G8B8A8_SRGB";
            case VK_FORMAT_B8G8R8A8_UNORM:
                return "VK_FORMAT_B8G8R8A8_UNORM";
            case VK_FORMAT_B8G8R8A8_SNORM:
                return "VK_FORMAT_B8G8R8A8_SNORM";
            case VK_FORMAT_B8G8R8A8_USCALED:
                return "VK_FORMAT_B8G8R8A8_USCALED";
            case VK_FORMAT_B8G8R8A8_SSCALED:
                return "VK_FORMAT_B8G8R8A8_SSCALED";
            case VK_FORMAT_B8G8R8A8_UINT:
                return "VK_FORMAT_B8G8R8A8_UINT";
            case VK_FORMAT_B8G8R8A8_SINT:
                return "VK_FORMAT_B8G8R8A8_SINT";
            case VK_FORMAT_B8G8R8A8_SRGB:
                return "VK_FORMAT_B8G8R8A8_SRGB";
            case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
                return "VK_FORMAT_A8B8G8R8_UNORM_PACK32";
            case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
                return "VK_FORMAT_A8B8G8R8_SNORM_PACK32";
            case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
                return "VK_FORMAT_A8B8G8R8_USCALED_PACK32";
            case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
                return "VK_FORMAT_A8B8G8R8_SSCALED_PACK32";
            case VK_FORMAT_A8B8G8R8_UINT_PACK32:
                return "VK_FORMAT_A8B8G8R8_UINT_PACK32";
            case VK_FORMAT_A8B8G8R8_SINT_PACK32:
                return "VK_FORMAT_A8B8G8R8_SINT_PACK32";
            case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
                return "VK_FORMAT_A8B8G8R8_SRGB_PACK32";
            case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
                return "VK_FORMAT_A2R10G10B10_UNORM_PACK32";
            case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
                return "VK_FORMAT_A2R10G10B10_SNORM_PACK32";
            case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
                return "VK_FORMAT_A2R10G10B10_USCALED_PACK32";
            case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
                return "VK_FORMAT_A2R10G10B10_SSCALED_PACK32";
            case VK_FORMAT_A2R10G10B10_UINT_PACK32:
                return "VK_FORMAT_A2R10G10B10_UINT_PACK32";
            case VK_FORMAT_A2R10G10B10_SINT_PACK32:
                return "VK_FORMAT_A2R10G10B10_SINT_PACK32";
            case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
                return "VK_FORMAT_A2B10G10R10_UNORM_PACK32";
            case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
                return "VK_FORMAT_A2B10G10R10_SNORM_PACK32";
            case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
                return "VK_FORMAT_A2B10G10R10_USCALED_PACK32";
            case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
                return "VK_FORMAT_A2B10G10R10_SSCALED_PACK32";
            case VK_FORMAT_A2B10G10R10_UINT_PACK32:
                return "VK_FORMAT_A2B10G10R10_UINT_PACK32";
            case VK_FORMAT_A2B10G10R10_SINT_PACK32:
                return "VK_FORMAT_A2B10G10R10_SINT_PACK32";
            case VK_FORMAT_R16_UNORM:
                return "VK_FORMAT_R16_UNORM";
            case VK_FORMAT_R16_SNORM:
                return "VK_FORMAT_R16_SNORM";
            case VK_FORMAT_R16_USCALED:
                return "VK_FORMAT_R16_USCALED";
            case VK_FORMAT_R16_SSCALED:
                return "VK_FORMAT_R16_SSCALED";
            case VK_FORMAT_R16_UINT:
                return "VK_FORMAT_R16_UINT";
            case VK_FORMAT_R16_SINT:
                return "VK_FORMAT_R16_SINT";
            case VK_FORMAT_R16_SFLOAT:
                return "VK_FORMAT_R16_SFLOAT";
            case VK_FORMAT_R16G16_UNORM:
                return "VK_FORMAT_R16G16_UNORM";
            case VK_FORMAT_R16G16_SNORM:
                return "VK_FORMAT_R16G16_SNORM";
            case VK_FORMAT_R16G16_USCALED:
                return "VK_FORMAT_R16G16_USCALED";
            case VK_FORMAT_R16G16_SSCALED:
                return "VK_FORMAT_R16G16_SSCALED";
            case VK_FORMAT_R16G16_UINT:
                return "VK_FORMAT_R16G16_UINT";
            case VK_FORMAT_R16G16_SINT:
                return "VK_FORMAT_R16G16_SINT";
            case VK_FORMAT_R16G16_SFLOAT:
                return "VK_FORMAT_R16G16_SFLOAT";
            case VK_FORMAT_R16G16B16_UNORM:
                return "VK_FORMAT_R16G16B16_UNORM";
            case VK_FORMAT_R16G16B16_SNORM:
                return "VK_FORMAT_R16G16B16_SNORM";
            case VK_FORMAT_R16G16B16_USCALED:
                return "VK_FORMAT_R16G16B16_USCALED";
            case VK_FORMAT_R16G16B16_SSCALED:
                return "VK_FORMAT_R16G16B16_SSCALED";
            case VK_FORMAT_R16G16B16_UINT:
                return "VK_FORMAT_R16G16B16_UINT";
            case VK_FORMAT_R16G16B16_SINT:
                return "VK_FORMAT_R16G16B16_SINT";
            case VK_FORMAT_R16G16B16_SFLOAT:
                return "VK_FORMAT_R16G16B16_SFLOAT";
            case VK_FORMAT_R16G16B16A16_UNORM:
                return "VK_FORMAT_R16G16B16A16_UNORM";
            case VK_FORMAT_R16G16B16A16_SNORM:
                return "VK_FORMAT_R16G16B16A16_SNORM";
            case VK_FORMAT_R16G16B16A16_USCALED:
                return "VK_FORMAT_R16G16B16A16_USCALED";
            case VK_FORMAT_R16G16B16A16_SSCALED:
                return "VK_FORMAT_R16G16B16A16_SSCALED";
            case VK_FORMAT_R16G16B16A16_UINT:
                return "VK_FORMAT_R16G16B16A16_UINT";
            case VK_FORMAT_R16G16B16A16_SINT:
                return "VK_FORMAT_R16G16B16A16_SINT";
            case VK_FORMAT_R16G16B16A16_SFLOAT:
                return "VK_FORMAT_R16G16B16A16_SFLOAT";
            case VK_FORMAT_R32_UINT:
                return "VK_FORMAT_R32_UINT";
            case VK_FORMAT_R32_SINT:
                return "VK_FORMAT_R32_SINT";
            case VK_FORMAT_R32_SFLOAT:
                return "VK_FORMAT_R32_SFLOAT";
            case VK_FORMAT_R32G32_UINT:
                return "VK_FORMAT_R32G32_UINT";
            case VK_FORMAT_R32G32_SINT:
                return "VK_FORMAT_R32G32_SINT";
            case VK_FORMAT_R32G32_SFLOAT:
                return "VK_FORMAT_R32G32_SFLOAT";
            case VK_FORMAT_R32G32B32_UINT:
                return "VK_FORMAT_R32G32B32_UINT";
            case VK_FORMAT_R32G32B32_SINT:
                return "VK_FORMAT_R32G32B32_SINT";
            case VK_FORMAT_R32G32B32_SFLOAT:
                return "VK_FORMAT_R32G32B32_SFLOAT";
            case VK_FORMAT_R32G32B32A32_UINT:
                return "VK_FORMAT_R32G32B32A32_UINT";
            case VK_FORMAT_R32G32B32A32_SINT:
                return "VK_FORMAT_R32G32B32A32_SINT";
            case VK_FORMAT_R32G32B32A32_SFLOAT:
                return "VK_FORMAT_R32G32B32A32_SFLOAT";
            case VK_FORMAT_R64_UINT:
                return "VK_FORMAT_R64_UINT";
            case VK_FORMAT_R64_SINT:
                return "VK_FORMAT_R64_SINT";
            case VK_FORMAT_R64_SFLOAT:
                return "VK_FORMAT_R64_SFLOAT";
            case VK_FORMAT_R64G64_UINT:
                return "VK_FORMAT_R64G64_UINT";
            case VK_FORMAT_R64G64_SINT:
                return "VK_FORMAT_R64G64_SINT";
            case VK_FORMAT_R64G64_SFLOAT:
                return "VK_FORMAT_R64G64_SFLOAT";
            case VK_FORMAT_R64G64B64_UINT:
                return "VK_FORMAT_R64G64B64_UINT";
            case VK_FORMAT_R64G64B64_SINT:
                return "VK_FORMAT_R64G64B64_SINT";
            case VK_FORMAT_R64G64B64_SFLOAT:
                return "VK_FORMAT_R64G64B64_SFLOAT";
            case VK_FORMAT_R64G64B64A64_UINT:
                return "VK_FORMAT_R64G64B64A64_UINT";
            case VK_FORMAT_R64G64B64A64_SINT:
                return "VK_FORMAT_R64G64B64A64_SINT";
            case VK_FORMAT_R64G64B64A64_SFLOAT:
                return "VK_FORMAT_R64G64B64A64_SFLOAT";
            case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
                return "VK_FORMAT_B10G11R11_UFLOAT_PACK32";
            case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
                return "VK_FORMAT_E5B9G9R9_UFLOAT_PACK32";
            case VK_FORMAT_D16_UNORM:
                return "VK_FORMAT_D16_UNORM";
            case VK_FORMAT_X8_D24_UNORM_PACK32:
                return "VK_FORMAT_X8_D24_UNORM_PACK32";
            case VK_FORMAT_D32_SFLOAT:
                return "VK_FORMAT_D32_SFLOAT";
            case VK_FORMAT_S8_UINT:
                return "VK_FORMAT_S8_UINT";
            case VK_FORMAT_D16_UNORM_S8_UINT:
                return "VK_FORMAT_D16_UNORM_S8_UINT";
            case VK_FORMAT_D24_UNORM_S8_UINT:
                return "VK_FORMAT_D24_UNORM_S8_UINT";
            case VK_FORMAT_D32_SFLOAT_S8_UINT:
                return "VK_FORMAT_D32_SFLOAT_S8_UINT";
            case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
                return "VK_FORMAT_BC1_RGB_UNORM_BLOCK";
            case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
                return "VK_FORMAT_BC1_RGB_SRGB_BLOCK";
            case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
                return "VK_FORMAT_BC1_RGBA_UNORM_BLOCK";
            case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
                return "VK_FORMAT_BC1_RGBA_SRGB_BLOCK";
            case VK_FORMAT_BC2_UNORM_BLOCK:
                return "VK_FORMAT_BC2_UNORM_BLOCK";
            case VK_FORMAT_BC2_SRGB_BLOCK:
                return "VK_FORMAT_BC2_SRGB_BLOCK";
            case VK_FORMAT_BC3_UNORM_BLOCK:
                return "VK_FORMAT_BC3_UNORM_BLOCK";
            case VK_FORMAT_BC3_SRGB_BLOCK:
                return "VK_FORMAT_BC3_SRGB_BLOCK";
            case VK_FORMAT_BC4_UNORM_BLOCK:
                return "VK_FORMAT_BC4_UNORM_BLOCK";
            case VK_FORMAT_BC4_SNORM_BLOCK:
                return "VK_FORMAT_BC4_SNORM_BLOCK";
            case VK_FORMAT_BC5_UNORM_BLOCK:
                return "VK_FORMAT_BC5_UNORM_BLOCK";
            case VK_FORMAT_BC5_SNORM_BLOCK:
                return "VK_FORMAT_BC5_SNORM_BLOCK";
            case VK_FORMAT_BC6H_UFLOAT_BLOCK:
                return "VK_FORMAT_BC6H_UFLOAT_BLOCK";
            case VK_FORMAT_BC6H_SFLOAT_BLOCK:
                return "VK_FORMAT_BC6H_SFLOAT_BLOCK";
            case VK_FORMAT_BC7_UNORM_BLOCK:
                return "VK_FORMAT_BC7_UNORM_BLOCK";
            case VK_FORMAT_BC7_SRGB_BLOCK:
                return "VK_FORMAT_BC7_SRGB_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
                return "VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
                return "VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
                return "VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
                return "VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
                return "VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK";
            case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
                return "VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK";
            case VK_FORMAT_EAC_R11_UNORM_BLOCK:
                return "VK_FORMAT_EAC_R11_UNORM_BLOCK";
            case VK_FORMAT_EAC_R11_SNORM_BLOCK:
                return "VK_FORMAT_EAC_R11_SNORM_BLOCK";
            case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
                return "VK_FORMAT_EAC_R11G11_UNORM_BLOCK";
            case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
                return "VK_FORMAT_EAC_R11G11_SNORM_BLOCK";
            case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_4x4_UNORM_BLOCK";
            case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_4x4_SRGB_BLOCK";
            case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_5x4_UNORM_BLOCK";
            case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_5x4_SRGB_BLOCK";
            case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_5x5_UNORM_BLOCK";
            case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_5x5_SRGB_BLOCK";
            case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_6x5_UNORM_BLOCK";
            case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_6x5_SRGB_BLOCK";
            case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_6x6_UNORM_BLOCK";
            case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_6x6_SRGB_BLOCK";
            case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_8x5_UNORM_BLOCK";
            case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_8x5_SRGB_BLOCK";
            case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_8x6_UNORM_BLOCK";
            case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_8x6_SRGB_BLOCK";
            case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_8x8_UNORM_BLOCK";
            case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_8x8_SRGB_BLOCK";
            case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_10x5_UNORM_BLOCK";
            case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_10x5_SRGB_BLOCK";
            case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_10x6_UNORM_BLOCK";
            case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_10x6_SRGB_BLOCK";
            case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_10x8_UNORM_BLOCK";
            case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_10x8_SRGB_BLOCK";
            case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_10x10_UNORM_BLOCK";
            case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_10x10_SRGB_BLOCK";
            case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_12x10_UNORM_BLOCK";
            case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_12x10_SRGB_BLOCK";
            case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
                return "VK_FORMAT_ASTC_12x12_UNORM_BLOCK";
            case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
                return "VK_FORMAT_ASTC_12x12_SRGB_BLOCK";
            case VK_FORMAT_G8B8G8R8_422_UNORM:
                return "VK_FORMAT_G8B8G8R8_422_UNORM";
            case VK_FORMAT_B8G8R8G8_422_UNORM:
                return "VK_FORMAT_B8G8R8G8_422_UNORM";
            case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM:
                return "VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM";
            case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM:
                return "VK_FORMAT_G8_B8R8_2PLANE_420_UNORM";
            case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM:
                return "VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM";
            case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM:
                return "VK_FORMAT_G8_B8R8_2PLANE_422_UNORM";
            case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM:
                return "VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM";
            case VK_FORMAT_R10X6_UNORM_PACK16:
                return "VK_FORMAT_R10X6_UNORM_PACK16";
            case VK_FORMAT_R10X6G10X6_UNORM_2PACK16:
                return "VK_FORMAT_R10X6G10X6_UNORM_2PACK16";
            case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16:
                return "VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16";
            case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:
                return "VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16";
            case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:
                return "VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16";
            case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
                return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16";
            case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
                return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16";
            case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
                return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16";
            case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
                return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16";
            case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
                return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16";
            case VK_FORMAT_R12X4_UNORM_PACK16:
                return "VK_FORMAT_R12X4_UNORM_PACK16";
            case VK_FORMAT_R12X4G12X4_UNORM_2PACK16:
                return "VK_FORMAT_R12X4G12X4_UNORM_2PACK16";
            case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16:
                return "VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16";
            case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:
                return "VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16";
            case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:
                return "VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16";
            case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
                return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16";
            case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
                return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16";
            case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
                return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16";
            case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
                return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16";
            case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
                return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16";
            case VK_FORMAT_G16B16G16R16_422_UNORM:
                return "VK_FORMAT_G16B16G16R16_422_UNORM";
            case VK_FORMAT_B16G16R16G16_422_UNORM:
                return "VK_FORMAT_B16G16R16G16_422_UNORM";
            case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM:
                return "VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM";
            case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM:
                return "VK_FORMAT_G16_B16R16_2PLANE_420_UNORM";
            case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM:
                return "VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM";
            case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM:
                return "VK_FORMAT_G16_B16R16_2PLANE_422_UNORM";
            case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM:
                return "VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM";
            case VK_FORMAT_G8_B8R8_2PLANE_444_UNORM:
                return "VK_FORMAT_G8_B8R8_2PLANE_444_UNORM";
            case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16:
                return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16";
            case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16:
                return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16";
            case VK_FORMAT_G16_B16R16_2PLANE_444_UNORM:
                return "VK_FORMAT_G16_B16R16_2PLANE_444_UNORM";
            case VK_FORMAT_A4R4G4B4_UNORM_PACK16:
                return "VK_FORMAT_A4R4G4B4_UNORM_PACK16";
            case VK_FORMAT_A4B4G4R4_UNORM_PACK16:
                return "VK_FORMAT_A4B4G4R4_UNORM_PACK16";
            case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK";
            case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK:
                return "VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK";
            case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
                return "VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG";
            case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
                return "VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG";
            case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
                return "VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG";
            case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
                return "VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG";
            case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
                return "VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG";
            case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
                return "VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG";
            case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
                return "VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG";
            case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
                return "VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG";
            case VK_FORMAT_R16G16_SFIXED5_NV:
                return "VK_FORMAT_R16G16_SFIXED5_NV";
            case VK_FORMAT_A1B5G5R5_UNORM_PACK16_KHR:
                return "VK_FORMAT_A1B5G5R5_UNORM_PACK16_KHR";
            case VK_FORMAT_A8_UNORM_KHR:
                return "VK_FORMAT_A8_UNORM_KHR";
            case VK_FORMAT_MAX_ENUM:
                return "VK_FORMAT_MAX_ENUM";
            default:
                return "VkFormat Specified Invalid!!!";
        }
    }

    VkDescriptorType to_vk_descriptor_type(const buffer& p_type) {
        switch (p_type) {
            case buffer::storage:
                return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            case buffer::uniform:
                return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            case buffer::combined_image_sampler:
                return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            case buffer::sampled_image:
                return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        }

        console_log_error("descriptor set type specified is invalid!!!");
    }

    int bytes_per_texture_format(VkFormat p_format) {
        switch (p_format) {
            case VK_FORMAT_R8_SINT:
            case VK_FORMAT_R8_UNORM:
                return 1;
            case VK_FORMAT_R16_SFLOAT:
                return 2;
            case VK_FORMAT_R16G16_SFLOAT:
            case VK_FORMAT_B8G8R8A8_UNORM:
            case VK_FORMAT_R8G8B8A8_UNORM:
                return 4;
            case VK_FORMAT_R16G16B16A16_SFLOAT:
                return 4 * sizeof(uint16_t);
            case VK_FORMAT_R32G32B32A32_SFLOAT:
                return 4 * sizeof(float);
            default:
                console_log_fatal("Error unknown format!!!");
                return 0;
        }

        return 0;
    }

    VkPresentModeKHR select_compatible_present_mode(
      const VkPresentModeKHR& p_request,
      const std::span<VkPresentModeKHR>& p_modes) {
        for (const auto& mode : p_modes) {
            if (mode == p_request) {
                return mode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    uint32_t select_images_size(
      const VkSurfaceCapabilitiesKHR& p_surface_capabilities) {
        uint32_t requested_images = p_surface_capabilities.minImageCount + 1;

        uint32_t final_image_count = 0;

        if ((p_surface_capabilities.maxImageCount > 0) and
            (requested_images > p_surface_capabilities.maxImageCount)) {
            final_image_count = p_surface_capabilities.maxImageCount;
        }
        else {
            final_image_count = requested_images;
        }

        return final_image_count;
    }
};