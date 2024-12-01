#pragma once
#include <Core/Core.hpp>
#include <unordered_map>
#include <vulkan/vulkan_core.h>
#include <vector>

namespace engine3d::vk{

    /*
    class VulkanDescriptorLayout{
    public:
        VulkanDescriptorLayout() = default;
        VulkanDescriptorLayout(std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> p_Bindings);


        //! @note Map to bindings vulkan requires.
        //! @note Such as what type of descriptoy type to expect.
        
        //  * @param binding
        //  * @param p_DescType
        //  * @param p_StageFlags tell this descriptor layout which shader stages have access to this specific binding
        //  * @param p_Count
        
        void AddBinding(
            uint32_t binding,
            VkDescriptorType p_DescType,
            VkShaderStageFlags p_StageFlags,
            uint32_t p_Count = 1
        );

        Ref<VulkanDescriptorLayout> BuildLayout() const;

        bool HasBinding(uint32_t p_Binding){
            return m_Bindings.contains(p_Binding);
        }

        VkDescriptorSetLayoutBinding GetBinding(uint32_t key){
            return m_Bindings[key];
        }

        VkDescriptorSetLayout GetDescriptorLayout() const { return m_DescSetLayoutHandler; }

    private:
        VkDescriptorSetLayout m_DescSetLayoutHandler;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings{};
    };

    class VulkanDescriptorPool{
    public:
        VulkanDescriptorPool() = default;
        VulkanDescriptorPool(uint32_t p_MaxSets, VkDescriptorPoolCreateFlags p_PoolFlags, const std::vector<VkDescriptorPoolSize>& p_PoolSizes);

        bool AllocateDescriptors(const VkDescriptorSetLayout p_DescSetLayout, VkDescriptorSet& p_Descriptor) const;
        void FreeDescriptors(const std::vector<VkDescriptorSet>& p_Descriptors);

        //! @note Resets this pool
        void Reset();

        //! @note In the tutorial these three functions are in the builder struct.
        void AddPoolSize(VkDescriptorType p_DescType, uint32_t p_Count);
        void SetPoolFlags(VkDescriptorPoolCreateFlags p_Flags);
        void SetMaxSets(uint32_t p_Count);
        Ref<VulkanDescriptorPool> BuildDescriptorPool() const;

    private:
        // Descriptor Pool Builder-specific
        //! @note in the tutorial these three variables are in the builder struct
        std::vector<VkDescriptorPoolSize> m_PoolSizes{};
        uint32_t m_MaxSets = 1000;
        VkDescriptorPoolCreateFlags m_PoolFlags = 0;

        VkDescriptorPool m_DescriptorPoolHandler;
    };

    class VulkanDescriptorWriter{
    public:
        VulkanDescriptorWriter(VulkanDescriptorLayout& p_SetLayout, VulkanDescriptorPool& p_DescriptorPool);

        VulkanDescriptorWriter WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* p_BufInfo);
        VulkanDescriptorWriter WriteImage(uint32_t binding, VkDescriptorImageInfo* p_ImageInfo);

        bool Build(VkDescriptorSet& p_Set);
        void Overwrite(VkDescriptorSet& p_Set);

    private:
        VulkanDescriptorLayout m_SetLayout;
        VulkanDescriptorPool m_DescriptorPool;
        std::vector<VkWriteDescriptorSet> m_Writes;
    };
    */

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
};