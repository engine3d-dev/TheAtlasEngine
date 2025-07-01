#include <drivers/vulkan-cpp/vk_shader_group.hpp>
#include <fstream>
#include <core/engine_logger.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <shaderc/shaderc.hpp>
#include <filesystem>

namespace atlas::vk {

    static VkFormat to_vk_format(const format& p_format) {
        switch (p_format){
        case format::rg32_sfloat:
            return VK_FORMAT_R32G32_SFLOAT;
        case format::rgb32_sfloat:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        default:
            return VK_FORMAT_UNDEFINED;
        }
    }

    static VkVertexInputRate to_vk_input_rate(const input_rate& p_input_rate) {
        switch (p_input_rate){
        case input_rate::vertex:
            return VK_VERTEX_INPUT_RATE_VERTEX;
        case input_rate::instance:
            return VK_VERTEX_INPUT_RATE_INSTANCE;
        default:
            return VK_VERTEX_INPUT_RATE_MAX_ENUM;
        }
    }
    

    static std::string read_file(const std::string& p_file) {
        std::ifstream ins(p_file, std::ios::ate | std::ios::binary);

        if(!ins) {
            console_log_error("{} file could not be loaded!!!", p_file);
            return {};
        }

        uint32_t file_size = (uint32_t)ins.tellg();
        std::string output="";
        output.resize(file_size);
        ins.seekg(0);
        ins.read(output.data(), file_size);
        return output;
    }

    static VkShaderModule create_shader_module(const std::span<uint32_t>& p_binary_blob) {
        VkDevice driver = vk_context::driver_context();

        VkShaderModuleCreateInfo shader_module_ci = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = nullptr,
            .codeSize = static_cast<uint32_t>(p_binary_blob.size_bytes()),
            .pCode = p_binary_blob.data()
        };

        VkShaderModule shader_module=nullptr;

        vk_check(vkCreateShaderModule(driver, &shader_module_ci, nullptr, &shader_module), "vkCreateShaderModule", __FILE__, __LINE__, __FUNCTION__);

        return shader_module;
    }

    static std::vector<uint32_t> compile_shader_source(const shader_info& p_shader_source) {
        std::vector<uint32_t> binary_blob{};

        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
        options.SetWarningsAsErrors();

        shaderc_shader_kind shader_type;

        switch (p_shader_source.stage){
        case shader_stage::vertex:
            shader_type = shaderc_glsl_vertex_shader;
            break;
        case shader_stage::fragment:
            shader_type = shaderc_glsl_fragment_shader;
        default:
            break;
        }

        shaderc::Compiler compiler;
        std::string filename = p_shader_source.source;
        if(std::filesystem::is_regular_file(filename)) {
            std::string shader_raw_source = read_file(filename);

            shaderc::CompilationResult result = compiler.CompileGlslToSpv(
                shader_raw_source.data(),
                shader_raw_source.size(),
                shader_type,
                filename.c_str(),
                "main",
                options
            );

            if(result.GetCompilationStatus() != shaderc_compilation_status_success) {
                console_log_error("{} file had a shader compilation error with status {}", filename, (int)result.GetCompilationStatus());
                console_log_error("{}", result.GetErrorMessage());

                return {};
            }

            for(const uint32_t& block : result) {
                binary_blob.push_back(block);
            }
        }
        else {
            std::string source = p_shader_source.source;
            shaderc::CompilationResult result = compiler.CompileGlslToSpv(
                source.data(), 
                source.size(), 
                shader_type, 
                "builtin",
                "main", 
                options);
            
            //! @note TODO: Should have a better approach at checking if shaderc failed to compile shaders
            if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
                console_log_error("Shader compilation with raw shader string with exit status {}", (int)result.GetCompilationStatus());
                console_log_error("{}", result.GetErrorMessage());
                return {};
            }

            for (uint32_t thing : result) {
                binary_blob.push_back(thing);
            }
        }

        return binary_blob;
    }

    vk_shader_group::vk_shader_group(const std::initializer_list<shader_info>& p_list) : m_shader_sources(p_list) {
        m_driver = vk_context::driver_context();
        compile();
    }

    vk_shader_group::vk_shader_group(const std::span<shader_info>& p_shader_sources) : m_shader_sources(p_shader_sources.begin(), p_shader_sources.end()) {
        m_driver = vk_context::driver_context();
        compile();
    }

    void vk_shader_group::compile() {
        for(const shader_info& info : m_shader_sources) {

            std::vector<uint32_t> binary_blobs = compile_shader_source(info);

            if(!binary_blobs.empty()) {
                vk_shader_module module_info = {
                    .module_handler = create_shader_module(binary_blobs),
                    .stage = info.stage
                };

                m_shader_modules.push_back(module_info);
            }
        }
    }

    void vk_shader_group::destroy() {
        for(size_t i = 0; i < m_shader_modules.size(); i++) {
            vkDestroyShaderModule(m_driver, m_shader_modules[i].module_handler, nullptr);
        }
    }

    void vk_shader_group::set_vertex_attributes(const std::initializer_list<VkVertexInputAttributeDescription>& p_attributes) {
        m_vertex_attributes = std::vector<VkVertexInputAttributeDescription>(p_attributes);
    }

    void vk_shader_group::set_vertex_bind_attributes(const std::initializer_list<VkVertexInputBindingDescription>& p_bind_attributes) {
        m_vertex_binding_attributes = std::vector<VkVertexInputBindingDescription>(p_bind_attributes);
    }

    void vk_shader_group::set_vertex_attributes(const std::span<VkVertexInputAttributeDescription>& p_attributes) {
        m_vertex_attributes = std::vector<VkVertexInputAttributeDescription>(p_attributes.begin(), p_attributes.end());
    }
    
    void vk_shader_group::set_vertex_bind_attributes(const std::span<VkVertexInputBindingDescription>& p_bind_attributes) {
        m_vertex_binding_attributes = std::vector<VkVertexInputBindingDescription>(p_bind_attributes.begin(), p_bind_attributes.end());
    }

    void vk_shader_group::vertex_attributes(const std::span<vertex_attribute>& p_vertex_attributes) {
        // Loading up the vertex binding attributes
        // Vertex binding attributes can be a binding point for specific vertex attribute data
        m_vertex_binding_attributes.resize(p_vertex_attributes.size());

        for(size_t i = 0; i < m_vertex_binding_attributes.size(); i++) {
            const vertex_attribute attribute = p_vertex_attributes[i];
            m_vertex_attributes.resize(attribute.entries.size());
            m_vertex_binding_attributes[i] = {
                .binding = attribute.binding,
                .stride = attribute.stride,
                .inputRate = to_vk_input_rate(attribute.input_rate)
            };

            // Setting the data values to the correct vertex attribute entry
            for(size_t j = 0; j < attribute.entries.size(); j++) {
                const vertex_attribute_entry attr_entry = attribute.entries[j];
                m_vertex_attributes[j] = {
                    .location = attr_entry.location,
                    .binding = attribute.binding,
                    .format = to_vk_format(attr_entry.format),
                    .offset = attr_entry.stride
                };
            }
        }
    }
};