#pragma once
#include "core.hpp"
#include <unordered_map>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace atlas::vk{
    class descriptor_set_layout {
    public:
        class builder {
        public:
            builder() =default;
        
            builder &addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<descriptor_set_layout> build() const;
        
        private:
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };
    
        descriptor_set_layout(std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~descriptor_set_layout();
        descriptor_set_layout(const descriptor_set_layout &) = delete;
        descriptor_set_layout &operator=(const descriptor_set_layout &) = delete;
        
        VkDescriptorSetLayout get_descriptor_set_layout() const { return m_descriptor_set_layout; }

        VkDescriptorSetLayoutBinding& Get(uint32_t bind){ return bindings.at(bind); }
    
    private:
        VkDescriptorSetLayout m_descriptor_set_layout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;
    };
    
    class descriptor_pool {
    public:
        class builder {
        public:
            builder() = default;
        
            builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
            builder &setMaxSets(uint32_t count);
            std::unique_ptr<descriptor_pool> build() const;
        
        private:
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };
        
        descriptor_pool(
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize> &poolSizes);
        ~descriptor_pool();
        descriptor_pool(const descriptor_pool &) = delete;
        descriptor_pool &operator=(const descriptor_pool &) = delete;
        
        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptor_set_layout, VkDescriptorSet &descriptor) const;
        
        void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;
        
        void resetPool();
    
    private:
        VkDescriptorPool m_descriptor_pool;
    };
    
    class descriptor_writer {
    public:
        descriptor_writer(descriptor_set_layout &setLayout, descriptor_pool &pool);
        
        descriptor_writer &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
        descriptor_writer &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);
        
        bool build(VkDescriptorSet &set);
        void overwrite(VkDescriptorSet &set);
    
    private:
        descriptor_set_layout &setLayout;
        descriptor_pool &pool;
        std::vector<VkWriteDescriptorSet> writes;
    };
};