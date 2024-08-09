#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <string>
#include <vulkan/vulkan_core.h>
#include <vector>

/**
 * @file helper_functions.h
 * @note Contains utility functions that will be used throughout engine3d.
*/
namespace engine3d{
    void CenterCurrentWindow();

    namespace vk{
        //! @note We go through our selected surface formats
        //! @note Checking for both the srgb formats and srgb non linear color space.
        //! @note If these aren't found in our selected surface formats and colorspaces then we use the default in our surface formats.
        VkSurfaceFormatKHR SelectSurfaceFormatAndColorspace(const std::vector<VkSurfaceFormatKHR>& surfaceFormats);
        std::string VkResultToString(VkResult res);
        std::string GetDebugSeverity(VkDebugUtilsMessageSeverityFlagBitsEXT Severity);
        
        std::string GetDebugType(VkDebugUtilsMessageTypeFlagsEXT Type);

        //! @note Checked to see what image usage flag has been enabled
        std::string VkImageUsageFlagsToString(const VkImageUsageFlags& flags);

        //! @note Used for checking what presentation mode that is being in use
        std::string VkPresentModeToString(const VkPresentModeKHR& modes);

        std::string VkFormatToString(const VkFormat& format);

        std::string VkColorspaceToString(const VkColorSpaceKHR& colorspace);

        std::string VkMemoryPropertyFlagToString(const VkMemoryPropertyFlags& flags);

        void Begin(VkCommandBuffer commandBuffer, VkCommandBufferUsageFlags usageFlags);

        void End(VkCommandBuffer buffer);

    }; // end of vulkan namespace
};