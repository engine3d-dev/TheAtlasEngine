#include <drivers/vulkan-cpp/vk_shader_group.hpp>
#include <fstream>
#include <core/engine_logger.hpp>
#include <drivers/vulkan-cpp/vk_context.hpp>
#include <drivers/vulkan-cpp/helper_functions.hpp>
#include <shaderc/shaderc.hpp>
#include <filesystem>

namespace atlas::vk {

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
        case shader_stage::Vertex:
            shader_type = shaderc_glsl_vertex_shader;
            break;
        case shader_stage::Fragment:
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
        compile();
    }

    void vk_shader_group::compile() {
        console_log_info("vk_shader_group::compile begin initializing!!!");
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

        console_log_info("m_shader_modules.size() = {}", m_shader_modules.size());
        console_log_info("m_shader_sources.size() = {}", m_shader_sources.size());
        console_log_info("vk_shader_group::compile end initializing!!!\n\n");
    }

    void vk_shader_group::destroy() {
        console_log_warn("vk_shader_group being freed!");
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
};