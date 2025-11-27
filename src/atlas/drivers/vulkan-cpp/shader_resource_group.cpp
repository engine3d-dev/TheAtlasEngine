#include <drivers/vulkan-cpp/shader_resource_group.hpp>
#include <fstream>
#include <filesystem>
#include <vulkan-cpp/utilities.hpp>
#include <ranges>
#include <shaderc/shaderc.hpp>
#include <core/engine_logger.hpp>

namespace atlas::vk {

    // Reading the raw .spv binaries
    static std::vector<char> read_raw_spirv(const std::string& p_file) {
        std::vector<char> out_buffer;
        std::ifstream ins(p_file, std::ios::ate | std::ios::binary);

        if (!ins) {
            throw std::runtime_error("Cannot load in .spv files!!");
        }

        uint32_t file_size = (uint32_t)ins.tellg();
        out_buffer.resize(file_size);
        ins.seekg(0);
        ins.read(out_buffer.data(), file_size);
        return out_buffer;
    }

    //! @brief Ensure file reads are valid before reading raw .spv binaries
    static std::vector<char> compile_binary_shader_source(
      const ::vk::shader_source& p_shader_source) {

        if (!std::filesystem::is_regular_file(p_shader_source.filename)) {
            throw std::runtime_error("Cannot load .spv file");
        }

        return read_raw_spirv(p_shader_source.filename);
    }

    static std::string read_shader_source_code(const std::string& p_filename) {
        std::ifstream ins(p_filename, std::ios::ate | std::ios::binary);

        if (!ins.is_open()) {
            console_log_warn("Could not open filename = {}", p_filename);
            return { 'a' };
        }

        size_t file_size = (size_t)ins.tellg();
        std::string output;
        output.resize(file_size);
        ins.seekg(0);
        ins.read(output.data(), static_cast<uint32_t>(file_size));

        return output;
    }

    /**
     * compiles source code from the shader directly without needing manual
     * recompilation
     *
     * shaderc requires these parameters to compile
     * text_source_code: the std::string version of the entire source code to
     * compile type: shader stage this shader corresponds to filename: input
     * filename text entry_point: the entry point to this shader options:
     * compiler-specific options to enable when compiling the shader sources
     */
    static std::vector<uint32_t> compile_source_from_file(
      const ::vk::shader_source& p_shader_source) {
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_vulkan,
                                     shaderc_env_version_vulkan_1_3);
        options.SetWarningsAsErrors();

        shaderc_shader_kind type;

        switch (p_shader_source.stage) {
            case ::vk::shader_stage::vertex:
                type = shaderc_glsl_vertex_shader;
                break;
            case ::vk::shader_stage::fragment:
                type = shaderc_glsl_fragment_shader;
                break;
            default:
                throw std::runtime_error("shader_stage unspecified!~!!");
        }

        shaderc::Compiler compiler;
        std::string text_source_code =
          read_shader_source_code(p_shader_source.filename);

        // Prints out the text of the shader source code
        // console_log_warn("Source Text Code!!!");
        // console_log_info("{}", text_source_code);
        shaderc::CompilationResult result =
          compiler.CompileGlslToSpv(text_source_code,
                                    type,
                                    p_shader_source.filename.c_str(),
                                    "main",
                                    options);

        std::vector<uint32_t> blob;

        if (result.GetCompilationStatus() !=
            shaderc_compilation_status_success) {
            throw std::runtime_error(
              std::format("Shader Compilation Error! Failed with reason {}\n{}",
                          p_shader_source.filename,
                          result.GetErrorMessage())
                .c_str());
        }

        for (auto blob_chunk : result) {
            blob.push_back(blob_chunk);
        }

