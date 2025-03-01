#pragma once
#include <vulkan/vulkan_core.h>

namespace atlas::vk{
    
    /**
     * @name Vulkan Uniform Buffer Object
     * @note Defines how we create different uniform buffers
     * @note Read-only for shaders
    */
    class VulkanUniformBuffer{
    public:
        VulkanUniformBuffer() = default;
        /**
         * @param p_size is the buffer size
         * @param p_count instance count
        */
        VulkanUniformBuffer(uint32_t p_size, uint32_t p_count);

        void AllocateUniforms();
        void MapWrite(uint32_t p_size, uint32_t p_offset, const void* p_src);

    private:
        VkBuffer m_BufferObject;
        VkDeviceMemory m_BufferMemory;
    };
};