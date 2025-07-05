#pragma once
#include <vulkan/vulkan.h>
#include <span>
#include <glm/glm.hpp>
#include <string>

namespace atlas::vk {
    /**
     * @param Primary
     * submission: immediately to vulkan queue for executing on device
     * execution: vkQueueSubmit primarily submits primary command buffers
     *
     * @param Seconary
     * submission: Cannot be submitted directly to vulkan queue
     * execution: Indirect execution by being called from primary command
     * buffers using `vkCmdExecuteCommands`
     * 
     * When executed within render-pass instance, they inherit
     * renderpass/subpass state from primary command buffer
     * 
     * Can be allocated and recorded in parallel, which can leverage
     * devices that support multi-core processing
     * 
     * Have independent lifetimes that can be managed independently of
     * primary command buffers, allowing for more flexible resource management
    */
    enum command_buffer_levels : uint8_t {
        primary = 0,
        secondary = 1,
        max_enum = 2
    };

    /**
     * @brief settings for specifying command buffers to construct
     * 
     * @param Levels are for specifying the kind of command buffer being
     * constructed
     * 
     * queue_index specifies the queue family associated with this
     * command buffer
     * 
     * pool_flag in the context of the command buffers are used to
     * control memory allocation behavior within the command buffer pool to
     * allocate
     *
     * @param VK_COMMAND_POOL_CREATE_TRANSIENT_BIT Indicates command buffers
     * allocated from pool will be short-lived, meaning they'll reset or freed
     * quickly
     * @param VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT Allows individual
     * command buffers allocated from the pool to be reset to their initial
     * state using `vkResetCommandBuffer`
     * @param VK_COMMAND_POOL_CREATE_PROTECTED_BIT Designates command buffers
     * allocated from the pool as protective command buffers, implying they are
     * stored in protected memory and access to them is restricted to authorized
     * operations
     */
    struct command_buffer_settings {
        command_buffer_settings(uint32_t p_queue_family,
                                const command_buffer_levels& p_levels,
                                const VkCommandPoolCreateFlagBits& p_pool_flags)
          : levels(p_levels)
          , queue_index(p_queue_family)
          , pool_flag(p_pool_flags) {}

        command_buffer_levels levels;
        uint32_t queue_index = -1;
        VkCommandPoolCreateFlagBits pool_flag;
    };

    struct image_extent {
        uint32_t width;
        uint32_t height;
        VkDeviceMemory device_memory;
        uint32_t mip_level=1;
    };

    struct vk_image_handle {
        VkImage image=nullptr;
        VkImageView image_view=nullptr;
    };

    //! @brief vulkan image handler with resources; used when handling textures
    struct vk_image {
        VkImage image = nullptr;
        VkImageView image_view = nullptr;
        VkSampler sampler = nullptr;
        VkDeviceMemory device_memory = nullptr;
    };

    //! @brief Range between min and max for the VkFilter
    struct vk_filter_range {
        VkFilter min;
        VkFilter max;
    };

    /**
     * @name vk_present_queue
     * @brief index is the queue family index the queue belongs to
     * @brief queue index in the specific queue family to retrieve
    */
    struct vk_queue_options {
        // uint32_t device_index=0; // device num
        uint32_t family_index=-1;
        uint32_t queue_index=0;
    };

    enum class format : uint64_t {
        rgb32_sfloat, // Represent R32G32B32_SFLOAT
        rg32_sfloat, // Represent R32G32_SFLOAT
    };

    enum buffer : uint8_t {
        uniform=0,
        storage=1,
        combined_image_sampler=2,
        sampled_image=3
    };

    enum shader_stage {
        undefined=-1,
        vertex=0,
        fragment=1
    };

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

    enum class image_layout : uint8_t {
        undefined=0,                                  // VK_IMAGE_LAYOUT_UNDEFINED
        general=1,                                    // VK_IMAGE_LAYOUT_GENERAL
        color_optimal=2,                              // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        depth_stencil_optimal=3,                      // VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        depth_stencil_read_only_optimal=4,            // VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_READ_ONLY_OPTIMAL
    };

    //! @brief Equivalent to doing VkSampleCountFlagBits but simplified
    enum class sample_bit : uint8_t {
        count_1,
        count_2,
        count_4,
        count_8,
        count_16,
        count_32,
        count_64,
        max_enum
    };

    //! @brief Equivalent to VkAttachmentLoadOp
    enum class attachment_load : uint8_t {
        load=0,                     // LOAD_OP_LOAD
        clear,                      // LOAD_OP_CLEAR
        dont_care,                  // lOAD_OP_DONT_CARE
        none_khr,                   // LOAD_OP_NONE_KHR
        none_ext,                   // LOAD_OP_NONE_EXT
        max_enum,                   // LOAD_OP_MAX_ENUM
    };

    //! @brief Equivalent to VkAttachmentStoreOp
    enum class attachment_store : uint8_t {
        store=0,                    // STORE_OP_STORE
        dont_care,                  // STORE_OP_CLEAR
        none_khr,                   // STORE_OP_NONE
        none_qcom,                  // STORE_OP_NONE_EXT
        none_ext,                   // STORE_OP_NONE_KHR
        max_enum,                   // STORE_OP_MAX_ENUM
    };

