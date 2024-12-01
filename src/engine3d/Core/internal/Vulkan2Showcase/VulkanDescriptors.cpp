#include <internal/Vulkan2Showcase/VulkanContext.hpp>
#include <internal/Vulkan2Showcase/helper_functions.hpp>
#include <internal/Vulkan2Showcase/VulkanDescriptors.hpp>
#include <Core/EngineLogger.hpp>
#include <vulkan/vulkan_core.h>

namespace engine3d::vk{
    /*
    // ************************************************************
    // *******************[Descriptor Set Layout]******************
    // ************************************************************
    
    VulkanDescriptorLayout::VulkanDescriptorLayout(std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> p_Bindings){
        std::vector<VkDescriptorSetLayoutBinding> set_layout_bindings{};

        for(auto[key, value] : p_Bindings){
            set_layout_bindings.emplace_back(value);
        }

        VkDescriptorSetLayoutCreateInfo desc_set_layout_create_info = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount = static_cast<uint32_t>(set_layout_bindings.size()),
            .pBindings = set_layout_bindings.data()
        };

        vk_check(vkCreateDescriptorSetLayout(VulkanContext::GetDriver(), &desc_set_layout_create_info, nullptr, &m_DescSetLayoutHandler), "vkCreateDescriptorSetLayout", __FILE__, __LINE__, __FUNCTION__);

    }

    void VulkanDescriptorLayout::AddBinding(
            uint32_t binding,
            VkDescriptorType p_DescType,
            VkShaderStageFlags p_StageFlags,
            uint32_t p_Count
        ){
        
        if(m_Bindings.contains(binding)){
            ConsoleLogWarn("Binding already in use!");
            assert(false);
        }

        VkDescriptorSetLayoutBinding layout_binding = {
            .binding = binding,
            .descriptorType = p_DescType,
            .descriptorCount = p_Count,
            .stageFlags = p_StageFlags
        };

        m_Bindings[binding] = layout_binding;
    }

    Ref<VulkanDescriptorLayout> VulkanDescriptorLayout::BuildLayout() const{
        return CreateRef<VulkanDescriptorLayout>(m_Bindings);
    }

    
    // ************************************************************
    // *******************[Descriptor Set Pool]********************
    // ************************************************************
    
    VulkanDescriptorPool::VulkanDescriptorPool(uint32_t p_MaxSets, VkDescriptorPoolCreateFlags p_PoolFlags, const std::vector<VkDescriptorPoolSize>& p_PoolSizes){
        VkDescriptorPoolCreateInfo desc_pool_ci = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = m_PoolFlags,
            .maxSets = m_MaxSets,
            .poolSizeCount = static_cast<uint32_t>(m_PoolSizes.size()),
            .pPoolSizes = m_PoolSizes.data(),
        };

        vk_check(vkCreateDescriptorPool(VulkanContext::GetDriver(), &desc_pool_ci, nullptr, &m_DescriptorPoolHandler), "vkCreateDescriptorPool", __FILE__, __LINE__, __FUNCTION__);
    }

    bool VulkanDescriptorPool::AllocateDescriptors(const VkDescriptorSetLayout p_DescSetLayout, VkDescriptorSet& p_Descriptor) const {
        VkDescriptorSetAllocateInfo desc_set_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = nullptr,
            .descriptorPool = m_DescriptorPoolHandler,
            .descriptorSetCount = 1,
            .pSetLayouts = &p_DescSetLayout
        };
        VkResult res = vkAllocateDescriptorSets(VulkanContext::GetDriver(), &desc_set_alloc_info, &p_Descriptor);
        vk_check(res, "vkAllocateDescriptorSets", __FILE__, __LINE__, __FUNCTION__);

        return (res == VK_SUCCESS) ? true : false;
    }

    void VulkanDescriptorPool::FreeDescriptors(const std::vector<VkDescriptorSet>& p_Descriptors){
        vkFreeDescriptorSets(VulkanContext::GetDriver(), m_DescriptorPoolHandler, static_cast<uint32_t>(p_Descriptors.size()), p_Descriptors.data());
    }

    //! @note Resets this entire pool
    void VulkanDescriptorPool::Reset(){
        vkResetDescriptorPool(VulkanContext::GetDriver(), m_DescriptorPoolHandler, 0);
    }

    //! @note In the tutorial these three functions are in the builder struct.
    void VulkanDescriptorPool::AddPoolSize(VkDescriptorType p_DescType, uint32_t p_Count){
        m_PoolSizes.push_back({p_DescType, p_Count});
    }

    void VulkanDescriptorPool::SetPoolFlags(VkDescriptorPoolCreateFlags p_Flags){
        m_PoolFlags = p_Flags;
    }

    void VulkanDescriptorPool::SetMaxSets(uint32_t p_Count){
        m_MaxSets = p_Count;
    }

    Ref<VulkanDescriptorPool> VulkanDescriptorPool::BuildDescriptorPool() const{
        return CreateRef<VulkanDescriptorPool>(m_MaxSets, m_PoolFlags, m_PoolSizes);
    }

    
    // ************************************************************
    // ******************[Descriptor Set Writer]*******************
    // ************************************************************
    
   VulkanDescriptorWriter::VulkanDescriptorWriter(VulkanDescriptorLayout& p_SetLayout, VulkanDescriptorPool& p_DescriptorPool) : m_SetLayout(p_SetLayout), m_DescriptorPool(p_DescriptorPool){
   }

    bool VulkanDescriptorWriter::Build(VkDescriptorSet& p_Set){
        bool isSuccessful = m_DescriptorPool.AllocateDescriptors(m_SetLayout.GetDescriptorLayout(), p_Set);
        if(!isSuccessful){
            return false;
        }

        Overwrite(p_Set);
        return true;
    }

    VulkanDescriptorWriter VulkanDescriptorWriter::WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* p_BufInfo){
        if(!m_SetLayout.HasBinding(binding)){
            ConsoleLogWarn("Layout does not contain specified binding!");
            assert(false);
        }

        auto bind_desc = m_SetLayout.GetBinding(binding);
        if(bind_desc.descriptorCount == 1){
            ConsoleLogWarn("Binding single descriptor info, but binding expected multiple!");
            assert(false);
        }

        VkWriteDescriptorSet write_desc_set{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstBinding = binding,
            .descriptorCount = 1,
            .descriptorType = bind_desc.descriptorType,
            .pBufferInfo = p_BufInfo,
        };

        m_Writes.push_back(write_desc_set);

        return *this;
    }

    VulkanDescriptorWriter VulkanDescriptorWriter::WriteImage(uint32_t binding, VkDescriptorImageInfo* p_ImageInfo){
        if(!m_SetLayout.HasBinding(binding)){
            ConsoleLogWarn("Layout does not contain specified binding!");
            assert(false);
        }

        auto bind_desc = m_SetLayout.GetBinding(binding);
        if(bind_desc.descriptorCount == 1){
            ConsoleLogWarn("Binding single descriptor info, but binding expected multiple!");
            assert(false);
        }

        VkWriteDescriptorSet write_desc_set{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstBinding = binding,
            .descriptorCount = 1,
            .descriptorType = bind_desc.descriptorType,
            .pImageInfo = p_ImageInfo
        };

        m_Writes.push_back(write_desc_set);

        return *this;
    }

    void VulkanDescriptorWriter::Overwrite(VkDescriptorSet& p_Set){
        for(auto& write : m_Writes){
            write.dstSet = p_Set;
        }

        vkUpdateDescriptorSets(  VulkanContext::GetDriver(), m_Writes.size(), m_Writes.data(), 0, nullptr);
    }

    */

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