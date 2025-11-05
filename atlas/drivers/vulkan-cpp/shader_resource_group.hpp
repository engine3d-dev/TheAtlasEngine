#pragma once
#include <vulkan-cpp/shader_resource.hpp>
#include <core/core.hpp>
#include <unordered_map>
// #include <wtr/watcher.hpp>

namespace atlas::vk {
    /**
     * @brief resource group for loading shader sources that give you back
     * VkShaderModule handles
     *
     * Responsibility is to load stages of shader sources whether that be
     * through precompiler .spv files or through shaderc runtime shader
     * compilation
     *
     * Responsibility is loading and streaming the amount of bytes from the
     * compiled shader sources into the vulkan shader module handles
     *
     * resource groups up the creation and management of vulkan shader modules.
     */
    class shader_resource_group {
    public:
        shader_resource_group() = default;
        /**
         * @brief constructs a new shader_resource_group
         *
         * @param p_device is the logical device required to creating the vulkan
         * shader module
         * @param p_info  has the properties such as specified shader sources to
         * load/compile
         */
        shader_resource_group(const VkDevice& p_device,
                              const ::vk::shader_resource_info& p_info);
        ~shader_resource_group() = default;

        /**
         * @return true if resources are valid, otherwise return false
         */
        [[nodiscard]] bool valid() const { return m_resource_valid; }

        /**
         * @brief sets the vertex attributes with the shader modules that gets
         * used by ::vk::pipeline (graphics pipeline)
         *
         * @param p_attributes is the high-level specifications for setting up
         * vertex attributes that correspond with these shaders
         */
        void vertex_attributes(
          std::span<const ::vk::vertex_attribute> p_attributes);

        /**
         * @brief this gives you back the shader module handles along with each
         * of their stages they have been compiled with
         *
         * Returns the vector to retain the shader modules that are needed by
         * the graphics pipeline.
         *
         * It is required by vulkan specs the graphics pipeline to contain valid
         * shader modules of the compiled shaders
         *
         * @return vector<::vk::shader_handle>
         */
        [[nodiscard]] std::vector<::vk::shader_handle> handles() const {
            return map_to_vector();
        }

        /**
         * @return span<const VkVertexInputAttributeDescription>
         */
        [[nodiscard]] std::span<const VkVertexInputAttributeDescription>
        vertex_attributes() const {
            return m_vertex_attributes;
        }

        /**
         * @return span<const VkVertexInputBindingDescription>
         */
        [[nodiscard]] std::span<const VkVertexInputBindingDescription>
        vertex_bind_attributes() const {
            return m_vertex_binding_attributes;
        }

        /**
         * @brief explicit cleanup to the VkShaderModule handles created with
         * this particular resource group
         */
        void destroy();

        /**
         * @brief ideally used for requesting for reload
         *
         * Planning to use this for invalidation when for runtime shader
         * hot-reloading
         *
         * @note this is not used at the moment as shader runtime hot reloading
         * is currently not supported.
         */
        [[nodiscard]] bool reload_requested() const {
            return m_reload_requested;
        }

    private:
        /**
         * converts unordered_map<string, shader_handle> to
         * vector<shader_handle>
         */
        [[nodiscard]] std::vector<::vk::shader_handle> map_to_vector() const;

        void create_module(std::span<char> p_blob,
                           const ::vk::shader_source& p_source);

        void create_module(std::span<uint32_t> p_blob,
                           const ::vk::shader_source& p_source);

        void reload_shader(const ::vk::shader_source& p_source);

    private:
        VkDevice m_device = nullptr;
        std::vector<VkVertexInputAttributeDescription> m_vertex_attributes;
        std::vector<VkVertexInputBindingDescription>
          m_vertex_binding_attributes;
        bool m_resource_valid = false;
        // shader module handles
        std::unordered_map<std::string, ::vk::shader_handle> m_modules;
        // ref<wtr::watch> m_watcher;
        bool m_reload_requested = false;
    };
};