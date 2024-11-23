#include "ApplicationInstance.hpp"
#include <Core/GraphicDrivers/IndexBuffer.hpp>
#include <Core/internal/Vulkan2Showcase/VulkanIndexBuffer.hpp>

namespace engine3d{
    Ref<IndexBuffer> IndexBuffer::Create(std::vector<uint32_t> p_Indices){
        switch (ApplicationInstance::CurrentAPI()){
        case API::VULKAN: return CreateRef<vk::VulkanIndexBuffer>(p_Indices);
        default: return nullptr;
        }

        return nullptr;
    }

    void IndexBuffer::Bind(VkCommandBuffer p_CommandBuffer){
        return BindToIndexBuffer(p_CommandBuffer);
    }

    void IndexBuffer::Draw(VkCommandBuffer p_CommandBuffer){
        return RenderIndexBuffer(p_CommandBuffer);
    }

    bool IndexBuffer::HasIndicesPresent() const{
        return HasIndices();
    }
};