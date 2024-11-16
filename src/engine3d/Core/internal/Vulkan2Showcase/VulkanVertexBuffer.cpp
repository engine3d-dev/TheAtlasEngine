#include <internal/Vulkan2Showcase/VulkanContext.hpp>
#include <internal/Vulkan2Showcase/helper_functions.hpp>
#include <internal/Vulkan2Showcase/VulkanVertexBuffer.hpp>
#include <Core/ApplicationInstance.hpp>
#include <Core/EngineLogger.hpp>

namespace engine3d::vk{
    // VulkanVertexBuffer::VulkanVertexBuffer(VkBuffer p_Buffer, VkDeviceMemory p_Memory, size_t p_Size, const std::vector<Vertex>& p_Triangle, std::vector<float> p_MappedData){
    VulkanVertexBuffer::VulkanVertexBuffer(const std::vector<Vertex>& p_Triangle){
        ConsoleLogError("Vulkan2Showcase: Begin Vulkan Vertex Buffer Initializing");
        m_VertexBuffer.Size = sizeof(Vertex) * 3;
        auto triangle = p_Triangle;
        Vertex triangles[3];
        triangle[0] = { {-0.5f, -0.288f, 0.0f, 1.0f}, {0.0f, 0.0f} };
        triangle[1] = { {.5f, -.288f, 0.0f, 1.0f}, {1.0f, 0.0f} };
        triangle[2] = { {0.0f, .577f, 0.0f, 1.0f}, {0.5f, 1.0f} };
        vkGetPhysicalDeviceMemoryProperties(VulkanContext::GetPhysicalDriver(), &m_PhysicalDeviceMemoryProperties);

        initialize_vertex_buffeR(m_VertexBuffer, reinterpret_cast<uint8_t *>(triangles), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        // Vertex triangle[3];
        // triangle[0] = { {-0.5f, -0.288f, 0.0f, 1.0f}, {0.0f, 0.0f} };
        // triangle[1] = { {.5f, -.288f, 0.0f, 1.0f}, {1.0f, 0.0f} };
        // triangle[2] = { {0.0f, .577f, 0.0f, 1.0f}, {0.5f, 1.0f} };
        // std::vector<Vertex> triangle = p_Triangle;
        // initialize_vertex_buffeR(reinterpret_cast<uint8_t*>(triangle.data()), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        // //! @note Initiate Uniform BUffers
        // ConsoleLogInfo("Vulkan2Showcase: Vulkan Vertex Buffer Initializing Uniform Buffers (In this implementation)");
        
        // VkPhysicalDeviceProperties device_properties;
        // vkGetPhysicalDeviceProperties(VulkanContext::GetPhysicalDevice(), &device_properties);
        // size_t min_uniform_alignment = static_cast<size_t>(device_properties.limits.minUniformBufferOffsetAlignment);

        // VkBufferUsageFlags  usage_flags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        // size_t buffer_data_size_per_swapchain = sizeof(float) * 4 * 4;
        // buffer_data_size_per_swapchain = static_cast<uint32_t>(AlignedDataSize(buffer_data_size_per_swapchain, min_uniform_alignment));

        // m_DynamicUniformBuffer.Size = buffer_data_size_per_swapchain * ApplicationInstance::GetWindow().GetSwapchainImagesSize();

        // initialize_vertex_buffeR(reinterpret_cast<uint8_t*>(m_DynamicUniformBuffer.MappedData), usage_flags);

        // m_DynamicUniformBuffer.BufferInfo.range = buffer_data_size_per_swapchain;
        // vk_check(vkMapMemory(VulkanContext::GetDriver(), m_DynamicUniformBuffer.DeviceMemory, 0, m_DynamicUniformBuffer.Size, 0, &m_DynamicUniformBuffer.MappedData), "vkMapMemory", __FILE__, __LINE__, __FUNCTION__);

        // ConsoleLogWarn("Vulkan2Showcase: Vulkan Uniform Buffers Initialized Completed!!!");


        //! @note Create Descriptor Sets.


        ConsoleLogWarn("Vulkan2Showcase: Vulkan Vertex Buffer Initialized Completed");
    }

    void VulkanVertexBuffer::initialize_vertex_buffeR(BufferData& p_BufferData, const uint8_t* p_Data, const VkBufferUsageFlags& p_Usage){
        /*//! @note Declare and populate VkBuffer Info.
        VkBufferCreateInfo buffer_create_info = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = m_Size,
            .usage = p_Usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices = nullptr
        };

        //! @note Creating buffer object itself.
        vk_check(vkCreateBuffer(VulkanContext::GetDriver(), &buffer_create_info, nullptr, &m_Buffer), "vkCreateBuffer", __FILE__, __LINE__, __FUNCTION__);

        // Defining struct for holding memory requirements of the buffer.
        VkMemoryRequirements memory_requirements;

        //! @note Extracting allocation info structs.
        VkMemoryAllocateInfo alloc_info = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = nullptr,
            .allocationSize = memory_requirements.size,
            .memoryTypeIndex = 0,
        };

        //! @note Checking if memory is about to be in-use supports necessary flags of the buffer.
        bool pass = SelectMemoryTypeProperties(m_PhysicalDeviceMemoryProperties, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &alloc_info.memoryTypeIndex);

        if(pass){
            vk_check(vkAllocateMemory(VulkanContext::GetDriver(), &alloc_info, nullptr, &(m_Memory)), "vkAllocateMemory", __FILE__, __LINE__, __FUNCTION__);
            m_MemoryPropertyFlags = m_PhysicalDeviceMemoryProperties.memoryTypes[alloc_info.memoryTypeIndex].propertyFlags;

            //! @note Saving data to the buffer.
            m_BufferInfo.range = memory_requirements.size / ApplicationInstance::GetWindow().GetSwapchainImagesSize();
            m_BufferInfo.offset = 0;
            m_BufferInfo.buffer = m_Buffer;
        }

        //! @note ptr to pass data to buffer.
        uint32_t* data;

        vk_check(vkAllocateMemory(VulkanContext::GetDriver(), &alloc_info, nullptr, &(m_Memory)), "vkAllocateMemory", __FILE__, __LINE__, __FUNCTION__);


        //! @note Save data to buffer struct.
        m_BufferInfo.range = memory_requirements.size;
        m_BufferInfo.offset = 0;
        m_BufferInfo.buffer = m_Buffer;

        VkMemoryPropertyFlags flags = m_PhysicalDeviceMemoryProperties.memoryTypes[alloc_info.memoryTypeIndex].propertyFlags;
        m_MemoryPropertyFlags = flags;

        if(p_Data != nullptr){
            // Mapping data to memory.
            vk_check(vkMapMemory(VulkanContext::GetDriver(), m_Memory, 0, m_Size, 0, reinterpret_cast<void**>(&data)), "vkMapMemory",__FILE__, __LINE__, __FUNCTION__);
            memcpy(data, p_Data, m_Size);

            VkMappedMemoryRange map_mem_range = {
                .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
                .pNext = nullptr,
                .memory = m_Memory,
                .size = m_Size
            };

            // Only flush memory if VK_MEMORY_PROPERTY_HOST_COHERENT_BIT not supported.
            if(!(flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)){
                vk_check(vkFlushMappedMemoryRanges(VulkanContext::GetDriver(), 1, &map_mem_range), "vkFlushMappedMemoryRanges", __FILE__, __LINE__, __FUNCTION__);
            }

            vk_check(vkBindBufferMemory(VulkanContext::GetDriver(), m_Buffer, m_Memory, 0), "vkBindBufferMemory", __FILE__, __LINE__, __FUNCTION__);
        }
        */

        VkBufferCreateInfo buffer_create_info = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = p_BufferData.Size,
            .usage = p_Usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices = nullptr
        };

        //! @note Initializing the buffer itself.
        vk_check(vkCreateBuffer(VulkanContext::GetDriver(), &buffer_create_info, nullptr, &p_BufferData.Buffer), "vkCreateBuffer", __FILE__, __LINE__, __FUNCTION__);

        // ! @note Extracts the mem requirements for our vertex buffer.

        VkMemoryRequirements memory_requirements;
        vkGetBufferMemoryRequirements(VulkanContext::GetDriver(), p_BufferData.Buffer, &memory_requirements);

        // inserting data for allocation
        VkMemoryAllocateInfo alloc_info = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = nullptr,
            .allocationSize = memory_requirements.size,
            .memoryTypeIndex = 0,
        };

