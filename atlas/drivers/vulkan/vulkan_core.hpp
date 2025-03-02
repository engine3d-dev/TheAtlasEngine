#pragma once
#include <span>
#include <type_traits>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>


namespace atlas::vk {

    //! @note This just checks the result type that is invoked by the passed
    //! callable object is valid
    //! @note If the returned result is invalid then there will be a compiled
    //! error
    //! @note Used in the submit function
    template<typename Type>
    concept ReturnInvoke = std::is_same_v<std::span<VkCommandBuffer>, Type>;

    /**
     * @name CommandbufferBeginInfo
     * @note Operation to indicate when a command buffer can start to record
     * @note Creates a complelety new command buffer and indicating to start
     * recording to that new command buffer
     * @note Use to creating a command buffer more easier
     * TODO: Depends on how this go, may change this later
     */
    VkCommandBuffer create_command_buffer_and_record(
      const VkDevice& p_driver,
      const VkCommandPool& p_command_buffer);

    /**
     * @name BeginCommandBuffer
     * @note Enables you to begin recording the command buffer excluding
     * actually specifying the structure VkBeginInfo
     * @note Minimize boilerplate to record command buffers
     */
    void begin_command_buffer(const VkCommandBuffer& p_command_buffer);

    /**
     * @name End Command Buffer
     * @note Operation to signal command buffer to stop recording commands
     */
    void end_command_buffer(const VkCommandBuffer& p_command_buffer);

    /**
     * @name Submit
     * @note This is the vulkan operation to submit commands directly to the GPU
     * @note This operation is to minimize the specific boilerplate that comes
     * with vulkan
     * @param VkQueue is the queue the user can specify to submit commands to
     * use
     * @param p_Commands is the span of commands that will be submitted to the
     * gpu
     * @note Typically the swapchain contains the information what queue to
     * submit to
     * @note swapchain also knows
     */
    void submit(VkQueue p_queue_to_use,
                const std::span<VkCommandBuffer>& p_command_buffers);

    /**
     * @name Submit
     * @note Handles direct operation to submit commands to directly to the gpu
     * @note UCallableTask must return a valid structure that is valid to
     * std::span<VkCommandBuffer>
     * @param VkQueue passing the queue to submit the command buffer to
     * @param UCallableTask defines a lambda function that is used to specify a
     * large operation that requires more then just passing the command buffer
     * itself
     * @note Usage of this is when we need to handle specific tasks just before
     * we directly submit those commands
     */
    template<typename UCallableTask>
        requires ReturnInvoke<std::invoke_result_t<UCallableTask>>
    void Submit(VkQueue p_queue_to_use, const UCallableTask& p_callable) {
        auto commands = p_callable();

        VkSubmitInfo submit_info = { .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                                     .commandBufferCount =
                                       static_cast<uint32_t>(commands.size()),
                                     .pCommandBuffers = commands.data() };

        vkQueueSubmit(p_queue_to_use,
                      static_cast<uint32_t>(commands.size()),
                      &submit_info,
                      VK_NULL_HANDLE);
        vkQueueWaitIdle(p_queue_to_use);
    }

    VkImage create_image(VkFormat p_format,
                         uint32_t p_width,
                         uint32_t p_height);
    VkImageView create_image_view(VkImage p_image, VkFormat p_format);

    VkCommandBuffer begin_single_time_command_buffer(
      const VkCommandPool& p_command_pool);
    void end_single_time_command_buffer(const VkCommandBuffer& p_command_buffer,
                                        const VkCommandPool& p_command_pool);

    VkCommandPool create_command_pool();

    void pipeline_image_barrier(VkCommandBuffer p_command_buffer,
                                VkImage Image,
                                VkAccessFlags srcAccessmask,
                                VkAccessFlags dstAccessMask,
                                VkImageLayout OldImageLayout,
                                VkImageLayout NewImageLayout,
                                VkPipelineStageFlags srcStageFlags,
                                VkPipelineStageFlags dstStageFlags,
                                VkImageSubresourceRange subresourceRange);
};