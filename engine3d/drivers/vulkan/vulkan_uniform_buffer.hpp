#pragma once
#include <string>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    
    /**
     * @name Vulkan Uniform Buffer Object
     * @note Defines how we create different uniform buffers
     * @note Read-only for shaders
    */
    class VulkanUniformBuffer{
    public:
        VulkanUniformBuffer() = default;
        VulkanUniformBuffer(uint32_t p_Size);

    private:
        VkBuffer m_BufferObject;
    };
};