#include <internal/Vulkan2Showcase/VulkanTexture.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace engine3d::vk{
    static VkDeviceSize GetAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment){
        if(minOffsetAlignment > 0){
            return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
        }

        return instanceSize;
    }

    VulkanTexture::VulkanTexture(const std::string& p_Filename){
        int width, height, channels, bytesPerPixel;
        
        // Loading our image
        stbi_uc* data = stbi_load(p_Filename.c_str(), &width, &height, &bytesPerPixel , 4);

        /*
        StagingBuffer(
            device,
            instanceSize: 4,
            instanceCount: static_cast<uint32_t>(width * height),
            usageFlags: VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            memoryPropertyFlags: VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        )
        */
        VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VkMemoryPropertyFlags memory_property_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        VkDeviceSize instanceSize = 4, instanceCount = static_cast<uint32_t>(width * height);
        m_AlignmentSize = GetAlignment(instanceSize, )

    }

    void VulkanTexture::TransitionImageLayout(VkImageLayout p_OldImageLayout, VkImageLayout p_NewImageLayout){
    }
};