        bool pass = SelectMemoryTypeProperties(m_PhysicalDeviceMemoryProperties, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &(alloc_info.memoryTypeIndex));

        if(pass){
            uint8_t* data;

            vk_check(vkAllocateMemory(VulkanContext::GetDriver(), &alloc_info, nullptr, &(p_BufferData.DeviceMemory)), "vkAllocateMemory", __FILE__, __LINE__, __FUNCTION__);

            //! @note Saving data buffer.
            // vk_check(vkAllocateMemory(VulkanContext::GetDriver(), &alloc_info, nullptr, &(p_BufferData.DeviceMemory)), "vkAllocateMemory", __FILE__, __LINE__, __FUNCTION__);

            p_BufferData.BufferInfo.range = memory_requirements.size;
            p_BufferData.BufferInfo.offset = 0;
            p_BufferData.BufferInfo.buffer = p_BufferData.Buffer;

            VkMemoryPropertyFlags flags = m_PhysicalDeviceMemoryProperties.memoryTypes[alloc_info.memoryTypeIndex].propertyFlags;
            p_BufferData.MemoryPropertyFlags = flags;

            if(data != nullptr){
                vk_check(vkMapMemory(VulkanContext::GetDriver(), p_BufferData.DeviceMemory, 0, p_BufferData.Size, 0, reinterpret_cast<void**>(&data)), "vkMapMemory", __FILE__, __LINE__, __FUNCTION__);
                // vk_check(vkMapMemory(VulkanContext::GetDriver(), p_BufferData.DeviceMemory, 0, p_BufferData.Size, 0, reinterpret_cast<void**>(&data)), "vkMapMemory", __FILE__, __LINE__, __FUNCTION__);
                // memcpy(data, p_Data, p_BufferData.Size);


                // VkMappedMemoryRange map_mem_range = {
                //     .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
                //     .pNext = nullptr,
                //     .offset = 0,
                //     .size = p_BufferData.Size
                // };

                // if(!(flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)){
                //     vk_check(vkFlushMappedMemoryRanges(VulkanContext::GetDriver(), 1, &map_mem_range), "vkFlushMappedMemoryRanges", __FILE__, __LINE__, __FUNCTION__);
                // }
            }

        //     vk_check(vkBindBufferMemory(VulkanContext::GetDriver(), p_BufferData.Buffer, p_BufferData.DeviceMemory, 0), "vkBindBuffermemory", __FILE__, __LINE__, __FUNCTION__);
        }
    }

    bool VulkanVertexBuffer::SelectMemoryTypeProperties(const VkPhysicalDeviceMemoryProperties& p_MemoryProperties, uint32_t typeBits, VkFlags requirements_mask, uint32_t* typeIndex){
        for(uint32_t i = 0; i < p_MemoryProperties.memoryTypeCount; i++){
            if((typeBits & 1) == 1){
                //! @note Type available, does it match user properties?
                if((p_MemoryProperties.memoryTypes[i].propertyFlags &requirements_mask) == requirements_mask){
                    *typeIndex = i;
                    return true;
                }
            }

            typeBits >>= 1;
        }

        return false;
    
    }

    size_t VulkanVertexBuffer::AlignedDataSize(size_t dataSize, size_t minimumAlignment){
        return (dataSize / minimumAlignment) * minimumAlignment + ((dataSize % minimumAlignment) > 0 ? minimumAlignment : 0);
    }

};