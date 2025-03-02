#pragma once

#include <vulkan/vulkan_core.h>
namespace atlas::vk {

    void BeginCommandBuffer(VkCommandBuffer* p_CommandBuffer);
    void EndCommandBuffer(VkCommandBuffer* p_CommandBuffer);
};