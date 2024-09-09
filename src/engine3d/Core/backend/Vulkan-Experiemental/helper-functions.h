#pragma once

namespace engine3d{
    namespace vk{
        class VulkanCommandBuffer;

        /**
         * @name record
         * @note Collects the command user supplies into the command buffer.
        */
        template<typename UCommand>
        void record(VulkanCommandBuffer& commandBuffer, const UCommand&& p_CommandToCollect){
            
        }
    };
};