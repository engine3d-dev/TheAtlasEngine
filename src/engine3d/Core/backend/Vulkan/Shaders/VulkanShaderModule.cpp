#include "Core/EngineLogger.h"
#include "Core/backend/Vulkan/VulkanDevice.h"
#include "Core/backend/utilities/helper_functions.h"
// #include <Core/backend/Vulkan/VulkanShaderModule.h>
#include <Core/backend/Vulkan/Shaders/VulkanShaderModule.h>
#include <fstream>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{

    VulkanShaderModule::VulkanShaderModule(const std::string& filepath){
        // if(!Load(filepath)){
        //     ConsoleLogError("Could not load shader file {}", filepath);
        //     // return;
        // }

        // std::vector<uint32_t> buffer = Load(filepath);

        // if(buffer.empty()){
        //     ConsoleLogError("m_Buffer.empty() was actually indicating that the buffers empty!");
        //     return;
        // }
        std::ifstream ins(filepath, std::ios::ate | std::ios::binary);

        if(!ins.is_open()){
            ConsoleLogError("Could not load shader named {}", filepath);
            return;
        }

        //! @note Spirv expects buffer to be uint32, make sure reserve an int vector big enough for entire file.
        size_t filesize = (size_t)ins.tellg();
        std::vector<uint32_t> buffer(filesize / sizeof(uint32_t));

        // put file cursor at beginning
        ins.seekg(0);

        // load entire file into buffer
        ins.read((char*)buffer.data(), filesize);

        ins.close();

        if(buffer.empty()){
            ConsoleLogError("Buffer was empty!");
            return;
        }

        // Creating shader module
        VkShaderModuleCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = nullptr,
            // codesize has to be in bytes, multiple ints by buffer by size of the ints to know its actual size
            .codeSize = buffer.size() * sizeof(uint32_t),
            .pCode = buffer.data()
        };
        
        //! @note Then we actually create the shader module with VkShaderModule being our handler
        VkResult res = vkCreateShaderModule(VulkanDevice::GetVkLogicalDeviceInstance(), &createInfo, nullptr, &m_ShaderModule);

        if(res != VK_SUCCESS){
            ConsoleLogError("vkCreateShaderModule errored with message\t\t{}", VkResultToString(res));
        }

        ConsoleLogInfo("VulkanShaderModule created!");
    }

    VkShaderModule& VulkanShaderModule::GetVkShaderModuleInstance() { return m_ShaderModule; }

    // std::vector<uint32_t> VulkanShaderModule::Load(const std::string& filepath){
    //     std::fstream ins(filepath.c_str(), std::ios::ate | std::ios::binary);

    //     if(!ins){
    //         return {};
    //     }

    //     //! @note Spirv expects buffer to be uint32, make sure reserve an int vector big enough for entire file.
    //     size_t filesize = (size_t)ins.tellg();
    //     std::vector<uint32_t> buffer(filesize / sizeof(uint32_t));

    //     // put file cursor at beginning
    //     ins.seekg(0);

    //     // load entire file into buffer
    //     ins.read((char*)buffer.data(), filesize);

    //     ins.close();


    //     return buffer;
    // }
};