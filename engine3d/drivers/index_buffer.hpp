#pragma once
#include <core/core.hpp>
#include <vulkan/vulkan.hpp>
#include <vector>

namespace engine3d{
    class IndexBuffer{
    public:
        virtual ~IndexBuffer() = default;
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