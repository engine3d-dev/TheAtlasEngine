#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <string>

struct GLFWwindow;
namespace engine3d{

    namespace vk{
        /**
        * @name Vulkan.h
        * @note Uses for instantiating our vulkan API
        * @note These are properties that will be used throughout our vulkan's backend
        * @note Instantiating when we initiate the logical/physical devices.
        */
        class VulkanPipeline{
        public:
            void InitializePipeline();
            void CleanupPipeline();
            static GLFWwindow* GetCurrentWindow();
            static VkInstance& GetVkInstance();
            static VkSurfaceKHR& GetVkSurface();
            static std::string& GetApplicationTitle();
            static uint32_t GetWidth();
            static uint32_t GetHeight();


        private:
            //! @note Debug callback requires validatoin layers and debug utils to be enabled in extensions as it is not part of the vulkan core.
            void CreateDebugMessenger();

            //! @note TODO -- Probably want to abstract this and deal with window surfaces using Vulkan and the targeted platforms-specific windowing system
            void CreateSurface();

        };
    };
};