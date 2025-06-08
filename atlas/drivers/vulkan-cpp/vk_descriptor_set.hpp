#pragma once
#include <cstdint>
#include <string>
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <span>
#include <vulkan/vulkan.h>
#include <vector>
#include <drivers/vulkan-cpp/vk_uniform_buffer.hpp>
#include <drivers/vulkan-cpp/vk_vertex_buffer.hpp>
#include <drivers/vulkan-cpp/vk_uniform_buffer.hpp>
#include <drivers/vulkan-cpp/vk_texture.hpp>

namespace atlas::vk {

    enum descriptor_type : uint8_t {
        StorageBuffer=0,
        UniformBuffer=1,
        ImageAndSampler=2
    };

    struct vk_descriptor_set_properties {
        vk_descriptor_set_properties(const std::string& p_name,
                                     const uint32_t p_layout_binding,
                                     const descriptor_type& p_type,
                                     const shader_stage& p_stage,
                                     bool p_has_samplers = false)
          : name(p_name)
          , binding(p_layout_binding)
          , type(p_type)
          , stage(p_stage)
          , has_samplers(p_has_samplers) {}

        std::string name = "Undefined";
        uint32_t binding = -1;
        descriptor_type type;
        shader_stage stage;
        bool has_samplers = false;
    };

    class write_descriptor_set {
    public:

    private:
    };

    class descriptor_set {
    public:
        descriptor_set() = default;
        descriptor_set(uint32_t p_count, const std::initializer_list<VkDescriptorSetLayoutBinding>& p_list);
        descriptor_set(const std::span<VkDescriptorSetLayoutBinding>& p_descriptors);

        ~descriptor_set() = default;

        void bind(const VkCommandBuffer& p_current, uint32_t p_frame_index, const VkPipelineLayout& p_layout);

        /**
         * 
         * Something to note for myself
         * The vk_renderer will actually store in and cache vk::mesh into a map<string, vk::mesh>
         * Each mesh will actually store the actual geometry data and apply a per-pipeline-info on them
         * This update_mesh would be invoked like(m_uniformss, m_cached_meshes[p_entity.name()]);
         * Where we invoke the mesh we want to update with the following invokation call here
         * vk::mesh will also contain the matrices such as glm::mat4 that is the model matrix
        */
        // void update_mesh(const std::span<vk_uniform_buffer>& p_uniforms, const mesh& p_mesh);

        [[nodiscard]] VkDescriptorPool get_pool() const { return m_descriptor_pool; }

        [[nodiscard]] VkDescriptorSetLayout get_layout() const { return m_descriptor_set_layout; }

        void update_test_descriptors(const std::span<vk_uniform_buffer>& p_uniforms, const texture& p_texture);

        void destroy();

    private:
        VkDevice m_driver=nullptr;
        uint32_t m_descriptor_count=0;
        VkDescriptorPool m_descriptor_pool=nullptr;
        VkDescriptorSetLayout m_descriptor_set_layout=nullptr;
        std::vector<VkDescriptorSet> m_descriptor_sets{};
    };
};