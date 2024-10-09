#include <internal/VulkanCpp/VulkanLogicalDevice.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    VulkanLogicalDevice::VulkanLogicalDevice(VulkanPhysicalDevice& p_PhysicalDevice){
        VkPhysicalDeviceFeatures vulkanEnabledFeatures = {
            .independentBlend = true,                   // specifies when VkPipelineColorAttachmentState settings are controlled independently per attachment
            .fillModeNonSolid = true,                   // specifies whether point and wireframe fill modes are supported. If feature are not enabled, VK_POLYGON_MODE_POINT and VK_POLYGON_MODE_LINE will not be used.
            .wideLines = true,                          // specifies whether lines with width other than 1.0 are supported. Feature if not enabled, lineWidth member of VkPipelineRasterizationStateCreateInfo will default to 1.0 value
            .samplerAnisotropy = true,                  // specifies whether anisotropic filtering is supported. If feature isn't enabled, the "anisotropyEnable" member of "VKSamplerCreateInfo" structure must be VK_FALSE
            .pipelineStatisticsQuery = true,            // specifies whether the pipeline statistics queries are supported. If not enabled, query type "VK_QUERY_TYPE_PIPELINE_STATISTICS" cannot be created.
            .shaderStorageImageReadWithoutFormat = true,// specifies whether storage images and storage texel buffers require a format qualifier to be specified when reading. Applies only to formats listed in "storage without format" list.
        };

        m_PhysicalDevice = p_PhysicalDevice;

        m_EnabledPhysicalFeatures = vulkanEnabledFeatures;

    }

    VkDevice VulkanLogicalDevice::GetVkDeviceInstance(){
        return m_LogicalDevice;
    }

    VulkanPhysicalDevice& VulkanLogicalDevice::GetPhysicalDevice(){
        return m_PhysicalDevice;
    }

    VkPhysicalDeviceFeatures VulkanLogicalDevice::GetVkPhysicalDeviceFeatures(){
        return m_EnabledPhysicalFeatures;
    }

    VkQueue VulkanLogicalDevice::GetVkGraphicsQueue(){
        return m_GraphicsQueue;
    }

    VkQueue VulkanLogicalDevice::GetVkComputeQueue(){
        return m_ComputeQueue;
    }
};