#pragma once
#include <vulkan/vulkan.h>
#include <drivers/vulkan-cpp/vk_types.hpp>
#include <drivers/vulkan-cpp/vk_uniform_buffer.hpp>
#include <span>
#include <vector>

namespace atlas::vk {

    class descriptor_resource {
    public:
        descriptor_resource() = default;
        descriptor_resource(VkDevice device, const descriptor_layout& layout);
        ~descriptor_resource() = default;

        void update(std::span<write_buffer_descriptor> buffer_descriptors);
        void update(std::span<write_buffer_descriptor> buffer_descriptors, 
                   std::span<write_image_descriptor> image_descriptors);
        
        void bind(VkCommandBuffer command_buffer, uint32_t frame_index, VkPipelineLayout pipeline_layout);
        
        VkDescriptorSetLayout layout() const { return m_descriptor_set_layout; }
        
        void destroy();

    private:
        VkDevice m_device = nullptr;
        VkDescriptorPool m_descriptor_pool = nullptr;
        VkDescriptorSetLayout m_descriptor_set_layout = nullptr;
        std::vector<VkDescriptorSet> m_descriptor_sets;
        uint32_t m_set_slot = 0;
        uint32_t m_allocated_descriptors = 0;
        uint32_t m_size_bytes = 0;
    };

} // namespace atlas::vk
