#pragma once

namespace engine3d{
    /**
    @name VulkanDevice
    @note Abstracting how we handle physical, logical devices and queue familties
    @note Checks for driver compatibilities, device suitability checks, and available queue families
    */
    class VulkanDevice{
    public:
        //! @note Initiating our physical, logical devices
        void InitializeVulkanDevices();

        //! @note Cleanup function making sure we have control when to do device drivers cleanup in Vulkan.
        void CleanupVulkanDevice();
    private:


        void PickPhysicalDevice();
        void InitializeLogicalDevice();
        void InitializeSurface();
        void InitializeSwapchain();

    };
};