#pragma once
#include <vulkan/vulkan.h>
#include <span>
#include <glm/glm.hpp>

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


    struct vk_image_handler {
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

    /**
     * @param allocate_count
     * @brief count of descriptor set layouts to allocate for this descriptor set
     * 
     * @param size_bytes
     * @brief Size of bytes of uniforms to allocate for the descriptor sets
     * 
     * @param max_sets
     * @brief maximum of descriptor sets that can be allocated
     * 
    */
    // struct descriptor_set_layout {
    //     uint32_t allocate_count=0;
    //     uint32_t max_sets=0;
    //     uint32_t size_bytes=0;
    //     std::span<VkDescriptorPoolSize> allocation_info;
    //     std::span<VkDescriptorSetLayoutBinding> bindings;
    // };

    enum class format : uint64_t {
        rgb32_sfloat, // Represent R32G32B32_SFLOAT
        rg32_sfloat, // Represent R32G32_SFLOAT
    };

    enum buffer : uint8_t {
        uniform=0,
        storage=1,
        image_sampler=2
    };

    enum shader_stage {
        undefined=-1,
        vertex=0,
        fragment=1
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
        glm::mat4 Projection{ 1.f };
        glm::mat4 View{ 1.f };
        glm::mat4 Model{ 1.f };
    };

    //! @brief Just for testing purposes for sending this struct over to the shader
    struct global_ubo {
        glm::mat4 mvp={1.f};
    };

    /**
     * @brief material is going to define properties about how a scene object itself gets rendered
     * 
    */
    struct material {
        glm::mat4 model={1.f};
        glm::vec4 color{1.f};
        // std::vector<std::string> texture_paths;
    };

};