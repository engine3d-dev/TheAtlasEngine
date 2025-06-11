#include <drivers/vulkan-cpp/vk_descriptor_set.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <core/engine_logger.hpp>
#include <array>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>

namespace atlas::vk {
    VkDescriptorType to_descriptor_set_type(const descriptor_type& p_type) {
        switch (p_type) {
            case descriptor_type::StorageBuffer:
                return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            case descriptor_type::UniformBuffer:
                return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            case descriptor_type::ImageAndSampler:
                return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        }

        console_log_error("descriptor set type specified is invalid!!!");
    }

    VkShaderStageFlags to_vk_shader_stage(const shader_stage& p_stage) {
        switch (p_stage) {
            case shader_stage::Vertex:
                return VK_SHADER_STAGE_VERTEX_BIT;
            case shader_stage::Fragment:
                return VK_SHADER_STAGE_FRAGMENT_BIT;
            default: return 0;
        }

        console_log_error(
          "vulkan shader stage that you specified was invalid!!!");
    }


    descriptor_set::descriptor_set(const descriptor_set_layout& p_layout) : m_allocated_descriptors(p_layout.allocate_count), m_size_bytes(p_layout.size_bytes) {
        m_driver = vk_context::driver_context();

        // std::array<VkDescriptorPoolSize, 2> pool_sizes;
        // pool_sizes[0] = {
        //     .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        //     .descriptorCount = static_cast<uint32_t>(m_descriptor_count),
        // };

        // pool_sizes[1] = {
        //     .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        //     .descriptorCount = static_cast<uint32_t>(m_descriptor_count),
        // };

        // 1. Setting up descriptor pool
        VkDescriptorPoolCreateInfo desc_pool_ci = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .maxSets = p_layout.max_sets,
            .poolSizeCount = static_cast<uint32_t>(p_layout.allocation_info.size()),
            .pPoolSizes = p_layout.allocation_info.data()
        };

        vk_check(vkCreateDescriptorPool(
                   m_driver, &desc_pool_ci, nullptr, &m_descriptor_pool),
                 "vkCreateDescriptorPool",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        std::vector<VkDescriptorSetLayoutBinding> layout_bindings(p_layout.bindings.begin(), p_layout.bindings.end());
        VkDescriptorSetLayoutCreateInfo descriptor_set_layout_ci = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .bindingCount = static_cast<uint32_t>(layout_bindings.size()),
            .pBindings = layout_bindings.data()
        };

        vk_check(vkCreateDescriptorSetLayout(m_driver,
                                             &descriptor_set_layout_ci,
                                             nullptr,
                                             &m_descriptor_set_layout),
                 "vkCreateDescriptorSetLayout",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);


        std::vector<VkDescriptorSetLayout> layouts(m_allocated_descriptors,
                                                   m_descriptor_set_layout);

        VkDescriptorSetAllocateInfo descriptor_set_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = nullptr,
            .descriptorPool = m_descriptor_pool,
            .descriptorSetCount = m_allocated_descriptors,
            .pSetLayouts = layouts.data()
        };

        m_descriptor_sets.resize(m_allocated_descriptors);

        vk_check(vkAllocateDescriptorSets(m_driver,
                                          &descriptor_set_alloc_info,
                                          m_descriptor_sets.data()),
                 "vkAllocateDescriptorSets",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);
    }

    void descriptor_set::update_test_descriptors(const std::span<vk_uniform_buffer>& p_uniforms, const texture& p_texture) {
        for(size_t i = 0; i < m_allocated_descriptors; i++) {
            VkDescriptorBufferInfo buffer_info = {
                .buffer = p_uniforms[i],
                .offset = 0,
                .range = m_size_bytes, // m_size_bytes replaces doing things like: sizeof(camera_ubo)
            };

            VkDescriptorImageInfo image_info = {
                .sampler = p_texture.sampler(),
                .imageView = p_texture.image_view(),
                .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            };

            // std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
            std::array<VkWriteDescriptorSet, 2> write_descriptors;
            write_descriptors[0] = {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = nullptr,
                .dstSet = m_descriptor_sets[i],
                .dstBinding = 0,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .pBufferInfo = &buffer_info,
            };
            write_descriptors[1] = {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = nullptr,
                .dstSet = m_descriptor_sets[i],
                .dstBinding = 1,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .pImageInfo = &image_info,
            };
            vkUpdateDescriptorSets(m_driver, static_cast<uint32_t>(write_descriptors.size()), write_descriptors.data(), 0, nullptr); 
        }
    }

    void descriptor_set::bind(const VkCommandBuffer& p_current,
                              uint32_t p_frame_index,
                              const VkPipelineLayout& p_pipeline_layout) {
        if (m_descriptor_sets.size() > 0) {
            vkCmdBindDescriptorSets(p_current,
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    p_pipeline_layout,
                                    0,
                                    1,
                                    &m_descriptor_sets[p_frame_index],
                                    0,
                                    nullptr);
        }
    }

    void descriptor_set::destroy() {
        vkDestroyDescriptorPool(m_driver, m_descriptor_pool, nullptr);
        vkDestroyDescriptorSetLayout(
          m_driver, m_descriptor_set_layout, nullptr);
    }
};