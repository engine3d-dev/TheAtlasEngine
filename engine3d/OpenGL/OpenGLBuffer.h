#pragma once
#include <engine3d/Core/core.h>
#include <engine3d/Graphics/Buffer.h>

namespace engine3d{
    class OpenGLVertexBuffer : public VertexBuffer{
    public:
        OpenGLVertexBuffer(const void* data, size_t size);
        OpenGLVertexBuffer(size_t size);
        ~OpenGLVertexBuffer();

    private:
        void ApplyCurrentBoundState() override;
        void ReleaseCurrentBoundState() override;

        void SetDataInternal(const void* data, uint32_t size) override;

        BufferLayout BufferLayoutExtract() const override;
        void SetBufferLayout(const BufferLayout& layout) override;

    private:
        uint32_t m_Id;
        BufferLayout m_Layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer{
    public:
        OpenGLIndexBuffer(const void* indices, size_t size);
        ~OpenGLIndexBuffer();

    private:
        void ApplyCurrentBoundState() override;
        void ReleaseCurrentBoundState() override;
        size_t ExtractCount() const override;

    private:
        uint32_t m_Id;
        uint32_t m_Count=0;
    };
};