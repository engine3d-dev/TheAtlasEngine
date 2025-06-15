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

    class descriptor_set {
    public:
        descriptor_set() = default;
        /**
         * @param p_set_slot is the slot index to bind this descriptor set
         * Tells vulkan when binding this descriptor set where the resources location for shader to know where to access the resources binded
         * 
        */
        descriptor_set(const uint32_t& p_set_slot, const descriptor_set_layout&);
        ~descriptor_set() = default;

        void bind(const VkCommandBuffer& p_current, uint32_t p_frame_index, const VkPipelineLayout&);

        /**
         * 
         * Something to note for myself
         * The vk_renderer will actually store in and cache vk::mesh into a map<string, vk::mesh>
         * Each mesh will actually store the actual geometry data and apply a per-pipeline-info on them
         * This update_mesh would be invoked like(m_uniformss, m_cached_meshes[p_entity.name()]);
         * Where we invoke the mesh we want to update with the following invokation call here
         * vk::mesh will also contain the matrices such as glm::mat4 that is the model matrix
        */

        [[nodiscard]] VkDescriptorPool get_pool() const { return m_descriptor_pool; }

        [[nodiscard]] VkDescriptorSetLayout get_layout() const { return m_descriptor_set_layout; }

        void update_test_descriptors(const std::span<vk_uniform_buffer>& p_uniforms, const texture& p_texture);

        //! @brief update descriptor if ofloading multiple uniforms
        void update(const std::span<vk_uniform_buffer>& p_uniforms);

        //! @brief update descriptor if by single uniforms
        void update(const vk_uniform_buffer& p_uniforms);

        //! @brief update descriptor if accepting multiple textures
        void update(const std::span<texture>& p_textures);

        void destroy();

    private:
        VkDevice m_driver=nullptr;
        uint32_t m_set_slot = 0;
        uint32_t m_allocated_descriptors=0;
        uint32_t m_size_bytes=0;
        VkDescriptorPool m_descriptor_pool=nullptr;
        VkDescriptorSetLayout m_descriptor_set_layout=nullptr;
        std::vector<VkDescriptorSet> m_descriptor_sets{};
    };
};