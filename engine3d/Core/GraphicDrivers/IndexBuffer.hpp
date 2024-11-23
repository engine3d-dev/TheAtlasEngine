#pragma once
#include <Core/Core.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace engine3d{
    class IndexBuffer{
    public:
        static Ref<IndexBuffer> Create(std::vector<uint32_t> p_Indices);

        void Bind(VkCommandBuffer p_CommandBuffer);
        void Draw(VkCommandBuffer p_CommandBuffer);

        bool HasIndicesPresent() const;

    protected:
        virtual void BindToIndexBuffer(VkCommandBuffer p_CommandBuffer) = 0;
        virtual void RenderIndexBuffer(VkCommandBuffer p_CommandBuffer) = 0;
        virtual bool HasIndices() const = 0;
    };
};