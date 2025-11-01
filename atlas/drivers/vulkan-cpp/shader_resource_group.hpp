#pragma once
#include <vulkan/vulkan.h>
#include <vulkan-cpp/shader_resource.hpp>
#include <unordered_map>

namespace atlas::vk {
    class shader_resource_group {
    public:
        shader_resource_group() = default;
        shader_resource_group(const VkDevice& p_device, const ::vk::shader_resource_info& p_info);
        ~shader_resource_group() = default;

        [[nodiscard]] bool valid() const { return m_resource_valid; }

        void vertex_attributes(std::span<const ::vk::vertex_attribute> p_attributes);


        [[nodiscard]] std::vector<::vk::shader_handle> handles() const {
            return map_to_vector();
        }

        [[nodiscard]] std::span<const VkVertexInputAttributeDescription> vertex_attributes() const { return m_vertex_attributes; }

        [[nodiscard]] std::span<const VkVertexInputBindingDescription> vertex_bind_attributes() const { return m_vertex_binding_attributes; }

        void destroy();

    private:
        // this just converts unordered_map<string, shader_handle> to vector<shader_handle>
        [[nodiscard]] std::vector<::vk::shader_handle> map_to_vector() const;

    private:
        VkDevice m_device=nullptr;
        std::vector<VkVertexInputAttributeDescription> m_vertex_attributes;
        std::vector<VkVertexInputBindingDescription>
          m_vertex_binding_attributes;
        bool m_resource_valid = false;
        // shader module hanles
        // for VkShaderModule
        // std::vector<::vk::shader_handle> m_modules; // shader module handles
        std::unordered_map<std::string, ::vk::shader_handle> m_modules;

    };
};