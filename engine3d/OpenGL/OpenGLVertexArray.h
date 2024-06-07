#include <engine3d/Core/core.h>
#include <engine3d/Graphics/VertexArray.h>
#include <vector>

namespace Engine3D{
    class OpenGLVertexArray : public VertexArray{
    public:
        OpenGLVertexArray();

    private:
        void ApplyCurrentBoundState() override;
        void ReleaseCurrentBoundState() override;

        void PushVertexBuffer(Ref<VertexBuffer>& buffer) override;
        void SetIndexBufferInternal(Ref<IndexBuffer>& buffer) override;

        const std::vector<Ref<VertexBuffer>>& ExtractBuffers() const override;
        const Ref<IndexBuffer>& ExtractIndexBuffer() const override;

    private:
        uint32_t m_Id;
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_Ibo;
        uint32_t m_Index = 0;
    };
};