        return blob;
    }

    shader_resource_group::shader_resource_group(
      const VkDevice& p_device,
      const ::vk::shader_resource_info& p_info)
      : m_device(p_device) {

        // We go through all of the specified shader source and their specific
        // stage Compile them through shader compiler or if provided a .spv,
        // then we compile and read in the stream of bytes directly
        for (size_t i = 0; i < p_info.sources.size(); i++) {
            const ::vk::shader_source shader_src = p_info.sources[i];
            std::filesystem::path filepath =
              std::filesystem::path(shader_src.filename);

            if (filepath.extension().string() == ".spv") {
                std::vector<char> blob =
                  compile_binary_shader_source(shader_src);

                if (blob.empty()) {
                    m_resource_valid = false;
                    throw std::runtime_error("Cannot load in vector<uint32_t> "
                                             "blob of compiled down data!!!");
                }

                create_module(blob, shader_src);
            }
            else {
                std::string text_source_code =
                  read_shader_source_code(filepath.string());
                std::vector<uint32_t> blob =
                  compile_source_from_file(shader_src);
                create_module(blob, shader_src);
            }
        }

        // This is a testing example for getting shaders hot-reloading
        // m_watcher = create_ref<wtr::watch>("./experimental-shaders",
        // [this](const wtr::event& e){
        //     console_log_info("File {} Reload!!!",
        //     e.path_name.filename().string()); if (e.effect_type ==
        //     wtr::event::effect_type::modify and e.path_type ==
        //     wtr::event::path_type::file) {
        //         console_log_info("File has been modified!!");
        //         // std::string fmt = std::format("experimental-shaders/{}",
        //         e.path_name.filename().string());
        //         console_log_info("({}).contains = {}",
        //         e.path_name.filename().string(),
        //         m_modules.contains(e.path_name.filename().string()));
        //         if(m_modules.contains(e.path_name.filename().string())) {
        //             console_log_info("Reload has been requested!!!");
        //             m_reload_requested = true;
        //             // reload
        //             ::vk::shader_source reload_src = {
        //                 .filename = e.path_name.filename().string(),
        //                 .stage =
        //                 m_modules.at(e.path_name.filename().string()).stage
        //             };
        //             reload_shader(reload_src);

        //         }
        //     }
        //     else {
        //         console_log_info("File {} Could Not Be Reloaded!!!",
        //         e.path_name.filename().string());
        //     }
        // });

        m_resource_valid = true;
    }

    void shader_resource_group::reload_shader(
      const ::vk::shader_source& p_source) {
        console_log_info("p_source.filename = {}", p_source.filename);
        if (m_modules[p_source.filename].module != nullptr) {
            vkDestroyShaderModule(
              m_device, m_modules[p_source.filename].module, nullptr);
        }

        auto& handle = m_modules[p_source.filename];

        std::filesystem::path filepath(p_source.filename);
        std::string text_source_code =
          read_shader_source_code(filepath.string());
        std::vector<uint32_t> blob = compile_source_from_file(p_source);
        std::span<uint32_t> view_blob(blob.data(), blob.size());
        // create_module(blob, p_source);
        VkShaderModuleCreateInfo shader_module_ci = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = nullptr,
            .codeSize = view_blob.size_bytes(),
            .pCode = view_blob.data()
        };

        ::vk::vk_check(vkCreateShaderModule(
                         m_device, &shader_module_ci, nullptr, &handle.module),
                       "vkCreateShaderModule");
    }

    void shader_resource_group::create_module(
      std::span<char> p_blob,
      const ::vk::shader_source& p_source) {
        VkShaderModuleCreateInfo shader_module_ci = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = nullptr,
            .codeSize = p_blob.size(),
            .pCode = reinterpret_cast<const uint32_t*>(p_blob.data())
        };

        std::filesystem::path filepath(p_source.filename);
        std::string filename = filepath.filename().string();

        // Setting m_shader_module_handlers[i]'s stage and the VkShaderModule
        // handle altogether construct this beforehand and then we are going set
        // that shader module
        m_modules.emplace(filename, ::vk::shader_handle{});
        ::vk::vk_check(
          vkCreateShaderModule(
            m_device, &shader_module_ci, nullptr, &m_modules[filename].module),
          "vkCreateShaderModule");
        m_modules[filename].stage = p_source.stage;
    }

    void shader_resource_group::create_module(
      std::span<uint32_t> p_blob,
      const ::vk::shader_source& p_source) {
        VkShaderModuleCreateInfo shader_module_ci = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = nullptr,
            .codeSize = p_blob.size_bytes(),
            .pCode = p_blob.data()
        };

        // console_log_info("map key = {}", p_source.filename);
        std::filesystem::path filepath(p_source.filename);

        std::string filename = filepath.filename().string();

        console_log_info("Key = {}", filename);

        // Setting m_shader_module_handlers[i]'s stage and the VkShaderModule
        // handle altogether construct this beforehand and then we are going set
        // that shader module
        m_modules.emplace(filename, ::vk::shader_handle{});
        ::vk::vk_check(
          vkCreateShaderModule(
            m_device, &shader_module_ci, nullptr, &m_modules[filename].module),
          "vkCreateShaderModule");
        m_modules[filename].stage = p_source.stage;
    }

    std::vector<::vk::shader_handle> shader_resource_group::map_to_vector()
      const {
        // Using C++'s std::views to extract all of the values in
        // unordered_map<string, vk::shader_handle> to a vector<shader_handle>
        // that gets passed to graphics pipeline
        return (m_modules | std::views::values |
                std::ranges::to<std::vector>());
    }

    void shader_resource_group::vertex_attributes(
      std::span<const ::vk::vertex_attribute> p_attributes) {
        /*
            -- These comments are a reminder to myself --
            - this function simplifies the need to separately define vertex
           attributes and the vertex binding attributes as shown below:

            - vertex attributes specify the types of data within the vertex

            - vertex binding attribute specifies the rate of reading that data
           layout specified by the vertex attributes

            - Interpret the following vertex attributes below with this shader
           code with `layout(location = n)` specified where by default these are
           set to binding zero by the shader

            layout(location = 0) in vec3 inPosition;
            layout(location = 1) in vec3 inColor;
            layout(location = 2) in vec3 inNormals;
            layout(location = 3) in vec2 inTexCoords;

            m_shader_group.set_vertex_attributes(VkVertexInputAttributeDescription{
                { .location = 0, .binding = 0, .format =
           VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(vk::vertex, position),
           }, { .location = 1, .binding = 0, .format =
           VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(vk::vertex, color), },
                { .location = 2, .binding = 0, .format =
           VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(vk::vertex, normals),
           }, { .location = 3, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT,
           .offset = offsetof(vk::vertex, uv), },
            });

            m_shader_group.set_vertex_bind_attributes(VkVertexInputBindingDescription{
                {.binding = 0, .stride = sizeof(vk::vertex), .inputRate =
           VK_VERTEX_INPUT_RATE_VERTEX,},
            });

            Which gets handled in specifying the following below
        */

        m_vertex_binding_attributes.resize(p_attributes.size());

        for (size_t i = 0; i < m_vertex_binding_attributes.size(); i++) {
            // setting up vertex binding
            const ::vk::vertex_attribute attribute = p_attributes[i];
            m_vertex_attributes.resize(attribute.entries.size());
            m_vertex_binding_attributes[i] = { .binding = attribute.binding,
                                               .stride = attribute.stride,
                                               .inputRate = to_input_rate(
                                                 attribute.input_rate) };

            // then setting up the vertex attributes for the vertex data layouts
            for (size_t j = 0; j < attribute.entries.size(); j++) {
                const ::vk::vertex_attribute_entry entry = attribute.entries[j];
                m_vertex_attributes[j] = { .location = entry.location,
                                           .binding = attribute.binding,
                                           .format = static_cast<VkFormat>(
                                             entry.format),
                                           .offset = entry.stride };
            }
        }
    }

    void shader_resource_group::destroy() {

        for (auto& [filename, shader_handle] : m_modules) {
            if (shader_handle.module != nullptr) {
                vkDestroyShaderModule(m_device, shader_handle.module, nullptr);
            }
        }
    }
};