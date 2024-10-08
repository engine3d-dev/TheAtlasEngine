#include <Core/internal/VulkanCpp/VulkanDevice.h>

namespace engine3d::vk{
    /**
     * @note Having a static pointer to the actual data of the logical device.
     * @note Reasons for this: Is because if we dont then the compiler treats this as as a copyable object that wont get instantiated.
     * @note To see the error, make g_LogicalDeviceInstance a non-pointer to receive the error.
    */
    static VulkanLogicalDevice* g_LogicalDeviceInstance;

    void VulkanDevice::InitializeVulkanDevice(){
        VulkanPhysicalDevice physicalDevice = vk::VulkanPhysicalDevice();
        vk::VulkanLogicalDevice logical = vk::VulkanLogicalDevice(physicalDevice);
        g_LogicalDeviceInstance = &logical;
    }

    void VulkanDevice::CleanupVulkanDevices(){
        delete g_LogicalDeviceInstance;
    }

    VulkanPhysicalDevice& VulkanDevice::GetPhysicalDevice() {
        return g_LogicalDeviceInstance->GetPhysicalDevice();
    }

    VulkanLogicalDevice& VulkanDevice::GetLogicalDevice(){
        return *g_LogicalDeviceInstance;
    }

    // RendererPipeline* Create(){
    //     switch (CurrentAPI()){
    //     case API::VULKAN:
    //         return new VulkanRenderer();
    //     case API::DIRECTX12:
    //         return new Dx12Renderer();
    //     default: return nullptr;
    //     }

    //     return nullptr;
    // }

};