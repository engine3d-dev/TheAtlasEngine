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

    VkDescriptorType to_descriptor_set_type(const buffer& p_type) {
        switch (p_type) {
            case buffer::Storage:
                return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            case buffer::Uniform:
                return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            case buffer::ImageSampler:
                return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        }

        console_log_error("descriptor set type specified is invalid!!!");
    }

    std::string descriptor_set_type_to_string(const VkDescriptorType& p_type) {
        switch (p_type){
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

    descriptor_set::descriptor_set(const uint32_t& p_set_slot, const descriptor_set_layout2& p_layout) : m_set_slot(p_set_slot), m_allocated_descriptors(p_layout.allocate_count), m_size_bytes(p_layout.size_bytes) {
        m_driver = vk_context::driver_context();

        std::vector<VkDescriptorPoolSize> pool_sizes(p_layout.entry.size());
        std::vector<VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings(p_layout.entry.size());

        console_log_fatal("entries.size() = {}", p_layout.entry.size());

        for(size_t i = 0; i < pool_sizes.size(); i++) {
            VkDescriptorType type = to_descriptor_set_type(p_layout.entry[i].type);
            std::string type_str = descriptor_set_type_to_string(type);
            pool_sizes[i] = {
                .type = type,
                .descriptorCount = static_cast<uint32_t>(p_layout.entry[i].descriptor_count)
            };
            console_log_fatal("pool_sizes[{}].type = {}", i, type_str);
        }

        for(size_t i = 0; i < descriptor_set_layout_bindings.size(); i++) {
            VkDescriptorType type = to_descriptor_set_type(p_layout.entry[i].type);
            std::string type_str = descriptor_set_type_to_string(type);
            console_log_fatal("descriptor_set_layout_bindings[{}].type = {}", i, type_str);
            descriptor_set_layout_bindings[i] = {
                .binding = p_layout.entry[i].binding_point.binding,
                .descriptorType = to_descriptor_set_type(p_layout.entry[i].type),
                .descriptorCount = p_layout.entry[i].descriptor_count,
                .stageFlags = to_vk_shader_stage(p_layout.entry[i].binding_point.stage),
            };

            console_log_fatal("p_layout.entry[{}].binding_point.binding = {}", i, p_layout.entry[i].binding_point.binding);
            console_log_fatal("p_layout.entry[{}].desciptor_count = {}", i, p_layout.entry[i].descriptor_count);
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
                 "vkCreateDescriptorPool",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        VkDescriptorSetLayoutCreateInfo descriptor_layout_ci = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .bindingCount = static_cast<uint32_t>(descriptor_set_layout_bindings.size()),
            .pBindings = descriptor_set_layout_bindings.data()
        };

        vk_check(vkCreateDescriptorSetLayout(m_driver,
                                             &descriptor_layout_ci,
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


    descriptor_set::descriptor_set(const uint32_t& p_set_slot, const descriptor_set_layout& p_layout) : m_set_slot(p_set_slot), m_allocated_descriptors(p_layout.allocate_count), m_size_bytes(p_layout.size_bytes) {
        m_driver = vk_context::driver_context();

        // 1. Setting up descriptor pool for allocations
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

    void descriptor_set::update(const std::span<vk_uniform_buffer>& p_uniforms, const std::span<texture>& p_textures) {
        std::vector<VkDescriptorBufferInfo> buffer_infos;
        std::vector<VkDescriptorImageInfo> image_infos;

        // console_log_fatal("size_bytes() = {}", p_uniforms.size_bytes());

        // Loading all uniforms specified into this descriptor set
        for(const auto& uniform : p_uniforms) {
            buffer_infos.push_back({
                .buffer = uniform,
                .offset = 0,
                .range = uniform.size_bytes()
            });
        }

        // If there are any textures, load the image_view's and samplers to write them through the descriptor set
        if(!p_textures.empty()) {
            for(const auto& texture : p_textures) {
                image_infos.push_back({
                    .sampler = texture.sampler(),
                    .imageView = texture.image_view(),
                    .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                });
            }
        }
        else {
            image_infos.push_back({
                .sampler = nullptr,
                .imageView = nullptr,
                .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            });
        }

        // Updating descriptor sets so shaders can utilize the uniforms and textures
        for(size_t i = 0; i < m_allocated_descriptors; i++) {
            std::vector<VkWriteDescriptorSet> write_descriptors;
            VkWriteDescriptorSet write_buffer {
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

            VkWriteDescriptorSet write_image {
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

            vkUpdateDescriptorSets(m_driver, static_cast<uint32_t>(write_descriptors.size()), write_descriptors.data(), 0, nullptr);
        }
    }

    void descriptor_set::update_test_descriptors(const std::span<vk_uniform_buffer>& p_uniforms, [[maybe_unused]] const texture& p_texture) {
        for(size_t i = 0; i < m_allocated_descriptors; i++) {
            VkDescriptorBufferInfo buffer_info = {
                .buffer = p_uniforms[i],
                .offset = 0,
                .range = p_uniforms.size_bytes(), // m_size_bytes replaces doing things like: sizeof(camera_ubo)
            };

            // VkDescriptorImageInfo image_info = {
            //     .sampler = p_texture.sampler(),
            //     .imageView = p_texture.image_view(),
            //     .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            // };

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
            // write_descriptors[1] = {
            //     .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            //     .pNext = nullptr,
            //     .dstSet = m_descriptor_sets[i],
            //     .dstBinding = 1,
            //     .dstArrayElement = 0,
            //     .descriptorCount = 1,
            //     .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            //     .pImageInfo = &image_info,
            // };
            vkUpdateDescriptorSets(m_driver, static_cast<uint32_t>(write_descriptors.size()), write_descriptors.data(), 0, nullptr); 
        }
    }

    void descriptor_set::update(const std::span<vk_uniform_buffer>& p_uniforms) {
        for(size_t i = 0; i < m_allocated_descriptors; i++) {
            VkDescriptorBufferInfo buffer_info = {
                .buffer = p_uniforms[i],
                .offset = 0,
                .range = m_size_bytes, // m_size_bytes replaces doing things like: sizeof(camera_ubo)
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
            vkUpdateDescriptorSets(m_driver, static_cast<uint32_t>(write_descriptors.size()), write_descriptors.data(), 0, nullptr);
        }
    }

    void descriptor_set::update(const vk_uniform_buffer& p_uniform) {
        for(size_t i = 0; i < m_allocated_descriptors; i++) {
            VkDescriptorBufferInfo buffer_info = {
                .buffer = p_uniform,
                .offset = 0,
                .range = m_size_bytes, // m_size_bytes replaces doing things like: sizeof(camera_ubo)
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
            vkUpdateDescriptorSets(m_driver, static_cast<uint32_t>(write_descriptors.size()), write_descriptors.data(), 0, nullptr);
        }
    }


    /*
     p_set_index is to tell vulkan which "slot" to put the descriptor set in

     beacuse our layout is like as shown here

     layout (set = 0, binding = 0) uniform GlobalUbo {
        mat4 mvp;
     } ubo;

     layout (set = 1, binding = 0) uniform MaterialUbo {
        vec4 color;
     } material_src;

     * If you bind the descriptor set at the wrong slot, the shader will not be able to bind to the resources
     * If you bind to set index 0 but the set is at 1, the shader will not be able to find it and give you validation error
     * If you bind two different descriptor sets at the same index, the second one overwrites the first one
     * 
     * [Requirements for Descriptor Set Binding]
     * Assign each descriptor set a slot index, to ensure it is the correct slot being binded.
     * Ensure that the shader resource group would be able to find that particular resource binded to this descriptor set
     * No descriptor sets should overwrite each other
     * Error occurred: Reason why validation errors occurred was because I was binding to index slot 0, when the validation layers and vulkan were searching for index slot 1 instead
     * 
    */
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
        if(m_descriptor_pool != nullptr) {
            vkDestroyDescriptorPool(m_driver, m_descriptor_pool, nullptr);
        }

        if(m_descriptor_set_layout != nullptr) {
            vkDestroyDescriptorSetLayout(
            m_driver, m_descriptor_set_layout, nullptr);
        }
    }
};