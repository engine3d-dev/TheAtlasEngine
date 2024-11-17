#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>
#include <Core/internal/Vulkan2Showcase/VulkanDriver.hpp>

namespace engine3d::vk{
    class VulkanContext{
    public:

        static void Initialize();
        static VkInstance GetVkInstance() { return s_Instance; }

        static VulkanPhysicalDriver GetPhysicalDriver() { return s_PhysicalDriver; }

        static VulkanDriver GetDriver() { return s_Driver; }

    private:
        static std::vector<const char*> InitializeValidationLayers();

        static std::vector<const char*> InitializeInstanceExtensions();

    private:
        static VkInstance s_Instance;
        static VulkanPhysicalDriver s_PhysicalDriver;
        static VulkanDriver s_Driver;
    };
};