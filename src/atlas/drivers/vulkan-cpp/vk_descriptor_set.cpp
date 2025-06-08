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

    descriptor_set::descriptor_set(
      uint32_t p_count,
      const std::initializer_list<VkDescriptorSetLayoutBinding>& p_list) : m_descriptor_count(p_count) {
        m_driver = vk_context::driver_context();

        /*
            TODO: Make this be a parameter for specifying for this descriptor
           sets what buffer types to allocate and the size

            These are examples of the new changes of how setting up the
           descriptor sets are going to work

            New API (once this works)
            std::array<pool_info, 3> allocation_info = {
                {vk::uniform_buffer, descriptor_count},
                {vk::storage_buffer, descriptor_count}
            };

            // Currently how the API looks
            std::array<VkDescriptorSetLayoutBinding, 2> layout_bindings = {
                {.binding = 0, .descriptorType =
           VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = 1, .stageFlags
           = VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT,
           .pImmutableSamplers  = nullptr},
                {.binding = 1, .descriptorType =
           VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = 1,
           .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT, .pImmutableSamplers  =
           nullptr},
            };

            // TODO: Will add a new high-level invokation for the API like:
            std::array<descriptor_layout_binding, 2> new_layout_bindings = {
                { .binding = 0, .type = vk::uniform_buffer, .count=1, .stage =
           shader_type::fragment | shader_type::vertex },
                {.binding = 1, .type = vk::image_sampler, .count=1, .stage =
           shader_type::fragment }
            };

            vk::descriptor_set example_of_new_descriptor_set =
           vk::descriptor_set(allocation_info, layout_bindings);
        */
        std::array<VkDescriptorPoolSize, 2> pool_sizes;
        pool_sizes[0] = {
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = static_cast<uint32_t>(m_descriptor_count),
        };

        pool_sizes[1] = {
            .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = static_cast<uint32_t>(m_descriptor_count),
        };

        // 1. Setting up descriptor pool
        VkDescriptorPoolCreateInfo desc_pool_ci = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .maxSets = m_descriptor_count,
            .poolSizeCount = static_cast<uint32_t>(pool_sizes.size()),
            .pPoolSizes = pool_sizes.data()
        };

        vk_check(vkCreateDescriptorPool(
                   m_driver, &desc_pool_ci, nullptr, &m_descriptor_pool),
                 "vkCreateDescriptorPool",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        console_log_info("descriptor pool created!!!");

        std::vector<VkDescriptorSetLayoutBinding> layout_bindings(p_list);
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

        console_log_info("descriptor layout created!!!");

        std::vector<VkDescriptorSetLayout> layouts(m_descriptor_count,
                                                   m_descriptor_set_layout);

        VkDescriptorSetAllocateInfo descriptor_set_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = nullptr,
            .descriptorPool = m_descriptor_pool,
            .descriptorSetCount = m_descriptor_count,
            .pSetLayouts = layouts.data()
        };

        m_descriptor_sets.resize(m_descriptor_count);

        vk_check(vkAllocateDescriptorSets(m_driver,
                                          &descriptor_set_alloc_info,
                                          m_descriptor_sets.data()),
                 "vkAllocateDescriptorSets",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        console_log_info("descriptor sets allocated!!!");
    }

    descriptor_set::descriptor_set(
      const std::span<VkDescriptorSetLayoutBinding>& p_descriptors) {
        m_driver = vk_context::driver_context();

        /*
            TODO: Make this be a parameter for specifying for this descriptor
           sets what buffer types to allocate and the size

            These are examples of the new changes of how setting up the
           descriptor sets are going to work

            New API (once this works)
            std::array<pool_info, 3> allocation_info = {
                {vk::uniform_buffer, descriptor_count},
                {vk::storage_buffer, descriptor_count}
            };

            // Currently how the API looks
            std::array<VkDescriptorSetLayoutBinding, 2> layout_bindings = {
                {.binding = 0, .descriptorType =
           VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = 1, .stageFlags
           = VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT,
           .pImmutableSamplers  = nullptr},
                {.binding = 1, .descriptorType =
           VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = 1,
           .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT, .pImmutableSamplers  =
           nullptr},
            };

            // TODO: Will add a new high-level invokation for the API like:
            std::array<descriptor_layout_binding, 2> new_layout_bindings = {
                { .binding = 0, .type = vk::uniform_buffer, .count=1, .stage =
           shader_type::fragment | shader_type::vertex },
                {.binding = 1, .type = vk::image_sampler, .count=1, .stage =
           shader_type::fragment }
            };

            vk::descriptor_set example_of_new_descriptor_set =
           vk::descriptor_set(allocation_info, layout_bindings);
        */
        std::array<VkDescriptorPoolSize, 2> pool_sizes;
        pool_sizes[0] = {
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = static_cast<uint32_t>(m_descriptor_count),
        };

        pool_sizes[1] = {
            .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = static_cast<uint32_t>(m_descriptor_count),
        };

        // 1. Setting up descriptor pool
        VkDescriptorPoolCreateInfo desc_pool_ci = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .maxSets = m_descriptor_count,
            .poolSizeCount = static_cast<uint32_t>(pool_sizes.size()),
            .pPoolSizes = pool_sizes.data()
        };

        vk_check(vkCreateDescriptorPool(
                   m_driver, &desc_pool_ci, nullptr, &m_descriptor_pool),
                 "vkCreateDescriptorPool",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        console_log_info("descriptor pool created!!!");

        std::vector<VkDescriptorSetLayoutBinding> layout_bindings(p_descriptors.begin(), p_descriptors.end());
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

        console_log_info("descriptor layout created!!!");

        std::vector<VkDescriptorSetLayout> layouts(m_descriptor_count,
                                                   m_descriptor_set_layout);

        VkDescriptorSetAllocateInfo descriptor_set_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = nullptr,
            .descriptorPool = m_descriptor_pool,
            .descriptorSetCount = m_descriptor_count,
            .pSetLayouts = layouts.data()
        };

        m_descriptor_sets.resize(m_descriptor_count);

        vk_check(vkAllocateDescriptorSets(m_driver,
                                          &descriptor_set_alloc_info,
                                          m_descriptor_sets.data()),
                 "vkAllocateDescriptorSets",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        console_log_info("descriptor sets allocated!!!");

    }

    void descriptor_set::update_test_descriptors(const std::span<vk_uniform_buffer>& p_uniforms, const texture& p_texture) {
        for(size_t i = 0; i < m_descriptor_count; i++) {
            VkDescriptorBufferInfo buffer_info = {
                .buffer = p_uniforms[i],
                .offset = 0,
                .range = sizeof(camera_ubo),
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
                              const VkPipelineLayout& p_layout) {
        if (m_descriptor_sets.size() > 0) {
            vkCmdBindDescriptorSets(p_current,
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    p_layout,
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