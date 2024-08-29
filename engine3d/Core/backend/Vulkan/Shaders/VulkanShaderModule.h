#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace engine3d{
    namespace vk{
        /**
         * @name VulkanShaderModule
         * @note To compile triangle.vert and triangle.frag do the following command
         * @note glslc.exe .\Resources\shaders\TriangleShader\triangle.vert -o triangle.vert.spirv
         * @note glslc.exe .\Resources\shaders\TriangleShader\triangle.frag -o triangle.frag.spirv
        */
        class VulkanShaderModule{
        public:
            VulkanShaderModule() = default;
            VulkanShaderModule(const std::string& filepath);
            VkShaderModule& GetVkShaderModuleInstance();

            bool IsLoaded();
        private:
            bool m_ShaderLoaded;
            VkShaderModule m_ShaderModule;
        };
    };
};