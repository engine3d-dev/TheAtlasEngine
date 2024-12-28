#pragma once
#include <drivers/vulkan/vulkan_physical_driver.hpp>
#include <vulkan/vulkan.hpp>

namespace engine3d::vk{
    class VulkanDriver{
    public:
        VulkanDriver() = default;
        VulkanDriver(VulkanPhysicalDriver p_PhysicalDevice);

        VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }

        operator VkDevice(){
            return m_CurrentDriver;
        }

        //! @note Returns -1 if there are no flags available/compatible/valid
        uint32_t SelectMemoryType(uint32_t p_TypeFilter, VkMemoryPropertyFlags p_PropertyFlag);

    private:
        VkDevice m_CurrentDriver;
        VkQueue m_GraphicsQueue;
    };
};