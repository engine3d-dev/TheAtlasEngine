#pragma once
#include <cstdint>
#include <vulkan/vulkan_core.h>

namespace atlas{

    struct DescriptorSet{
        VkDescriptorSetLayoutBinding Binding;
        VkDescriptorSet Set;
    };

    /**
     * @name Uniform Buffer
     * @note Each uniform buffer will require you to specify requiremenst on the descriptor set usage
     * @note Per uniform buffer you must specify descriptor uses of those uniforms
     * @note Descriptor sets are how we group and these uniform data to the shader that then gets offloaded to the GPU
    */


    /*
        camera_ubo global_ubo = camera_ubo(...);

        // Structure that you see with the uniform buffer's constructor takes the following struct
        // Descriptor set configuration to set groups for you shader bindnig by update frequency
        struct UniformConfiguration{
            uint32_t Binding = -1;                              // specify where in the binding that we want to assign our descriptor set to
            ShaderStage Stage = ShaderStage::NOT_DEFINED;       // specify which shader stage to use this uniform buffers descriptor set
            bool HasSamples=false;                              // Checks if we want to use immutable samples
        };



        UniformBuffer ubo = UniformBuffer({
            {.binding = 0, .shaderStage = STAGE_TYPE_GRAPHICS, .hasSamples = true}
        });


        - These do the following for VkDescriptorSetlayoutCreateInfo
            - Sets binding count to the binding structure we specify
            - Set the data of that binding (setting up VkDescriptorSetLayoutBinding)
            - 
    
    
    */
    class UniformBuffer{
    public:
        UniformBuffer(uint32_t p_BindingIndex);
    };
};