#include <numeric>
#include <drivers/vulkan/vulkan_renderer.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <atomic>
#include <drivers/vulkan/vulkan_context.hpp>
#include <drivers/vulkan/helper_functions.hpp>
#include <core/application_instance.hpp>
#include <core/engine_logger.hpp>
#include <drivers/vulkan/vulkan_swapchain.hpp>

namespace engine3d::vk{
    static std::vector<VkCommandBuffer> g_CommandBuffers;
    VkCommandPool g_CommandPool;
    std::atomic<uint32_t> g_CurrentFrameIndex = -1;
    static bool g_IsFrameStarted = false;
    static VkPipelineLayout g_PipelineLayout;
    static VkPipeline g_Pipeline;

    void VulkanRenderer::InitializeRendererPipeline(){
        VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .setLayoutCount = 0,
            .pSetLayouts = nullptr,
            // .setLayoutCount = static_cast<uint32_t>(g_DescriptorSetLayoutVector.size()),
            // .pSetLayouts = g_DescriptorSetLayoutVector.data(),
            // .pushConstantRangeCount = 1,
            // .pPushConstantRanges = &push_const_range
        };

        vk::vk_check(vkCreatePipelineLayout(VulkanContext::GetDriver(), &pipeline_layout_create_info, nullptr, &g_PipelineLayout), "vkCreatePipelineLayout", __FILE__, __LINE__, __FUNCTION__);
        
        // *******************************************************
        // ****************[End of Pipeline Layout]**************
        // *******************************************************

        //! @note We are setting our shader pipeline to utilize our current window's swapchain
        //! @note a TODO is to utilize different render passes utiization for shader pipelines, potentially.
        //! @note [NOTES] -> These two lines below are for the shader::configurations.
        // pipeline_config.PipelineRenderPass = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetRenderPass();
        // pipeline_config.PipelineLayout = g_PipelineLayout;

