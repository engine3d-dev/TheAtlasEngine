#include "drivers/vulkan/vulkan_context.hpp"
#include "engine_logger.hpp"
#include <drivers/vulkan/descriptor_tutorial.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <vulkan/vulkan_core.h>

namespace atlas::vk {
    /*
    DescriptorLayout::builder DescriptorLayout::builder::AddBinding(
                uint32_t binding,
                VkDescriptorType DescriptorType,
                VkShaderStageFlags StageFlags,
                uint32_t Count){
        VkDescriptorSetLayoutBinding layout_binding = {
            .binding = binding,
            .descriptorType = DescriptorType,
            .descriptorCount = Count,
            .stageFlags = StageFlags
        };
        bindings[binding] = layout_binding;
        return *this;
    }

    ref<DescriptorLayout> DescriptorLayout::builder::Build() const{
        return create_ref<DescriptorLayout>(bindings);
    }

    DescriptorLayout::DescriptorLayout(std::unordered_map<uint32_t,
    VkDescriptorSetLayoutBinding> bindings){
        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
        for(auto kv : bindings){
            setLayoutBindings.push_back(kv.second);
        }

        VkDescriptorSetLayoutCreateInfo ci = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .bindingCount = static_cast<uint32_t>(setLayoutBindings.size()),
            .pBindings = setLayoutBindings.data()
        };

        vk_check(vkCreateDescriptorSetLayout(vk_context::get_current_driver(),
    &ci, nullptr, &descriptor_set_layout), "vkCreateDescriptorSetLayout",
    __FILE__, __LINE__, __FUNCTION__);


    }

    DescriptorLayout::~DescriptorLayout(){}


    descriptor_pool::builder&
    descriptor_pool::builder::addPoolSize(VkDescriptorType Type, uint32_t
    Count){ m_PoolSizes.push_back({Type, Count}); return *this;
    }

    descriptor_pool::builder&
    descriptor_pool::builder::setPoolFlags(VkDescriptorPoolCreateFlags Flags){
        m_PoolFlags = Flags;
        return *this;
    }

    descriptor_pool::builder& descriptor_pool::builder::setMaxSets(uint32_t
    Count){ MaxSets = Count;

        return *this;
    }

    ref<descriptor_pool> descriptor_pool::builder::Build() const {
        return create_ref<descriptor_pool>(MaxSets, m_PoolFlags, m_PoolSizes);
    }



    descriptor_pool::descriptor_pool(uint32_t MaxSets,
    VkDescriptorPoolCreateFlags PoolFlags, const
    std::vector<VkDescriptorPoolSize>& PoolSizes){ VkDescriptorPoolCreateInfo
    pool_ci = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO, .pNext =
    nullptr, .flags = PoolFlags, .maxSets = MaxSets, .poolSizeCount =
    static_cast<uint32_t>(PoolSizes.size()),
        };

        vk_check(vkCreateDescriptorPool(vk_context::get_current_driver(),
    &pool_ci, nullptr, &m_DescriptorPool), "vkCreateDescriptorPool", __FILE__,
    __LINE__, __FUNCTION__);


    }

    bool descriptor_pool::AllocvateDescriptor(const VkDescriptorSetLayout
    descriptor_set_layout, VkDescriptorSet& Descriptor) const {
        VkDescriptorSetAllocateInfo allocInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = nullptr,
            .descriptor_pool = m_DescriptorPool,
            .descriptorSetCount = 1,
            .pSetLayouts = &descriptor_set_layout,
        };

        vk_check(vkAllocateDescriptorSets(vk_context::get_current_driver(),
    &allocInfo, &Descriptor), "vkAllocateDescriptorSets", __FILE__, __LINE__,
    __FUNCTION__);

        return true;
    }

    void descriptor_pool::FreeDescriptors(std::vector<VkDescriptorSet>&
    Descriptors) const{ vkFreeDescriptorSets(vk_context::get_current_driver(),
    m_DescriptorPool, static_cast<uint32_t>(Descriptors.size()),
    Descriptors.data());
    }

    void descriptor_pool::Reset(){
        vkResetDescriptorPool(vk_context::get_current_driver(),
    m_DescriptorPool, 0);
    }


    descriptor_writer::descriptor_writer(DescriptorLayout& setLayout,
    descriptor_pool& Pool) : m_Pool{Pool}, m_DescSetLayout{setLayout}{}

    descriptor_writer& descriptor_writer::writeBuffer(uint32_t binding,
    VkDescriptorBufferInfo* bufferInfo){ auto& bindDescription =
    m_DescSetLayout.Get(binding);

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.descriptorType = bindDescription.descriptorType;
        write.dstBinding = binding;
        write.pBufferInfo = bufferInfo;
        write.descriptorCount = 1;

        m_Writes.push_back(write);
        return *this;
    }

    descriptor_writer& descriptor_writer::writeImage(uint32_t binding,
    VkDescriptorImageInfo* imageInfo){ auto& bindDescription =
    m_DescSetLayout.Get(binding); VkWriteDescriptorSet write{}; write.sType =
    VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET; write.descriptorType =
    bindDescription.descriptorType; write.dstBinding = binding; write.pImageInfo
    = imageInfo; write.descriptorCount = 1;

        m_Writes.push_back(write);
        return *this;
    }

    bool descriptor_writer::Build(VkDescriptorSet& Set){
        console_log_trace("Before Part 3.1");
        bool success =
    m_Pool.AllocvateDescriptor(m_DescSetLayout.get_descriptor_set_layout(),
    Set);

        if(!success){
            return false;
        }

        console_log_trace("Before Part #4");

        Overwrite(Set);

        console_log_trace("Before Part #5");
        return true;
    }

    void descriptor_writer::Overwrite(VkDescriptorSet& Set){
        for (auto &write : m_Writes) {
            write.dstSet = Set;
        }
        vkUpdateDescriptorSets(vk_context::get_current_driver(),
    m_Writes.size(), m_Writes.data(), 0, nullptr);
    }

    */
    // *************** Descriptor Set Layout builder *********************

