#pragma once
#include <string>
#include <initializer_list>
#include <span>
#include <vector>
#include <vulkan/vulkan.h>
#include <drivers/vulkan-cpp/vk_driver.hpp>
#include <drivers/vulkan-cpp/vk_types.hpp>

namespace atlas::vk {

    struct shader_info {
        std::string source="";
        shader_stage stage=Undefined;
    };

    struct vk_shader_module {
        VkShaderModule module_handler=nullptr;
        shader_stage stage=Undefined;
    };

    class vk_shader_group {
    public:
        vk_shader_group() = default;

        //! @brief Constructing new shader group as an initialized list
        vk_shader_group(const std::initializer_list<shader_info>& p_list);

        //! @brief Constructs a shader group because a pipeline may have multiple shader groups part of the shader pipeline with a specific stage associated with it
        // Does not limit users to the data structure type they need to pass in for the shader sources
        vk_shader_group(const std::span<shader_info>& p_shader_sources);

        void set_vertex_attributes(const std::initializer_list<VkVertexInputAttributeDescription>& p_attributes);
        
        void set_vertex_bind_attributes(const std::initializer_list<VkVertexInputBindingDescription>& p_bind_attributes);

        void set_vertex_attributes(const std::span<VkVertexInputAttributeDescription>& p_attributes);
        
        void set_vertex_bind_attributes(const std::span<VkVertexInputBindingDescription>& p_bind_attributes);

        //! @return span of vk_shader_module that contains the handler and stage this handler was loaded at
        [[nodiscard]] std::span<const vk_shader_module> data() const { return m_shader_modules; }

        //! @return vertex attributes set in the vertex shader
        [[nodiscard]] std::span<VkVertexInputAttributeDescription> get_vertex_attributes() { return m_vertex_attributes; }

        //! @return binding vertex attributes also set in the vertex shader
        [[nodiscard]] std::span<VkVertexInputBindingDescription> get_vertex_bind_attributes() {return m_vertex_binding_attributes; }


        //! @return uint32_t the size of vulkan shader modules loaded
        [[nodiscard]] uint32_t size() const { return m_shader_modules.size(); }

        void compile();

        void destroy();

    private:
        vk_driver m_driver{};
        std::vector<shader_info> m_shader_sources{};
        std::vector<vk_shader_module> m_shader_modules{};

        std::vector<VkVertexInputAttributeDescription> m_vertex_attributes{};
        std::vector<VkVertexInputBindingDescription> m_vertex_binding_attributes{};
    };
};