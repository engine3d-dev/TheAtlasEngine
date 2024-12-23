#pragma once
#include <cstring>
#include <string>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    class VulkanTexture{
    public:
        VulkanTexture(const std::string& p_Filename);

        bool IsLoaded() { return m_IsTextureLoaded; }

        VkSampler GetSampler() { return m_Sampler; }
        VkImageView GetImageView() { return m_ImageView; }
        VkImageLayout GetImageLayout() { return m_ImageLayout; }

    private:
        void TransitionImageLayout(VkImageLayout p_OldImageLayout, VkImageLayout p_NewImageLayout);

    private:
        bool m_IsTextureLoaded=false;
        VkDeviceMemory m_DeviceMemory;
        VkImage m_Image;
        VkImageView m_ImageView;
        VkSampler m_Sampler;
        VkFormat m_ImageFormat;
        VkImageLayout m_ImageLayout;

        //! @note Buffer-related stuff
        VkBuffer m_TextureBuffer;
        VkDeviceSize m_BufferSize;
        VkDeviceSize m_AlignmentSize;
    };
};