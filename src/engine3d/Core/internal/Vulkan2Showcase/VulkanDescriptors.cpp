#include <internal/Vulkan2Showcase/VulkanContext.hpp>
#include <internal/Vulkan2Showcase/helper_functions.hpp>
#include <internal/Vulkan2Showcase/VulkanDescriptors.hpp>
#include <Core/EngineLogger.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    // ************************************************************
    // *******************[Descriptor Set Layout]******************
    // ************************************************************
    VulkanDescriptorSetLayout::Builder& VulkanDescriptorSetLayout::Builder::AddBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count){
        if(m_Bindings.contains(binding)){
            ConsoleLogWarn("Binding already in use!");
        }

        VkDescriptorSetLayoutBinding layout_bind = {
            .binding = binding,
            .descriptorType = descriptorType,
            .descriptorCount = count,
            .stageFlags = stageFlags,
        };

        m_Bindings[binding] = layout_bind;
        return *this;
    }

    Scope<VulkanDescriptorSetLayout> VulkanDescriptorSetLayout::Builder::Build() const{
        return CreateScope<VulkanDescriptorSetLayout>(m_Bindings);
    }

    VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> p_Bindings) : m_Bindings(p_Bindings){
        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};

        for(auto[key, value] : m_Bindings){
            setLayoutBindings.push_back(value);
        }

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
            .bindingCount = static_cast<uint32_t>(setLayoutBindings.size()),
            .pBindings = setLayoutBindings.data()
        };

        vk_check(vkCreateDescriptorSetLayout(VulkanContext::GetDriver(), &descriptorSetLayoutInfo, nullptr, &m_DescriptorSetLayoutHandler), "vkCreateDescriptorSetLayout", __FILE__, __LINE__, __FUNCTION__);

    }


    // ************************************************************
    // *******************[Descriptor Set Pool]********************
    // ************************************************************

    VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::AddPoolSize(VkDescriptorType descriptorType, uint32_t count){
        m_PoolSizes.push_back({descriptorType, count});

        return *this;
    }

    VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::SetPoolFlags(VkDescriptorPoolCreateFlags flags){
        m_PoolFlags = flags;
        return *this;
    }

    VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::SetMaxSets(uint32_t count){
        m_MaxSets = count;
        return *this;
    }

    Scope<VulkanDescriptorPool> VulkanDescriptorPool::Builder::Build() const{
        return CreateScope<VulkanDescriptorPool>(m_MaxSets, m_PoolFlags, m_PoolSizes);
    }

    VulkanDescriptorPool::VulkanDescriptorPool(uint32_t p_MaxSets, VkDescriptorPoolCreateFlags p_PoolFlags, const std::vector<VkDescriptorPoolSize>& p_PoolSizes){
        VkDescriptorPoolCreateInfo desc_pool_ci = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .flags = p_PoolFlags,
            .maxSets = p_MaxSets,
            .poolSizeCount = static_cast<uint32_t>(p_PoolSizes.size()),
            .pPoolSizes = p_PoolSizes.data(),
        };

        vk_check(vkCreateDescriptorPool(VulkanContext::GetDriver(), &desc_pool_ci, nullptr, &m_DescriptorPoolHandler), "vkCreateDescriptorPool", __FILE__, __LINE__, __FUNCTION__);

    }

    bool VulkanDescriptorPool::AllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const{
        VkDescriptorSetAllocateInfo desc_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool = m_DescriptorPoolHandler,
            .descriptorSetCount = 1,
            .pSetLayouts = &descriptorSetLayout
        };
        VkResult res = vkAllocateDescriptorSets(VulkanContext::GetDriver(), &desc_alloc_info, &descriptor);
        vk_check(res, "vkAllocateDescriptorSets", __FILE__, __LINE__, __FUNCTION__);
        return (res == VK_SUCCESS) ? true : false;
    }

    void VulkanDescriptorPool::FreeDescriptor(std::vector<VkDescriptorSet>& p_Descriptors){
        vkFreeDescriptorSets(VulkanContext::GetDriver(), m_DescriptorPoolHandler, static_cast<uint32_t>(p_Descriptors.size()), p_Descriptors.data());
    }

    //! @note Resets descriptor pool
    void VulkanDescriptorPool::Reset(){
        vkResetDescriptorPool(VulkanContext::GetDriver(), m_DescriptorPoolHandler, 0);
    }

    // ************************************************************
    // ******************[Descriptor Set Writer]*******************
    // ************************************************************
    VulkanDescriptorWriter::VulkanDescriptorWriter(VulkanDescriptorSetLayout& setLayout, VulkanDescriptorPool& pool) : m_SetLayout(setLayout), m_DescriptorPool(pool){

    }

    VulkanDescriptorWriter& VulkanDescriptorWriter::WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo){
        if(!m_SetLayout.HasBinding(binding)){
            ConsoleLogWarn("Layout does not contain specific binding!");
        }

        auto& bind_desc = m_SetLayout.GetLayoutBinding(binding);
        if(bind_desc.descriptorCount == 1){
            ConsoleLogWarn("Binding single descriptor info but binding expects multiple!");
        }

        VkWriteDescriptorSet write = {
            .sType =  VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstBinding = binding,
            .descriptorCount = 1,
            .descriptorType = bind_desc.descriptorType,
            .pBufferInfo = bufferInfo,
        };

        m_Writes.push_back(write);
        return *this;
    }

    VulkanDescriptorWriter& VulkanDescriptorWriter::WriteBuffer(uint32_t binding, VkDescriptorImageInfo* imageInfo){
        if(!m_SetLayout.HasBinding(binding)){
            ConsoleLogWarn("Layout does not contain specific binding!");
        }

        auto& bind_desc = m_SetLayout.GetLayoutBinding(binding);
        if(bind_desc.descriptorCount == 1){
            ConsoleLogWarn("Binding single descriptor info but binding expects multiple!");
        }

        VkWriteDescriptorSet write = {
            .sType =  VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstBinding = binding,
            .descriptorCount = 1,
            .descriptorType = bind_desc.descriptorType,
            .pImageInfo = imageInfo,
        };

        m_Writes.push_back(write);
        return *this;
    }

    bool VulkanDescriptorWriter::Build(VkDescriptorSet& set){
        bool IsSuccessful = m_DescriptorPool.AllocateDescriptor(m_SetLayout.GetDescriptorSetLayout(), set);
        if(!IsSuccessful) return false;

        Overwrite(set);
        return true;
    }

    void VulkanDescriptorWriter::Overwrite(VkDescriptorSet& set){
        for(auto& write : m_Writes){
            write.dstSet = set;
        }

        vkUpdateDescriptorSets(VulkanContext::GetDriver(), m_Writes.size(), m_Writes.data(), 0, nullptr);
    }

};