    //! @brief Equivalent to VkPipelineBindPoint
    enum class pipeline_bind_point : uint8_t {
        graphics=0,                 // VK_PIPELINE_BIND_POINT_GRAPHICS
        compute=1,                  // VK_PIPELINE_BIND_POINT_COMPUTE
        ray_tracing_khr,            // VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR
        subpass_shading_hauwei,     // VK_PIPELINE_BIND_POINT_SUBPASS_SHADING_HUAWEI
        ray_tracing_nv,             // VK_PIPELINE_BIND_POINT_RAY_TRACING_NV
        max_enum                    // VK_PIPELINE_BIND_POINT_MAX_ENUM
    };

    //! @brief this is the expectation of specifying the renderpass attachments
    // TODO: Find a way to differentiate the different VkAttachmentReference specifications of color and depth attachments that get referenced
    /*
    
        std::array<renderpass_attachment, 2> renderpass_attachments = {
            renderpass_attachment{ // this sets up the VkAttachmentDescription and VkAttachmentReference
                .layout = image_layout::color_optimal,
                .format = surface_format,
                .sample_count = sample_bit::count_1,
                .load = attachment_load::load,
                .store = attachment_store::store,
                .stencil_load = attachment_load::load,
                .stencil_store = attachment_store::store,
                .initial = image_layout::undefined,
                .finalize = image_layout::present_src
            },
            renderpass_attachment{
                .layout = image_layout::depth_optimal, // this sets up the VkAttachmentDescription and VkAttachmentReference
                .format = depth_format,
                .sample_count = sample_bit::count_1,
                .load = attachment_load::load,
                .store = attachment_store::store,
                .stencil_load = attachment_load::load,
                .stencil_store = attachment_store::store,
                .initial = image_layout::undefined,
                .finalize = image_layout::present_src
            }
        };
    
    */

    enum class renderpass_type : uint8_t {
        color,
        depth
    };

    struct renderpass_attachment {
        renderpass_type type;
        image_layout layout;
        format format;
        sample_bit sample_count;
        attachment_load load;
        attachment_store store;
        attachment_load stencil_load;
        attachment_store stencil_store;
        image_layout initial;  // initial starting layout
        image_layout finalize; // final layout
    };

    /**
     * @brief Renderpass specifictations for VkRenderpass
     * 
     * Specifying requirements needed for specifying the vulkan renderpass handlers
    */
    struct vk_renderpass_options {
        bool cache=false; // set this to true if you want to use the default constructed configuration
        std::span<VkClearColorValue> clear_values{};
        std::span<VkAttachmentDescription> attachments{};
        std::span<VkSubpassDescription> subpass_descriptions{};
        std::span<VkSubpassDependency> dependencies{};
    };

    struct renderpass_options {
        std::span<VkClearColorValue> clear_values{};
        std::span<renderpass_attachment> color_attachments{};
        std::span<renderpass_attachment> depth_attachments{};
    };

    //! @brief vulkan buffer struct to define the handlers and memory specifications required for buffer handlers in vulkan
    struct vk_buffer {
        VkBuffer handler = nullptr;
        VkDeviceMemory device_memory = nullptr;
        uint32_t allocation_size = 0; // device allocation size
    };

    //! @brief Specifications of the vulkan buffer handlers and the use and memory bits associated with the buffer handlers
    struct vk_buffer_info {
        uint32_t device_size=0;
        VkBufferUsageFlags usage;
        VkMemoryPropertyFlags memory_property_flag;
    };

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

    struct texture_properties {
        uint32_t width;
        uint32_t height;
        VkImageUsageFlagBits usage;
        VkMemoryPropertyFlagBits property;
        VkFormat format;
    };


    struct descriptor_binding_point {
        uint32_t binding;
        shader_stage stage;
    };

    struct descriptor_binding_entry {
        buffer type;
        descriptor_binding_point binding_point;
        uint32_t descriptor_count;
    };


    struct descriptor_set_layout {
        uint32_t allocate_count=0;
        uint32_t max_sets=0;
        uint32_t size_bytes=0;
        std::span<descriptor_binding_entry> entry;
    };

    struct vertex_input {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normals;
        glm::vec2 uv;

        bool operator==(const vertex_input& other) const {
            return position == other.position and color == other.color and uv == other.uv and normals == other.normals;
        }
    };


    //! @brief Going to remove this
    //! @brief This is being used by descriptor sets and vk_renderer
    //! TODO: Modify vk_descriptor_set and vk_renderer and add uniforms.hpp inside renderer/ (dir) for used across vk_renderer and the renderer
    struct camera_ubo {
        glm::mat4 projection{ 1.f };
        glm::mat4 view{ 1.f };
    };

    //! @brief Just for testing purposes for sending this struct over to the shader
    struct global_ubo {
        glm::mat4 mvp={1.f};
    };

    /**
     * @brief material is going to define properties about how a scene object itself gets rendered
     * 
    */
    struct material_uniform {
        glm::mat4 model{1.f};
        glm::vec4 color{1.f};
        // std::vector<std::string> texture_paths;
    };

};