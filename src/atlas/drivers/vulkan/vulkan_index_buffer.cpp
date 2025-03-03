#include <core/engine_logger.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <drivers/vulkan/vulkan_index_buffer.hpp>

namespace atlas::vk {
    vk_index_buffer::vk_index_buffer(const std::vector<uint32_t>& p_Indices) {
        // console_log_info("Vulkan2Showcase: Begin Vulkan vertex_buffer
        // Initialization!!");
        console_log_warn_tagged("vulkan",
                                "Begin Vulkan vertex_buffer Initialization!!");
        m_indices_count = static_cast<uint32_t>(p_Indices.size());

        m_has_indices = (m_indices_count > 0);

        if (!m_has_indices) {
            console_log_warn(
              "Cannot provide empty indices to the vk_index_buffer!");
            return;
        }

        VkDeviceSize buffer_size = sizeof(p_Indices[0]) * m_indices_count;
        VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        VkMemoryPropertyFlags property_flags =
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        VkBufferCreateInfo vk_buffer_create_info = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .size = buffer_size,
            .usage = usage_flags,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        vk_check(vkCreateBuffer(vk_context::get_current_driver(),
                                &vk_buffer_create_info,
                                nullptr,
                                &m_index_buffer_handler),
                 "vkCreateBuffer",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        VkMemoryRequirements memory_reqs;
        vkGetBufferMemoryRequirements(vk_context::get_current_driver(),
                                      m_index_buffer_handler,
                                      &memory_reqs);

        VkMemoryAllocateInfo alloc_info = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memory_reqs.size,
            .memoryTypeIndex =
              vk_context::get_current_driver().select_memory_type(
                memory_reqs.memoryTypeBits, property_flags)
        };

        vk_check(vkAllocateMemory(vk_context::get_current_driver(),
                                  &alloc_info,
                                  nullptr,
                                  &m_index_device_buffer_memory),
                 "vkAllocateMemory",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        vkBindBufferMemory(vk_context::get_current_driver(),
                           m_index_buffer_handler,
                           m_index_device_buffer_memory,
                           0);

        //! @note Mapping memory data.
        //! @note THIS is how we map our vertices data to our VkBuffer
        //! (essentially it is our vertex buffer)

        /**
            @note Purpose of these buffer objects are to write these information
           to memory.
            @note This maps memory on our host to the device's memory.
            @note Where this creates a data region on the host, that is linked
           to the device memory
            @note This way when we used memcpy to write to the CPU's accessible
           memory. That information will also be flushed to the GPU's memory, by
           this link
            @note Then once this information has been mapped, then we no longer
           need the data on the host's side, therefore unmapping that
           information.
        */
        void* data;
        vkMapMemory(vk_context::get_current_driver(),
                    m_index_device_buffer_memory,
                    0,
                    buffer_size,
                    0,
                    &data);
        memcpy(data, p_Indices.data(), static_cast<size_t>(buffer_size));
        vkUnmapMemory(vk_context::get_current_driver(),
                      m_index_device_buffer_memory);

        // console_log_warn("Vulkan2Showcase: Vulkan vertex_buffer Initialized
        // Completed!!!");
        console_log_warn_tagged(
          "vulkan", "Vulkan vertex_buffer Initialized Completed!!!");
    }

    vk_index_buffer::~vk_index_buffer() {}

    void vk_index_buffer::bind_to_index_buffer(
      const VkCommandBuffer& p_CommandBuffer) {
        // VkBuffer buffers[] = {m_index_buffer_handler};
        // VkDeviceSize offsets[] = {0};

        if (m_has_indices) {
            vkCmdBindIndexBuffer(
              p_CommandBuffer, m_index_buffer_handler, 0, VK_INDEX_TYPE_UINT32);
        }
    }

    void vk_index_buffer::render_index_buffer(
      const VkCommandBuffer& p_CommandBuffer) {
        /**
        //! @note This will happen within Renderer itself, but not here nor
        vertex_buffer class. if(m_has_indices){
            vkCmdDrawIndexed(p_CommandBuffer, m_indices_count, 1, 0, 0, 0);
        }
        else{
            vkCmdDraw(p_CommandBuffer, m_VerticesCount, 1, 0, 0);
        }
        */

        vkCmdDrawIndexed(p_CommandBuffer, m_indices_count, 1, 0, 0, 0);
    }

    bool vk_index_buffer::contains_indices() const {
        return m_has_indices;
    }
};