        g_CommandBuffers.resize(ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetImagesSize());
        VkCommandPoolCreateInfo pool_create_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = VulkanContext::GetPhysicalDriver().GetQueueIndices().Graphics
        };

        vk_check(vkCreateCommandPool(VulkanContext::GetDriver(), &pool_create_info, nullptr, &g_CommandPool), "vkCreateCommandPool", __FILE__, __LINE__, __FUNCTION__);


        ConsoleLogInfo("RENDERER COMMAND BUFFERS SIZE === {}", g_CommandBuffers.size());
        //! @note Allocating our command buffers.
        VkCommandBufferAllocateInfo cmd_buffer_alloc_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = g_CommandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = static_cast<uint32_t>(g_CommandBuffers.size()),
        };

        vk_check(vkAllocateCommandBuffers(VulkanContext::GetDriver(), &cmd_buffer_alloc_info, g_CommandBuffers.data()), "vkAllocateCommandBuffers", __FILE__, __LINE__, __FUNCTION__);

        //! @note Setting up Uniform Buffers here
        //! TODO: Move uniform buffers...
        //! @note Bandage fix for NonCoherentAtomSize bug.


        auto phys_driver_props = vk::VulkanContext::GetPhysicalDriver().GetProperties();
        auto min_offset_alignment = std::lcm(phys_driver_props.limits.minUniformBufferOffsetAlignment, phys_driver_props.limits.nonCoherentAtomSize);
        //! @note We want their to be the same amount of uniform buffers as our current frames in flight.
        //! @note Uniform buffers are how we are going to be sending data to our pipelines.
        ConsoleLogInfo("Renderer --- Begin Initializing Uniform Buffers at Max_Frames_In_Flight = {}", VulkanSwapchain::MaxFramesInFlight);
        // g_UniformBuffers = std::vector<UniformBuffer>(vk::VulkanSwapchain::MaxFramesInFlight);
        // ConsoleLogTrace("g_UniformBuffers.size() === {}", g_UniformBuffers.size());
        // uint32_t instance_count = vk::VulkanSwapchain::MaxFramesInFlight;
        uint32_t instance_count = 1;
        // for(int i = 0; i <= g_UniformBuffers.size(); i++){
        //     g_UniformBuffers[i] = UniformBuffer(
        //         sizeof(GlobalUbo),
        //         instance_count,
        //         VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        //         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        //         min_offset_alignment
        //     );
            
        //     //! @note In tutorial referenced to as .map(), I just changed it to be explicitly called MapData
        //     g_UniformBuffers[i].MapData();
        // }

        ConsoleLogWarn("Renderer --- g_UniformBuffers Initialized Correctly!!");
        
        // g_Global_desc_sets = std::vector<VkDescriptorSet>(vk::VulkanSwapchain::MaxFramesInFlight);

        // for(int i = 0; i <= g_Global_desc_sets.size(); i++){
        //     auto buffer_info = g_UniformBuffers[i].InitializeDescriptorInfo();
        //     vk::VulkanDescriptorWriter(*glob_set_layout, *g_GlobalPool)
        //     .WriteBuffer(0, &buffer_info)
        //     .Build(g_Global_desc_sets[i]);
        // }

        ConsoleLogInfo("CommandBuffers Size === {}", g_CommandBuffers.size());
    }

    VulkanRenderer::VulkanRenderer(const std::string& p_Tag){
        g_CurrentFrameIndex = 0;
        g_IsFrameStarted = false;
        InitializeRendererPipeline();
    }

    void VulkanRenderer::BeginFrame(){
        //! @note Note to Self: This is just a hack on how to get "resizing" to work
        //! @note I will need to do this better as there are problems with this.
        //! @note Currently the state is if the window is resized by glfwSetFramebufferSizeCallback in vulkan_window.cpp, we also check here for the state of our swapchain.
        //! @note If our swapchain's state is either VK_ERROR_OUT_OF_DATE_KHR or VK_SUBOPTIMAL_KHR, then the Acquired function returns -3.
        //! @note -3 was just to indicate the swapchain's state as been modified and needs to modify the pipeline to work with the new swapchain.
        //! @note Another TODO: Make abstract the pipeline's so that we do not have to modify this directly.
        //! @note If this isn't already done, but the vk::submit function should handle the way we directly handle these tasks... potentially?
        g_CurrentFrameIndex = ApplicationInstance::GetWindow().GetCurrentSwapchain()->AcquireNextImage();
        // ConsoleLogInfoWithTag("vulkan", "Current Frame Index = {}", g_CurrentFrameIndex.load());
        // ConsoleLogWarnWithTag("vulkan", "IsResized = {}", ApplicationInstance::GetWindow().GetCurrentSwapchain()->IsResized());
        // if(ApplicationInstance::GetWindow().GetCurrentSwapchain()->IsResized()){
        // ConsoleLogErrorWithTag("vulkan", "Atomic Current Frame Index = {}", g_CurrentFrameIndex.load());
        auto isResized = ApplicationInstance::GetWindow().GetCurrentSwapchain()->IsResized();
        // ConsoleLogTraceWithTag("vulkan", "IsResized (bool) = {}", isResized);
        if(isResized){
            // ConsoleLogInfoWithTag("vulkan", "How often are you being called!");
            ConsoleLogFatalWithTag("vulkan", "How often are you being called to be initialized?");
            // Renderer::Initialize();
            InitializeRendererPipeline();
        }

        VkCommandBufferBeginInfo cmd_buffer_begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
        };

        auto cmd_buffer = GetCurrentCommandBuffer();

        vk::vk_check(vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info), "vkBeginCommandBuffer", __FILE__, __LINE__, __FUNCTION__);
        VkRenderPassBeginInfo rp_begin_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetRenderPass(),
            .framebuffer = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetFramebuffer(g_CurrentFrameIndex), // Specifying which framebuffer to render pass to.
            .renderArea = {
                .offset = {0, 0},
                .extent = {ApplicationInstance::GetWindow().GetWidth(), ApplicationInstance::GetWindow().GetHeight()}
            }
        };

        std::array<VkClearValue, 2> clearValues;
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        rp_begin_info.clearValueCount = static_cast<uint32_t>(clearValues.size());
        rp_begin_info.pClearValues = clearValues.data();


        vkCmdBeginRenderPass(cmd_buffer, &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);
        g_IsFrameStarted = true;
        
        VkViewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetSwapchainExtent().width),
            .height = static_cast<float>(ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetSwapchainExtent().height),
            .maxDepth = 1.0f,
        };

        VkRect2D scissor = {
            .offset = {0, 0},
            .extent = ApplicationInstance::GetWindow().GetCurrentSwapchain()->GetSwapchainExtent()
        };

        g_CommandBuffers[g_CurrentFrameIndex] = cmd_buffer;

        vkCmdSetViewport(g_CommandBuffers[g_CurrentFrameIndex], 0, 1, &viewport);
        vkCmdSetScissor(g_CommandBuffers[g_CurrentFrameIndex], 0, 1, &scissor);
    }

    void VulkanRenderer::EndFrame(){
        auto cmd_buffer = GetCurrentCommandBuffer();
        vkCmdEndRenderPass(cmd_buffer);
        vk::vk_check(vkEndCommandBuffer(cmd_buffer), "vkEndCommandBuffer", __FILE__, __LINE__, __FUNCTION__);

        ApplicationInstance::GetWindow().GetCurrentSwapchain()->SubmitCommandBuffer(&cmd_buffer);
        g_IsFrameStarted = false;
    }

    // void VulkanRenderer::DrawScene(){}

    VkCommandBuffer VulkanRenderer::GetCurrentCommandBuffer(){
        return g_CommandBuffers[g_CurrentFrameIndex];
    }
};