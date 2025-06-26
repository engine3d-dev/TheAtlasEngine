#pragma once
#include <string>
#include <initializer_list>
#include <span>
#include <vector>
#include <vulkan/vulkan.h>
#include <drivers/vulkan-cpp/vk_driver.hpp>
#include <drivers/vulkan-cpp/vk_types.hpp>

namespace atlas::vk {
    
    /**
     * @brief Refers to the input rate
     * 
     * vertex - refers to most common rate. Indicates GPU to move next data entry (next vertex data)
     *          into a buffer for every single vertex that's processed.
     *       - Used for vertex attributes that change per vertex on the mesh
     *       - vertex attribute-only data
     *       - Per-object based specification in the next data entry
     * 
     * instance - refers to data entry per-instance. Specifying to the GPU that
     *           the data entry in the buffer is to be after the instance of the object itself.
     *         - Typically used for instanced rendering. Specfying next entry of data to be after instanced
     *           drawn, could be shared. Therefore instance is an option to choose from if vertex data is
     *           across as a per-instance basis.
     *        - instance-based specification next data entry
     * 
    */
    enum class input_rate : uint8_t {
        vertex,
        instance,
        max_enum
    };


    /*
    
        Setting up vertex_attribute_layout

        Specified for:
        NOTE: bindings is for the vertex (geoemtry data) such as the following below. These are locations 1 to 3. But they are set to binding 0
                 The span is used for setting up these bindings of these vertex attributes, if no binding was specified
        
        layout(location = 0) in vec3 inPosition;
        layout(location = 1) in vec3 inColor;
        layout(location = 2) in vec3 inNormals;
        layout(location = 3) in vec2 inTexCoords;

        m_shader_group.vertex_attributes({
            {.location = 0, .binding = 0, .format = color_format::sfloat_rgb32, .stride=offsetof(vk::vertex, position) },
            {.location = 1, .binding = 0, .format = color_format::sfloat_rgb32, .stride=offsetof(vk::vertex, color) },
            {.location = 2, .binding = 0, .format = color_format::sfloat_rgb32, .stride=offsetof(vk::vertex, normals) },
            {.location = 3, .binding = 0, .format = color_format::sfloat_rg32, .stride=offsetof(vk::vertex, uv) }
        });

        m_shader_group.vertex_bind_attributes({
            {.binding = 0, .stride = sizeof(vk::vertex), .input = input_rate::vertex}
        });


        New API Usage: TODO Implement

        Thngs to consider:
            * binding can be 0 (zero) if the vertex attributes that are specified are for vertex data of the mesh per-vertex
            * binding is a means to specify slots of where the data are going to be sent.
        
        Things to think about:
            * How will setting binding=0 be consistent across the vertex attributes themselves but
              also enable themselves to have varied binding points.
        
        NOTE: I think I am happy for the time being in how the new API is going to 

        std::array<vertex_attributes_entry, 4> attributes = {
            {.location = 0, .format = color_format::sfloat_rgb32, .stride=offsetof(vk::vertex, position) },
            {.location = 1, .format = color_format::sfloat_rgb32, .stride=offsetof(vk::vertex, color) },
            {.location = 2, .format = color_format::sfloat_rgb32, .stride=offsetof(vk::vertex, normals) },
            {.location = 3, .format = color_format::sfloat_rg32, .stride=offsetof(vk::vertex, uv) }
        };

        struct vertex_attribute_entry {
            uint32_t location;      // location data entry goes to
            color_format format;    // specify the type of data the entry to expect
            uint32_t stride;        // stride consistent with the daa being sent to this specific location slot
        };

        struct vertex_attribute {
            // binding slot specifically to the location of data being sent
            uint32_t binding = 0;

            // we may have multiple different attributes that may be assigned to this specific binding point (like mesh vertex attribute for the specific vertex data)
            std::span<vertex_attribute_entry> entries{};
        };

        std::array<vertex_attribute, 1> vertex_input_attributes = {
            vertex_attribute{
                .binding = 0,
                .stride = sizeof(vk::vertex),
                .input_rate = input_rate::vertex
                .attributes = attributes
            }
        };

        m_shader_group.vertex_attributes(vertex_input_attributes);
    
    */
    struct vertex_attribute_entry {
        uint32_t location;
        format format;
        uint32_t stride;
    };

    struct vertex_attribute {
        uint32_t binding;
        std::span<vertex_attribute_entry> entries;
        uint32_t stride;
        input_rate input_rate;
    };

    struct shader_info {
        std::string source="";
        shader_stage stage=undefined;
    };

    struct vk_shader_module {
        VkShaderModule module_handler=nullptr;
        shader_stage stage=undefined;
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

        void vertex_attributes(const std::span<vertex_attribute>& p_vertex_attributes);

        //! @return span of vk_shader_module that contains the handler and stage this handler was loaded at
        [[nodiscard]] std::span<const vk_shader_module> data() const { return m_shader_modules; }

        //! @return vertex attributes set in the vertex shader
        [[nodiscard]] std::span<VkVertexInputAttributeDescription> vertex_attributes() { return m_vertex_attributes; }

        //! @return binding vertex attributes also set in the vertex shader
        [[nodiscard]] std::span<VkVertexInputBindingDescription> vertex_bind_attributes() {return m_vertex_binding_attributes; }


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