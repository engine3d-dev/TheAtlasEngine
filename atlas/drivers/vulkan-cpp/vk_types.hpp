#pragma once
#include <vulkan/vulkan.h>

namespace atlas::vk {
    struct vk_image {
        VkImage Image = nullptr;
        VkImageView ImageView = nullptr;
        VkSampler Sampler = nullptr;
        VkDeviceMemory DeviceMemory = nullptr;
    };

    //! @brief contains the min and max ranges for the VkFilter
    struct vk_filter_range {
        VkFilter min;
        VkFilter max;
    };

    /**
     * @name vk_present_queue
     * @brief index is the queue family index the queue belongs to
     * @brief queue index in the specific queue family to retrieve
    */
    struct vk_queue_options {
        // uint32_t device_index=0; // device num
        uint32_t family_index=-1;
        uint32_t queue_index=0;
    };

};