#include <drivers/vulkan-cpp/vk_descriptor_set.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <core/engine_logger.hpp>
#include <array>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>

namespace atlas::vk {

    std::string descriptor_set_type_to_string(const VkDescriptorType& p_type) {
        switch (p_type) {
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                return "VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER";
            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
                return "VK_DESCRIPTOR_TYPE_STORAGE_BUFFER";
            case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                return "VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER";
            default:
                return "vulkan descriptor type default!!";
        }
    }

    // VkImageLayout to_vk_image_layout(const image_layout& p_layout) {
    //     switch (p_layout){
    //     case image_layout::shader_read_only: return
    //     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; default:
    //         break;
    //     }
    // }

    descriptor_set::descriptor_set(const uint32_t& p_set_slot,
                                   const descriptor_set_layout& p_layout)
      : m_set_slot(p_set_slot)
      , m_allocated_descriptors(p_layout.allocate_count)
      , m_size_bytes(p_layout.size_bytes) {
        m_driver = vk_context::driver_context();
        texture_extent extent = { .width = 1, .height = 1 };
        m_error_texture = texture(extent);
        std::vector<VkDescriptorPoolSize> pool_sizes(p_layout.entry.size());
        std::vector<VkDescriptorSetLayoutBinding>
          descriptor_set_layout_bindings(p_layout.entry.size());

        for (size_t i = 0; i < pool_sizes.size(); i++) {
            VkDescriptorType type =
              to_vk_descriptor_type(p_layout.entry[i].type);
            std::string type_str = descriptor_set_type_to_string(type);
            pool_sizes[i] = { .type = type,
                              .descriptorCount =
                                static_cast<uint32_t>(p_layout.max_sets) };
        }

        for (size_t i = 0; i < descriptor_set_layout_bindings.size(); i++) {
            VkDescriptorType type =
              to_vk_descriptor_type(p_layout.entry[i].type);
            std::string type_str = descriptor_set_type_to_string(type);

            descriptor_set_layout_bindings[i] = {
                .binding = p_layout.entry[i].binding_point.binding,
                .descriptorType = to_vk_descriptor_type(p_layout.entry[i].type),
                .descriptorCount = p_layout.entry[i].descriptor_count,
                .stageFlags =
                  to_vk_shader_stage(p_layout.entry[i].binding_point.stage),
            };
        }

        VkDescriptorPoolCreateInfo pool_ci = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .maxSets = p_layout.max_sets,
            .poolSizeCount = static_cast<uint32_t>(pool_sizes.size()),
            .pPoolSizes = pool_sizes.data()
        };

        vk_check(vkCreateDescriptorPool(
                   m_driver, &pool_ci, nullptr, &m_descriptor_pool),
                 "vkCreateDescriptorPool");

        VkDescriptorSetLayoutCreateInfo descriptor_layout_ci = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .bindingCount =
              static_cast<uint32_t>(descriptor_set_layout_bindings.size()),
            .pBindings = descriptor_set_layout_bindings.data()
        };

        vk_check(
          vkCreateDescriptorSetLayout(
            m_driver, &descriptor_layout_ci, nullptr, &m_descriptor_set_layout),
          "vkCreateDescriptorSetLayout");

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
                 "vkAllocateDescriptorSets");
    }

    void descriptor_set::update(const std::span<vk_uniform_buffer>& p_uniforms,
                                const std::span<texture>& p_textures) {
        std::vector<VkDescriptorBufferInfo> buffer_infos;
        std::vector<VkDescriptorImageInfo> image_infos;

        // Loading all uniforms specified into this descriptor set
        for (const auto& uniform : p_uniforms) {
            buffer_infos.push_back({ .buffer = uniform,
                                     .offset = 0,
                                     .range = uniform.size_bytes() });
        }

        // If there are any textures, load the image_view's and samplers to
        // write them through the descriptor set
        for (const auto& texture : p_textures) {
            if (texture.loaded()) {
                image_infos.push_back(
                  { .sampler = texture.sampler(),
                    .imageView = texture.image_view(),
                    .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL });
            }
            else { // if no image supplied for loading or loading failed, then
                   // use error/default texture
                image_infos.push_back(
                  { .sampler = m_error_texture.sampler(),
                    .imageView = m_error_texture.image_view(),
                    .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL });
            }
        }

        // Updating descriptor sets so shaders can utilize the uniforms and
        // textures
        for (size_t i = 0; i < m_allocated_descriptors; i++) {
            std::vector<VkWriteDescriptorSet> write_descriptors;
            VkWriteDescriptorSet write_buffer{
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = nullptr,
                .dstSet = m_descriptor_sets[i],
                .dstBinding = 0,
                .dstArrayElement = 0,
                .descriptorCount = static_cast<uint32_t>(buffer_infos.size()),
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .pBufferInfo = buffer_infos.data(),
            };

            write_descriptors.push_back(write_buffer);

            VkWriteDescriptorSet write_image{
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = nullptr,
                .dstSet = m_descriptor_sets[i],
                .dstBinding = 1,
                .dstArrayElement = 0,
                .descriptorCount = static_cast<uint32_t>(image_infos.size()),
                .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .pImageInfo = image_infos.data()
            };

            write_descriptors.push_back(write_image);

            vkUpdateDescriptorSets(
              m_driver,
              static_cast<uint32_t>(write_descriptors.size()),
              write_descriptors.data(),
              0,
              nullptr);
        }
    }

    void descriptor_set::update(
      const std::span<vk_uniform_buffer>& p_uniforms) {
        for (size_t i = 0; i < m_allocated_descriptors; i++) {
            VkDescriptorBufferInfo buffer_info = {
                .buffer = p_uniforms[i],
                .offset = 0,
                .range = m_size_bytes, // m_size_bytes replaces doing things
                                       // like: sizeof(camera_ubo)
            };

            // std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
            std::array<VkWriteDescriptorSet, 1> write_descriptors;
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
            vkUpdateDescriptorSets(
              m_driver,
              static_cast<uint32_t>(write_descriptors.size()),
              write_descriptors.data(),
              0,
              nullptr);
        }
    }

    void descriptor_set::bind(const VkCommandBuffer& p_current,
                              uint32_t p_frame_index,
                              const VkPipelineLayout& p_pipeline_layout) {
        if (m_descriptor_sets.size() > 0) {
            vkCmdBindDescriptorSets(p_current,
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    p_pipeline_layout,
                                    m_set_slot,
                                    1,
                                    &m_descriptor_sets[p_frame_index],
                                    0,
                                    nullptr);
        }
    }

    void descriptor_set::destroy() {
        if (m_descriptor_pool != nullptr) {
            vkDestroyDescriptorPool(m_driver, m_descriptor_pool, nullptr);
        }

        if (m_descriptor_set_layout != nullptr) {
            vkDestroyDescriptorSetLayout(
              m_driver, m_descriptor_set_layout, nullptr);
        }

        m_error_texture.destroy();
    }
};
