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
        Primary = 0,
        Secondary = 1,
        MaxEnum = 2
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

    struct vk_image {
        VkImage image = nullptr;
        VkImageView image_view = nullptr;
        VkSampler sampler = nullptr;
        VkDeviceMemory device_memory = nullptr;
    };

    //! @brief contains the min and max ranges for the VkFilter
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
     * @brief Renderpass Specifications
     * 
     * These specifications are for configuring the attachments and descriptions of the renderpass being created
    */
    struct vk_renderpass_options {
        bool cache=false; // set this to true if you want to use the default constructed configuration
        std::span<VkClearColorValue> clear_values{};
        std::span<VkAttachmentDescription> attachments{};
        std::span<VkSubpassDescription> subpass_descriptions{};
        std::span<VkSubpassDependency> dependencies{};
    };

    struct vk_buffer {
        VkBuffer handler = nullptr;
        VkDeviceMemory device_memory = nullptr;
        uint32_t allocation_size = 0; // device allocation size
    };

    struct vk_buffer_info {
        uint32_t device_size=0;
        VkBufferUsageFlags usage;
        VkMemoryPropertyFlags memory_property_flag;
    };

    struct vertex {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normals;
        glm::vec2 uv;

        bool operator==(const vertex& other) const {
            return position == other.position and color == other.color and uv == other.uv and normals == other.normals;
        }
    };

    

    enum shader_stage {
        Undefined=-1,
        Vertex=0,
        Fragment=1
    };


    //! @brief Going to remove this
    //! @brief This is being used by descriptor sets and vk_renderer
    //! TODO: Modify vk_descriptor_set and vk_renderer and add uniforms.hpp inside renderer/ (dir) for used across vk_renderer and the renderer
    struct camera_ubo {
        glm::mat4 Projection{ 1.f };
        glm::mat4 View{ 1.f };
        glm::mat4 Model{ 1.f };
    };

};