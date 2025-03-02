#include "drivers/vulkan/helper_functions.hpp"
#include "drivers/vulkan/vulkan_context.hpp"
#include <cstdint>
#include <drivers/vulkan/buffer_tutorial.hpp>
#include <vulkan/vulkan_core.h>


namespace atlas::vk {
    static uint64_t get_alignment(uint64_t p_size,
                                  uint64_t p_min_offset_alignment) {
        if (p_min_offset_alignment > 0) {
            return (p_size + p_min_offset_alignment - 1) &
                   ~(p_min_offset_alignment - 1);
        }

        return p_size;
    }

    BufferTutorial::BufferTutorial(
      uint64_t p_size,
      uint32_t p_count,
      VkBufferUsageFlags p_usage,
      VkMemoryPropertyFlags p_memory_property_flags,
      uint64_t p_min_offset_alignment)
      : m_count(p_count)
      , m_instances_size(p_size)
      , m_usage(p_usage)
      , m_memory_property_flags(p_memory_property_flags) {
        m_alignment_size =
          get_alignment(m_instances_size, p_min_offset_alignment);
        m_size = m_alignment_size * m_count;

        //! @note Creating buffer
        // device.createBuffer(bufferSize, usageFlags, memoryPropertyFlags,
        // buffer, memory);
        VkBufferCreateInfo create_info = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = m_size,
            .usage = m_usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        // Creating buffer before getting memory requirements
        vk_check(vkCreateBuffer(vk_context::get_current_driver(),
                                &create_info,
                                nullptr,
                                &m_buffer_handler),
                 "vkCreateBuffer",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);

        VkMemoryRequirements memory_requirements;
        vkGetBufferMemoryRequirements(vk_context::get_current_driver(),
                                      m_buffer_handler,
                                      &memory_requirements);

        VkMemoryAllocateInfo alloc_info = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = nullptr,
            .allocationSize = memory_requirements.size,
            .memoryTypeIndex =
              vk_context::get_current_selected_physical_driver()
                .search_memory_type(memory_requirements.memoryTypeBits,
                                    p_memory_property_flags)
        };

        vk_check(vkAllocateMemory(vk_context::get_current_driver(),
                                  &alloc_info,
                                  nullptr,
                                  &m_buffer_device_memory),
                 "vkAllocateMemory",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);
    }

    void BufferTutorial::map(VkDeviceSize p_size, uint64_t p_offset) {
        vk_check(vkMapMemory(vk_context::get_current_driver(),
                             m_buffer_device_memory,
                             p_offset,
                             p_size,
                             0,
                             &m_mapped),
                 "vkMapMemory",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);
    }

    void BufferTutorial::unmap() {
        if (m_buffer_device_memory) {
            vkUnmapMemory(vk_context::get_current_driver(),
                          m_buffer_device_memory);
        }
    }

    void BufferTutorial::write_to_buffer(void* p_data,
                                         uint64_t p_size,
                                         uint64_t p_offset) {
        if (p_size == VK_WHOLE_SIZE) {
            memcpy(m_mapped, p_data, m_size);
        }
        else {
            char* mem_offset = (char*)m_mapped;
            mem_offset += p_offset;
            memcpy(mem_offset, p_data, p_size);
        }
    }

    void BufferTutorial::flush(uint64_t p_size, uint64_t offset) {
        VkMappedMemoryRange mapped_mem_range = {
            .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
            .pNext = nullptr,
            .memory = m_buffer_device_memory,
            .offset = offset,
            .size = p_size
        };

        vk_check(vkFlushMappedMemoryRanges(
                   vk_context::get_current_driver(), 1, &mapped_mem_range),
                 "vkFlushMappedMemoryRanges",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);
    }

    VkDescriptorBufferInfo BufferTutorial::descriptor_info(uint64_t p_size,
                                                           uint64_t p_offset) {
        return VkDescriptorBufferInfo{ .buffer = m_buffer_handler,
                                       .offset = p_offset,
                                       .range = p_size };
    }

    void BufferTutorial::invalidate(uint64_t p_size, uint64_t p_offset) {
        VkMappedMemoryRange mapped_mem_range = {
            .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
            .memory = m_buffer_device_memory,
            .offset = p_offset,
            .size = p_size
        };

        vk_check(vkInvalidateMappedMemoryRanges(
                   vk_context::get_current_driver(), 1, &mapped_mem_range),
                 "vkInvalidateMappedMemoryRanges",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__);
    }

    void BufferTutorial::write_to_index(void* p_data, int p_index) {
        write_to_buffer(p_data, m_instances_size, p_index * m_alignment_size);
    }

    void BufferTutorial::flush_index(int p_index) {
        flush(m_alignment_size, p_index * m_alignment_size);
    }

    VkDescriptorBufferInfo BufferTutorial::descriptor_info_at_index(
      int p_index) {
        return descriptor_info(m_alignment_size, p_index * m_alignment_size);
    }

    void BufferTutorial::invalidate_index(int p_index) {
        invalidate(m_alignment_size, p_index * m_alignment_size);
    }

};