#pragma once
#include <Core/Core.hpp>
#include <unordered_map>
#include <vulkan/vulkan_core.h>
#include <vector>

namespace engine3d::vk{

    class VulkanDescriptorSetLayout{
    public:
        class Builder{
        public:
            Builder& AddBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count=1);

            Scope<VulkanDescriptorSetLayout> Build() const;

            bool HasBinding(uint32_t binding){
                return m_Bindings.contains(binding);
            }

        private:
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings{};
        };

        VulkanDescriptorSetLayout() = default;

        VulkanDescriptorSetLayout(std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> p_Bindings);
        VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_DescriptorSetLayoutHandler; }

        bool HasBinding(uint32_t binding){
            return m_Bindings.contains(binding);
        }

        VkDescriptorSetLayoutBinding& GetLayoutBinding(uint32_t binding){
            return m_Bindings[binding];
        }


    private:
        VkDescriptorSetLayout m_DescriptorSetLayoutHandler;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;
    };

    class VulkanDescriptorPool{
    public:
        class Builder{
        public:
            Builder& AddPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& SetPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& SetMaxSets(uint32_t count);

            Scope<VulkanDescriptorPool> Build() const;

        private:
            std::vector<VkDescriptorPoolSize> m_PoolSizes;
            uint32_t m_MaxSets = 1000;
            VkDescriptorPoolCreateFlags m_PoolFlags = 0;
        };

        VulkanDescriptorPool(uint32_t p_MaxSets, VkDescriptorPoolCreateFlags p_PoolFlags, const std::vector<VkDescriptorPoolSize>& p_PoolSizes);

        bool AllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void FreeDescriptor(std::vector<VkDescriptorSet>& p_Descriptors);

        //! @note Resets descriptor pool
        void Reset();

    private:
        VkDescriptorPool m_DescriptorPoolHandler;
    };

    class VulkanDescriptorWriter{
    public:
        VulkanDescriptorWriter(VulkanDescriptorSetLayout& setLayout, VulkanDescriptorPool& pool);

        VulkanDescriptorWriter& WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        VulkanDescriptorWriter& WriteBuffer(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool Build(VkDescriptorSet& set);
        void Overwrite(VkDescriptorSet& set);

    private:
        VulkanDescriptorSetLayout m_SetLayout;
        VulkanDescriptorPool m_DescriptorPool;
        std::vector<VkWriteDescriptorSet> m_Writes;
    };

    /*

        // Setting descriptor data for the descriptor pool
        DescriptorData{
            .binding = 0,
            .DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .DescriptorCount = VK_SHADER_STAGE_ALL_GRAPHICS
        };
    
    */
    struct DescriptorData{
        uint32_t Binding = -1;
        VkDescriptorType DescriptorType;
        VkShaderStageFlags Flags;
        uint32_t DescriptorCount = -1;
    };

    class VulkanDescriptors{
    public:
        VulkanDescriptors() = default;
        void InitDescriptorPool();

        void InitDescriptorSets();


    private:
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;
        std::vector<VkDescriptorSet> m_DescriptorSets;
        VkDescriptorSetLayout m_DescriptorSetLayout;
        VkDescriptorPool m_DescriptorPool;
        VkWriteDescriptorSet m_WriteDescriptorSet;
        VkDescriptorSet m_DescriptorSet;
    };
};