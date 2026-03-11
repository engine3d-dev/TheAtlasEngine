module;

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <vector>
#include <span>
#include <string>

#define GLFW_INCLUDE_VULKAN
#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#else
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#endif
#include <stb_image.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <print>

export module atlas.drivers.vulkan.environment_map;

import atlas.drivers.vulkan.instance_context;

import vk;

namespace atlas {
    /**
     * We shift 32-bits to the high 32-bits for the old layout and OR the
     * new layout lsb 32-bits additional to the shifted 32-bits
     *
     * This is to ensure the image layouts do not overlap and can be used to
     * directly jump to set specific image loyouts rather then doing an
     * if-statement originally to check for that
     */
    constexpr uint64_t
    image_layout(VkImageLayout p_old, VkImageLayout p_new) {
        // Shift the old_layout into the high 32 bits, and combine with
        // new_layout in the low 32 bits.
        return (static_cast<uint64_t>(p_old) << 32) | static_cast<uint64_t>(p_new);
    }

    void
    memory_barrier(const VkCommandBuffer& p_command,
                const VkImage& p_image,
                VkFormat p_format,
                VkImageLayout p_old,
                VkImageLayout p_new,
                uint32_t p_layer_count = 1) {
        // 1. Image Memory Barrier Initialization (using C++ Designated
        // Initializers - C++20)
        VkImageMemoryBarrier image_memory_barrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = 0,
            .dstAccessMask = 0,
            .oldLayout = p_old,
            .newLayout = p_new,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = p_image,
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = p_layer_count,
            },
        };

        VkPipelineStageFlags source_stage = VK_PIPELINE_STAGE_NONE;
        VkPipelineStageFlags dst_stages = VK_PIPELINE_STAGE_NONE;

        // 2. Aspect Mask Logic (Keep as if/else, but use helper
        // function)
        if (p_new == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL ||
            vk::has_stencil_attachment(p_format)) {

            image_memory_barrier.subresourceRange.aspectMask =
            VK_IMAGE_ASPECT_DEPTH_BIT;

            // Assuming has_stencil_attachment(p_format) is defined
            // elsewhere works as the same as the if-statement, leaving
            // it here for testing purposes
            // image_memory_barrier.subresourceRange.aspectMask |=
            // has_stencil_attachment(p_format) ?
            // VK_IMAGE_ASPECT_STENCIL_BIT : VK_IMAGE_ASPECT_DEPTH_BIT;
            if (vk::has_stencil_attachment(p_format)) {
                image_memory_barrier.subresourceRange.aspectMask |=
                VK_IMAGE_ASPECT_STENCIL_BIT;
            }
        }
        else {
            image_memory_barrier.subresourceRange.aspectMask =
            VK_IMAGE_ASPECT_COLOR_BIT;
        }

        // 3. Main Transition Logic using Combined Switch
        const uint64_t current_layout = image_layout(p_old, p_new);

        switch (current_layout) {

            // UNDEFINED -> SHADER_READ_ONLY_OPTIMAL
            case image_layout(VK_IMAGE_LAYOUT_UNDEFINED,
                            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL): {
                image_memory_barrier.srcAccessMask = 0;
                image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                dst_stages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                break;
            }

            // UNDEFINED -> GENERAL
            case image_layout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL): {
                image_memory_barrier.srcAccessMask = 0;
                image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                // source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                dst_stages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                break;
            }

            // UNDEFINED -> TRANSFER_DST_OPTIMAL
            case image_layout(VK_IMAGE_LAYOUT_UNDEFINED,
                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL): {
                image_memory_barrier.srcAccessMask = 0;
                image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                dst_stages = VK_PIPELINE_STAGE_TRANSFER_BIT;
                break;
            }

            // SHADER_READ_ONLY_OPTIMAL -> TRANSFER_DST_OPTIMAL (Convert
            // back from read-only to transferr)
            case image_layout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL): {
                image_memory_barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                source_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                dst_stages = VK_PIPELINE_STAGE_TRANSFER_BIT;
                break;
            }

            // TRANSFER_DST_OPTIMAL -> SHADER_READ_ONLY_OPTIMAL (Convert
            // from updateable texture to shader read-only)
            case image_layout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL): {
                image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                dst_stages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                break;
            }

            // UNDEFINED -> DEPTH_STENCIL_ATTACHMENT_OPTIMAL (Convert
            // depth texture from undefined state)
            case image_layout(VK_IMAGE_LAYOUT_UNDEFINED,
                            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL): {
                image_memory_barrier.srcAccessMask = 0;
                image_memory_barrier.dstAccessMask =
                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                dst_stages = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
                break;
            }

            // SHADER_READ_ONLY_OPTIMAL -> SHADER_READ_ONLY_OPTIMAL
            // (Wait for render pass to complete - Note: This case is
            // unusual but kept as per your original logic)
            case image_layout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL): {
                // Note: Your original code had conflicting
                // re-assignments for source_stage/dst_stages here. The
                // last pair of assignments is used.
                image_memory_barrier.srcAccessMask = 0;
                image_memory_barrier.dstAccessMask = 0;
                source_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                dst_stages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                break;
            }

            // SHADER_READ_ONLY_OPTIMAL -> COLOR_ATTACHMENT_OPTIMAL
            // (Convert back from read-only to color attachment)
            case image_layout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL): {
                image_memory_barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                image_memory_barrier.dstAccessMask =
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                source_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                dst_stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                break;
            }

            // COLOR_ATTACHMENT_OPTIMAL -> SHADER_READ_ONLY_OPTIMAL
            // (Convert from updateable color to shader read-only)
            case image_layout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL): {
                image_memory_barrier.srcAccessMask =
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                source_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                dst_stages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                break;
            }

            // SHADER_READ_ONLY_OPTIMAL ->
            // DEPTH_STENCIL_ATTACHMENT_OPTIMAL (Convert back from
            // read-only to depth attachment)
            case image_layout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL): {
                image_memory_barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                image_memory_barrier.dstAccessMask =
                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                source_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                dst_stages = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                break;
            }

            // DEPTH_STENCIL_ATTACHMENT_OPTIMAL ->
            // SHADER_READ_ONLY_OPTIMAL (Convert from updateable depth
            // texture to shader read-only)
            case image_layout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL): {
                image_memory_barrier.srcAccessMask =
                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                source_stage = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                dst_stages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                break;
            }

            default: {
                // Unhandled Transition
                break;
            }
        }

        vkCmdPipelineBarrier(p_command,
                            source_stage,
                            dst_stages,
                            0, // dependencyFlags
                            0,
                            nullptr,
                            0,
                            nullptr,
                            1,
                            &image_memory_barrier);
    }

    export struct skybox_uniform {
        glm::mat4 proj_view;
    };

    export class environment_map {
    public:
        environment_map() = default;
        environment_map(const VkDevice& p_device,
                std::span<std::string> p_faces,
                VkPhysicalDeviceMemoryProperties p_memory_properties,
                VkRenderPass p_renderpass)
        : m_device(p_device) {

            create_skybox(p_faces, p_memory_properties);

            create_skybox_pipeline(p_memory_properties, p_renderpass);
        }

        // ~environment_map() {
        //     destroy();
        // }

        void create_skybox(std::span<const std::string> p_faces, VkPhysicalDeviceMemoryProperties p_memory_properties) {
            if (p_faces.size() != 6) {
                std::println("Cubemap requires 6 faces, got {}", p_faces.size());
                return;
            }

            int w = 0;
            int h = 0;
            int channels = 0;
            std::array<stbi_uc*, 6> pixel_arr{};
            pixel_arr[0] = stbi_load(p_faces[0].c_str(), &w, &h, &channels, STBI_rgb_alpha);
            int face_w = w;
            int face_h = h;

            for (size_t i = 1; i < 6; i++) {
                pixel_arr[i] = stbi_load(p_faces[i].c_str(), &w, &h, &channels, STBI_rgb_alpha);
                if (pixel_arr[i] == nullptr) {
                    std::println("Could not load cubemap face {}: {}", i, p_faces[i]);
                    return;
                }

                if (w != face_w || h != face_h) {
                    std::println(
                    "Cubemap faces must match dimensions. Face 0 is {}x{}, face {} is {}x{} ({})",
                    face_w,
                    face_h,
                    i,
                    w,
                    h,
                    p_faces[i]);
                    for (size_t j = 0; j <= i; j++) {
                        stbi_image_free(pixel_arr[j]);
                        pixel_arr[j] = nullptr;
                    }
                    return;
                }
            }



            const uint32_t width = static_cast<uint32_t>(face_w);
            const uint32_t height = static_cast<uint32_t>(face_h);

            VkFormat texture_format = VK_FORMAT_R8G8B8A8_SRGB;
            const uint32_t bytes_per_pixel =
            static_cast<uint32_t>(vk::bytes_per_texture_format(texture_format));
            const VkDeviceSize face_size_bytes =
            static_cast<VkDeviceSize>(width) * static_cast<VkDeviceSize>(height) *
            static_cast<VkDeviceSize>(bytes_per_pixel);
            const VkDeviceSize total_size_bytes = face_size_bytes * 6;

            VkImageCreateInfo image_ci = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .pNext = nullptr,
                .flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
                .imageType = VK_IMAGE_TYPE_2D,
                .format = texture_format,
                .extent = {
                    .width = width,
                    .height = height,
                    .depth = 1,
                },
                .mipLevels = 1,
                .arrayLayers = 6,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .tiling = VK_IMAGE_TILING_OPTIMAL,
                .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .queueFamilyIndexCount = 0,
                .pQueueFamilyIndices = nullptr,
            };

            vk::vk_check(
            vkCreateImage(m_device, &image_ci, nullptr, &m_skybox_image),
            "vkCreateImage");

            // memory properties
            VkMemoryRequirements memory_requirements;
            vkGetImageMemoryRequirements(m_device, m_skybox_image, &memory_requirements);

            // const auto property = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            const auto property = vk::memory_property::device_local_bit;
            uint32_t memory_index = vk::select_memory_requirements(p_memory_properties, memory_requirements, property);

            // 4. Allocate info
            VkMemoryAllocateInfo memory_alloc_info = {
                .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                .pNext = nullptr,
                .allocationSize = memory_requirements.size,
                .memoryTypeIndex = memory_index
            };

            vk::vk_check(vkAllocateMemory(
                    m_device, &memory_alloc_info, nullptr, &m_skybox_dev_memory),
                    "vkAllocateMemory");

            // 5. bind image memory
            vk::vk_check(vkBindImageMemory(m_device, m_skybox_image, m_skybox_dev_memory, 0),
                    "vkBindImageMemory");

            VkImageViewCreateInfo image_view_ci = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .image = m_skybox_image,
                .viewType = VK_IMAGE_VIEW_TYPE_CUBE,
                .format = texture_format,
                .components = {
                    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .a = VK_COMPONENT_SWIZZLE_IDENTITY,
                },
                .subresourceRange = {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 6u,
                },
            };

            vk::vk_check(vkCreateImageView(
                        m_device, &image_view_ci, nullptr, &m_skybox_image_view),
                        "vkCreateImageView");

            VkSamplerCreateInfo sampler_info = {
                .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .magFilter = VK_FILTER_LINEAR,
                .minFilter = VK_FILTER_LINEAR,
                .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
                .addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
                .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
                .addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
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

            vk::vk_check(vkCreateSampler(m_device, &sampler_info, nullptr, &m_skybox_sampler), "vkCreateSampler");

            // --- Upload all 6 faces via a single staging buffer ---
            VkBuffer staging_buffer = nullptr;
            VkDeviceMemory staging_memory = nullptr;

            VkBufferCreateInfo staging_buffer_ci = {
                .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .size = static_cast<VkDeviceSize>(total_size_bytes),
                .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .queueFamilyIndexCount = 0,
                .pQueueFamilyIndices = nullptr,
            };
            vk::vk_check(
            vkCreateBuffer(m_device, &staging_buffer_ci, nullptr, &staging_buffer),
            "vkCreateBuffer(staging)");

            VkMemoryRequirements staging_mem_req{};
            vkGetBufferMemoryRequirements(m_device, staging_buffer, &staging_mem_req);

            const auto staging_property = static_cast<vk::memory_property>(
            vk::memory_property::host_visible_bit |
            vk::memory_property::host_coherent_bit);
            uint32_t staging_memory_index = vk::select_memory_requirements(
            p_memory_properties, staging_mem_req, staging_property);

            VkMemoryAllocateInfo staging_alloc_info = {
                .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                .pNext = nullptr,
                .allocationSize = staging_mem_req.size,
                .memoryTypeIndex = staging_memory_index,
            };
            vk::vk_check(vkAllocateMemory(
                        m_device, &staging_alloc_info, nullptr, &staging_memory),
                        "vkAllocateMemory(staging)");
            vk::vk_check(vkBindBufferMemory(m_device, staging_buffer, staging_memory, 0),
                        "vkBindBufferMemory(staging)");

            void* mapped = nullptr;
            vk::vk_check(vkMapMemory(m_device,
                                    staging_memory,
                                    0,
                                    static_cast<VkDeviceSize>(total_size_bytes),
                                    0,
                                    &mapped),
                        "vkMapMemory(staging)");
            auto* mapped_bytes = static_cast<uint8_t*>(mapped);
            for (size_t face = 0; face < 6; face++) {
                const size_t dst_off = static_cast<size_t>(face_size_bytes) * face;
                std::memcpy(mapped_bytes + dst_off,
                            pixel_arr[face],
                            static_cast<size_t>(face_size_bytes));
            }
            vkUnmapMemory(m_device, staging_memory);

            // We no longer need CPU pixel data after staging copy.
            for (auto* px : pixel_arr) {
                stbi_image_free(px);
            }

            // Record: transition -> copy -> transition (all 6 layers)
            //
            // We intentionally use vk::command_buffer here (as requested).
            // IMPORTANT: to avoid VUID-vkCmdCopyBufferToImage-imageOffset-07738 on
            // some dedicated transfer queue families (minImageTransferGranularity = (0,0,0)),
            // we record on the GRAPHICS queue family command pool (family index 0).
            vk::command_params upload_params = {
                .levels = vk::command_levels::primary,
                .queue_index = 0, // graphics queue family index
                .flags = vk::command_pool_flags::reset,
            };
            vk::command_buffer upload_cmd(m_device, upload_params);
            upload_cmd.begin(vk::command_usage::one_time_submit);

            memory_barrier(upload_cmd,
                        m_skybox_image,
                        texture_format,
                        VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        6);

            // Some queue families report minImageTransferGranularity = (0,0,0),
            // meaning they only allow FULL image-subresource copies. Copying one
            // cubemap face (one layer) at a time can fail validation in that case.
            //
            // We pack the 6 faces sequentially in the staging buffer, so we can
            // copy all 6 layers in one shot.
            // VkBufferImageCopy region = {
            //     .bufferOffset = 0,
            //     .bufferRowLength = 0,
            //     .bufferImageHeight = 0,
            //     .imageSubresource =
            //       VkImageSubresourceLayers{
            //         .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            //         .mipLevel = 0,
            //         .baseArrayLayer = 0,
            //         .layerCount = 6,
            //     },
            //     .imageOffset = VkOffset3D{ .x = 0, .y = 0, .z = 0 },
            //     .imageExtent = VkExtent3D{ .width = width, .height = height, .depth = 1 },
            // };

            // vkCmdCopyBufferToImage(upload_cmd,
            //                        staging_buffer,
            //                        m_skybox_image,
            //                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            //                        1,
            //                        &region);
            std::array<VkBufferImageCopy, 6> regions;
            for (uint32_t face = 0; face < 6; face++) {
                VkBufferImageCopy region = {
                    .bufferOffset = face_size_bytes * face,
                    .bufferRowLength = 0,
                    .bufferImageHeight = 0,
                    .imageSubresource = {
                        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                        .mipLevel = 0,
                        .baseArrayLayer = face, // Copy to this specific face
                        .layerCount = 1,        // One at a time
                    },
                    .imageOffset = {0, 0, 0},
                    .imageExtent = { .width = width, .height = height, .depth = 1},
                };
                
                regions[face] = region;
            }

            vkCmdCopyBufferToImage(upload_cmd, staging_buffer, m_skybox_image, 
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                static_cast<uint32_t>(regions.size()), 
                                regions.data());
            
            memory_barrier(upload_cmd,
                        m_skybox_image,
                        texture_format,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                        6);
            upload_cmd.end();

            VkQueue graphics_queue = nullptr;
            vkGetDeviceQueue(m_device, /*queueFamilyIndex*/ 0, /*queueIndex*/ 0, &graphics_queue);
            const VkCommandBuffer cmd = upload_cmd;
            VkSubmitInfo submit = {
                .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .pNext = nullptr,
                .waitSemaphoreCount = 0,
                .pWaitSemaphores = nullptr,
                .pWaitDstStageMask = nullptr,
                .commandBufferCount = 1,
                .pCommandBuffers = &cmd,
                .signalSemaphoreCount = 0,
                .pSignalSemaphores = nullptr,
            };
            vk::vk_check(vkQueueSubmit(graphics_queue, 1, &submit, nullptr),
                        "vkQueueSubmit(cubemap upload)");
            vk::vk_check(vkQueueWaitIdle(graphics_queue),
                        "vkQueueWaitIdle(cubemap upload)");

            upload_cmd.destroy();

            vkDestroyBuffer(m_device, staging_buffer, nullptr);
            vkFreeMemory(m_device, staging_memory, nullptr);
        }


        void create_buffers() {
            std::vector<float> skyboxVertices = {
                // positions          
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f,  1.0f
            };
            // m_physical = instance_context::physical_driver();
            // m_device = instance_context::logical_device();

            // std::vector<vk::vertex_input> vertices = {
            //     vk::vertex_input{
            //         .position = {-1.0f,  1.0f, -1.0f,}
            //     },
            // };
            std::vector<vk::vertex_input> vertices = {
                // Front Face
                vk::vertex_input{{-1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{-1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

                // Left Face
                vk::vertex_input{{-1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{-1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{-1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{-1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{-1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

                // Right Face
                vk::vertex_input{{ 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

                // Back Face
                vk::vertex_input{{-1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{-1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{-1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

                // Top Face
                vk::vertex_input{{-1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{-1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{-1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

                // Bottom Face
                vk::vertex_input{{-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{-1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{-1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                vk::vertex_input{{ 1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}
            };
            // for (size_t i = 0; i < skyboxVertices.size(); i += 3) {
            //     vk::vertex_input v{};
                
            //     // Assign position from the float array
            //     v.position = glm::vec3(
            //         vertices[i], 
            //         vertices[i + 1],
            //         vertices[i + 2]
            //     );

            //     // Default values for the remaining fields
            //     v.color   = glm::vec3(1.0f); // Default white
            //     v.normals = glm::vec3(0.0f); // Or calculate if needed
            //     v.uv      = glm::vec2(0.0f);

            //     vertices.push_back(v);
            // }
            vk::vertex_params vbo_params = {
                .phsyical_memory_properties = vulkan::instance_context::physical_driver().memory_properties(),
                .vertices = vertices
            };

            m_skybox_vbo = vk::vertex_buffer(m_device, vbo_params);
        }

        void create_skybox_pipeline(VkPhysicalDeviceMemoryProperties p_memory_properties, const VkRenderPass& p_renderpass) {
            create_buffers();
            std::array<vk::vertex_attribute_entry, 4> attribute_entries = {
                vk::vertex_attribute_entry{
                  .location = 0,
                  .format = vk::format::rgb32_sfloat,
                  .stride = offsetof(vk::vertex_input, position),
                },
                vk::vertex_attribute_entry{
                  .location = 1,
                  .format = vk::format::rgb32_sfloat,
                  .stride = offsetof(vk::vertex_input, color),
                },
                vk::vertex_attribute_entry{
                  .location = 2,
                  .format = vk::format::rgb32_sfloat,
                  .stride = offsetof(vk::vertex_input, normals),
                },
                vk::vertex_attribute_entry{
                  .location = 3,
                  .format = vk::format::rg32_sfloat,
                  .stride = offsetof(vk::vertex_input, uv),
                }
            };
            std::array<vk::vertex_attribute, 1> attribute = {
                vk::vertex_attribute{
                  // layout (set = 0, binding = 0)
                  .binding = 0,
                  .entries = attribute_entries,
                  .stride = sizeof(vk::vertex_input),
                  .input_rate = vk::input_rate::vertex,
                },
            };

            const std::array<vk::shader_source, 2> sources = {
                vk::shader_source{
                .filename = "builtin.shaders/skybox/skybox.vert.spv",
                .stage = vk::shader_stage::vertex,
                },
                vk::shader_source{
                .filename = "builtin.shaders/skybox/skybox.frag.spv",
                .stage = vk::shader_stage::fragment,
                },
            };

            vk::shader_resource_info shader_info = {
                .sources = sources,
            };
            m_skybox_shaders = vk::shader_resource(m_device, shader_info);
            m_skybox_shaders.vertex_attributes(attribute);

            // set=0 binding=0 UBO: mat4 VP
            vk::uniform_params ubo_params = {
                .phsyical_memory_properties = p_memory_properties,
                .size_bytes = sizeof(skybox_uniform),
                .debug_name = "skybox_ubo",
                .vkSetDebugUtilsObjectNameEXT = nullptr,
            };
            m_skybox_ubo =vk::uniform_buffer(m_device, ubo_params);
            // vk::uniform_buffer(m_device, sizeof(skybox_uniform), ubo_params);

            skybox_uniform identity = { .proj_view = glm::mat4(1.0f) };
            identity.proj_view[1][1] *= -1;
            // m_skybox_ubo.write(std::span<const skybox_uniform>(&identity, 1));
            m_skybox_ubo.update(&identity);

            // set=0 bindings:
            //  - binding 0: UBO (vertex)
            //  - binding 1: samplerCube (fragment)
            std::array<vk::descriptor_entry, 2> entries = {
                vk::descriptor_entry{
                .type = vk::buffer::uniform,
                .binding_point =
                    vk::descriptor_binding_point{ .binding = 0,
                                                .stage = vk::shader_stage::vertex },
                .descriptor_count = 1,
                },
                vk::descriptor_entry{
                .type = vk::buffer::combined_image_sampler,
                .binding_point =
                    vk::descriptor_binding_point{
                    .binding = 1, .stage = vk::shader_stage::fragment },
                .descriptor_count = 1,
                },
            };

            vk::descriptor_layout desc_layout = {
                .slot = 0,
                .max_sets = 1,
                .entries = entries,
            };
            m_skybox_descriptors = vk::descriptor_resource(m_device, desc_layout);

            const std::array<vk::write_buffer, 1> ubo_writes = {
                vk::write_buffer{ .buffer = static_cast<VkBuffer>(m_skybox_ubo),
                                .offset = 0,
                                .range = static_cast<uint32_t>(sizeof(skybox_uniform)) },
            };
            const vk::write_buffer_descriptor ubo_write_desc = {
                .dst_binding = 0,
                .uniforms = ubo_writes,
            };

            const std::array<vk::write_image, 1> image_writes = {
                vk::write_image{
                .sampler = m_skybox_sampler,
                .view = m_skybox_image_view,
                .layout = vk::image_layout::shader_read_only_optimal,
                },
            };
            const vk::write_image_descriptor image_write_desc = {
                .dst_binding = 1,
                .sample_images = image_writes,
            };

            m_skybox_descriptors.update(std::span(&ubo_write_desc, 1),
                                        std::span(&image_write_desc, 1));

            const std::array<VkDescriptorSetLayout, 1> layouts = {
                m_skybox_descriptors.layout(),
            };

            const std::array<vk::color_blend_attachment_state, 1> blend_attachments = {
                vk::color_blend_attachment_state{ .blend_enabled = false },
            };
            vk::color_blend_state blend_state = {
                .logic_op_enable = false,
                .logical_op = vk::logical_op::copy,
                .attachments = blend_attachments,
                .blend_constants = {},
            };

            std::array<vk::dynamic_state, 2> dyn = {
                vk::dynamic_state::viewport,
                vk::dynamic_state::scissor,
            };

            // pipeline expects a non-const span<VkDescriptorSetLayout>
            std::array<VkDescriptorSetLayout, 1> pipeline_layouts = layouts;

            vk::pipeline_params pipe_info = {
                .renderpass = p_renderpass,
                .shader_modules = m_skybox_shaders.handles(),
                .vertex_attributes = m_skybox_shaders.vertex_attributes(),      // no vertex input
                .vertex_bind_attributes = m_skybox_shaders.vertex_bind_attributes(), // no vertex input
                .descriptor_layouts = pipeline_layouts,
                .input_assembly = vk::input_assembly_state{
                .topology = vk::primitive_topology::triangle_list,
                .primitive_restart_enable = false,
                },
                .viewport = vk::viewport_state{ .viewport_count = 1, .scissor_count = 1 },
                .rasterization = vk::rasterization_state{
                .polygon_mode = vk::polygon_mode::fill,
                  .cull_mode = vk::cull_mode::front_bit,
                // .cull_mode = vk::cull_mode::none,
                // .front_face = vk::front_face::counter_clockwise,
                    .front_face = vk::front_face::clockwise,
                    .line_width = 1.f,
                },
                .multisample = vk::multisample_state{},
                .color_blend = blend_state,
                .depth_stencil_enabled = true,
                .depth_stencil = vk::depth_stencil_state{
                .depth_test_enable = true,
                .depth_write_enable = false,
                .depth_compare_op = vk::compare_op::less_or_equal,
                .depth_bounds_test_enable = false,
                .stencil_test_enable = false,
                },
                .dynamic_states = dyn,
            };

            m_skybox_pipeline = vk::pipeline(m_device, pipe_info);

        }

        void update_uniform(const skybox_uniform& p_ubo) {
            // m_skybox_ubo.transfer(std::span<const skybox_uniform>(&p_ubo, 1));
            m_skybox_ubo.update(&p_ubo);
        }

        void bind(const VkCommandBuffer& p_current) {
            m_skybox_pipeline.bind(p_current);
            m_skybox_descriptors.bind(p_current, m_skybox_pipeline.layout());
            m_skybox_vbo.bind(p_current);
        }

        void draw(const VkCommandBuffer& p_current) {
            // bind(p_current);
            vkCmdDraw(p_current, m_skybox_vbo.size(), 1, 0, 0);
            // vkCmdDrawIndexed(p_current, 36, 1, 0, 0, 0);
        }

        void destroy() {

            if (m_skybox_pipeline.alive()) {
                m_skybox_pipeline.destroy();
            }
            m_skybox_descriptors.destroy();
            m_skybox_ubo.destroy();
            m_skybox_shaders.destroy();
            m_skybox_vbo.destroy();

            destroy_image();
        }

        void destroy_image() {
            if (m_skybox_image_view != nullptr) {
                vkDestroyImageView(m_device, m_skybox_image_view, nullptr);
            }

            // Boolean check is to make sure we might only want
            // to destroy vk::sample_image resources.

            // Example of this is the swapchain may pass in
            // its images and we should only destruct the VkImageView
            // and not the swapchain's images directly
            if (m_skybox_image != nullptr) {
                vkDestroyImage(m_device, m_skybox_image, nullptr);
            }

            if (m_skybox_sampler != nullptr) {
                vkDestroySampler(m_device, m_skybox_sampler, nullptr);
            }

            if (m_skybox_dev_memory != nullptr) {
                vkFreeMemory(m_device, m_skybox_dev_memory, nullptr);
            }
        }

    private:
        VkDevice m_device = nullptr;

        VkImage m_skybox_image = nullptr;
        VkImageView m_skybox_image_view = nullptr;
        VkDeviceMemory m_skybox_dev_memory = nullptr;
        VkSampler m_skybox_sampler = nullptr;

        vk::shader_resource m_skybox_shaders{};
        vk::uniform_buffer m_skybox_ubo{};
        vk::descriptor_resource m_skybox_descriptors{};
        vk::pipeline m_skybox_pipeline{};
        vk::vertex_buffer m_skybox_vbo;
    };

};