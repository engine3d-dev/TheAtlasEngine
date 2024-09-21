#include <Core/backend/Vulkan-Experiemental/helper-functions.h>
#include <Core/EngineLogger.h>
#include <Core/backend/utilities/helper_functions.h>

namespace engine3d::vk{
    void begin(const VkCommandBuffer& buffer, VkCommandBufferUsageFlags p_UsageFlags){
        VkCommandBufferBeginInfo beginInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = p_UsageFlags,
            .pInheritanceInfo = nullptr
        };

        VkResult res = vkBeginCommandBuffer(buffer, &beginInfo);

        if(res != VK_SUCCESS){
            ConsoleLogError("vkBeginCommandBuffer errored message!===>\t\t\t{}", vk::VkResultToString(res));
        }
    }

    void end(const VkCommandBuffer& buffer){
        VkResult res = vkEndCommandBuffer(buffer);

        if(res != VK_SUCCESS){
            ConsoleLogError("vkEndCommandBuffer errored message!===>\t\t\t{}", vk::VkResultToString(res));
        }
    }
};