    descriptor_set_layout::builder& descriptor_set_layout::builder::addBinding(
      uint32_t binding,
      VkDescriptorType descriptorType,
      VkShaderStageFlags stageFlags,
      uint32_t count) {
        assert(bindings.count(binding) == 0 && "Binding already in use");
        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = binding;
        layoutBinding.descriptorType = descriptorType;
        layoutBinding.descriptorCount = count;
        layoutBinding.stageFlags = stageFlags;
        bindings[binding] = layoutBinding;
        return *this;
    }

    std::unique_ptr<descriptor_set_layout>
    descriptor_set_layout::builder::build() const {
        return std::make_unique<descriptor_set_layout>(bindings);
    }

    // *************** Descriptor Set Layout *********************

    descriptor_set_layout::descriptor_set_layout(
      std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
      : bindings{ bindings } {
        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
        for (auto kv : bindings) {
            setLayoutBindings.push_back(kv.second);
        }

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
        descriptorSetLayoutInfo.sType =
          VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutInfo.bindingCount =
          static_cast<uint32_t>(setLayoutBindings.size());
        descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

        if (vkCreateDescriptorSetLayout(vk_context::get_current_driver(),
                                        &descriptorSetLayoutInfo,
                                        nullptr,
                                        &m_descriptor_set_layout) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }

    descriptor_set_layout::~descriptor_set_layout() {
        vkDestroyDescriptorSetLayout(
          vk_context::get_current_driver(), m_descriptor_set_layout, nullptr);
    }

    // *************** Descriptor Pool builder *********************

    descriptor_pool::builder& descriptor_pool::builder::addPoolSize(
      VkDescriptorType descriptorType,
      uint32_t count) {
        poolSizes.push_back({ descriptorType, count });
        return *this;
    }

    descriptor_pool::builder& descriptor_pool::builder::setPoolFlags(
      VkDescriptorPoolCreateFlags flags) {
        poolFlags = flags;
        return *this;
    }
    descriptor_pool::builder& descriptor_pool::builder::setMaxSets(
      uint32_t count) {
        maxSets = count;
        return *this;
    }

    std::unique_ptr<descriptor_pool> descriptor_pool::builder::build() const {
        return std::make_unique<descriptor_pool>(maxSets, poolFlags, poolSizes);
    }

    // *************** Descriptor Pool *********************

    descriptor_pool::descriptor_pool(
      uint32_t maxSets,
      VkDescriptorPoolCreateFlags poolFlags,
      const std::vector<VkDescriptorPoolSize>& poolSizes) {
        VkDescriptorPoolCreateInfo descriptorPoolInfo{};
        descriptorPoolInfo.sType =
          VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.poolSizeCount =
          static_cast<uint32_t>(poolSizes.size());
        descriptorPoolInfo.pPoolSizes = poolSizes.data();
        descriptorPoolInfo.maxSets = maxSets;
        descriptorPoolInfo.flags = poolFlags;

        if (vkCreateDescriptorPool(vk_context::get_current_driver(),
                                   &descriptorPoolInfo,
                                   nullptr,
                                   &m_descriptor_pool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    descriptor_pool::~descriptor_pool() {
        vkDestroyDescriptorPool(
          vk_context::get_current_driver(), m_descriptor_pool, nullptr);
    }

    bool descriptor_pool::allocateDescriptor(
      const VkDescriptorSetLayout descriptor_set_layout,
      VkDescriptorSet& descriptor) const {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_descriptor_pool;
        allocInfo.pSetLayouts = &descriptor_set_layout;
        allocInfo.descriptorSetCount = 1;

        // Might want to create a "DescriptorPoolManager" class that handles
        // this case, and builds a new pool whenever an old pool fills up. But
        // this is beyond our current scope
        if (vkAllocateDescriptorSets(vk_context::get_current_driver(),
                                     &allocInfo,
                                     &descriptor) != VK_SUCCESS) {
            return false;
        }
        return true;
    }

    void descriptor_pool::freeDescriptors(
      std::vector<VkDescriptorSet>& descriptors) const {
        vkFreeDescriptorSets(vk_context::get_current_driver(),
                             m_descriptor_pool,
                             static_cast<uint32_t>(descriptors.size()),
                             descriptors.data());
    }

    void descriptor_pool::resetPool() {
        vkResetDescriptorPool(
          vk_context::get_current_driver(), m_descriptor_pool, 0);
    }

    // *************** Descriptor Writer *********************

    descriptor_writer::descriptor_writer(descriptor_set_layout& setLayout,
                                         descriptor_pool& pool)
      : setLayout{ setLayout }
      , pool{ pool } {}

    descriptor_writer& descriptor_writer::writeBuffer(
      uint32_t binding,
      VkDescriptorBufferInfo* bufferInfo) {
        assert(setLayout.bindings.count(binding) == 1 &&
               "Layout does not contain specified binding");

        auto& bindingDescription = setLayout.Get(binding);

        assert(bindingDescription.descriptorCount == 1 &&
               "Binding single descriptor info, but binding expects multiple");

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.descriptorType = bindingDescription.descriptorType;
        write.dstBinding = binding;
        write.pBufferInfo = bufferInfo;
        write.descriptorCount = 1;

        writes.push_back(write);
        return *this;
    }

    descriptor_writer& descriptor_writer::writeImage(
      uint32_t binding,
      VkDescriptorImageInfo* imageInfo) {
        assert(setLayout.bindings.count(binding) == 1 &&
               "Layout does not contain specified binding");

        auto& bindingDescription = setLayout.Get(binding);

        assert(bindingDescription.descriptorCount == 1 &&
               "Binding single descriptor info, but binding expects multiple");

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.descriptorType = bindingDescription.descriptorType;
        write.dstBinding = binding;
        write.pImageInfo = imageInfo;
        write.descriptorCount = 1;

        writes.push_back(write);
        return *this;
    }

    bool descriptor_writer::build(VkDescriptorSet& set) {
        bool success =
          pool.allocateDescriptor(setLayout.get_descriptor_set_layout(), set);
        if (!success) {
            return false;
        }
        overwrite(set);
        return true;
    }

    void descriptor_writer::overwrite(VkDescriptorSet& set) {
        for (auto& write : writes) {
            write.dstSet = set;
        }
        vkUpdateDescriptorSets(vk_context::get_current_driver(),
                               writes.size(),
                               writes.data(),
                               0,
                               nullptr);
    }
};