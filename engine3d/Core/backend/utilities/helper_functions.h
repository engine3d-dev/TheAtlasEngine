#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <string>
#include <vulkan/vulkan_core.h>

/**
 * @file helper_functions.h
 * @note Contains utility functions that will be used throughout engine3d.
*/
namespace engine3d{
    void CenterCurrentWindow();

    namespace vk{
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

    }; // end of vulkan namespace
};