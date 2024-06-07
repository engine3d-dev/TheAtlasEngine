#pragma once
#include <engine3d/Core/core.h>
#include <engine3d/Graphics/Buffer.h>

namespace Engine3D{
    /**
     * @name VertexArray.h
     * @note API-agnostic implementations and naming convention functions
     * 
     * @param ApplyResource
     * @note In OpenGL equivalent to doing glBind*, essentially
     * @note User API function call for applying specific data to our buffer for reading/writing to our buffer
     * 
     * @param ReleaseResource
     * @note User API function call used for unbinding or releasing data not in use for read/writing
     * 
     * @param ApplyCurrentBoundState
     * @note API-agnostic implementation since OpenGL, Vulkan, and DirectX has differences on how they handle in writing to buffers
     * 
     * @param ReleaseCurrentBoundState
     * @note API-agnostic implementation that will unbind/release/detach data for not in use for writing data to this buffer.
     * 
    */
    class VertexArray{
    public:
        static Ref<VertexArray> Create();

        void ApplyResource();
        void ReleaseResources();
        const std::vector<Ref<VertexBuffer>> GetBuffers() const;
        const Ref<IndexBuffer> GetIndexBuffer() const;

        void AddVertexBuffer(Ref<VertexBuffer>& buffer);
        void SetIndexBuffer(Ref<IndexBuffer>& buffer);


    private:
        virtual void ApplyCurrentBoundState() = 0;
        virtual void ReleaseCurrentBoundState() = 0;

        virtual void PushVertexBuffer(Ref<VertexBuffer>& buffer) = 0;
        virtual void SetIndexBufferInternal(Ref<IndexBuffer>& buffer) = 0;

        virtual const std::vector<Ref<VertexBuffer>>& ExtractBuffers() const = 0;
        virtual const Ref<IndexBuffer>& ExtractIndexBuffer() const = 0;
    };
};