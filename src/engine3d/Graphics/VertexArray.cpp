#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Graphics/VertexArray.h>
#include <engine3d/OpenGL/OpenGLVertexArray.h>

namespace engine3d{
    Ref<VertexArray> VertexArray::Create(){
        switch(Pipeline::CurrentAPI()){
            case API::OPENGL: return CreateRef<OpenGLVertexArray>();
            default:
                CoreLogError("API either not supported or enabled correctly");
                CoreLogError("Error is in VertexArray::Create");
                assert(false);
                return nullptr;
        }

        CoreLogError("API either not supported or enabled correctly");
        CoreLogError("Error is in VertexArray::Create");
        assert(false);
        return nullptr;
    }

    void VertexArray::ApplyResource(){
        ApplyCurrentBoundState();
    }

    void VertexArray::ReleaseResources(){
        ReleaseCurrentBoundState();
    }

    void VertexArray::AddVertexBuffer(Ref<VertexBuffer>& buffer){
        PushVertexBuffer(buffer);
    }

    void VertexArray::SetIndexBuffer(Ref<IndexBuffer>& buffer){
        SetIndexBufferInternal(buffer);
    }

    const std::vector<Ref<VertexBuffer>> VertexArray::GetBuffers() const{ return ExtractBuffers(); }
    const Ref<IndexBuffer> VertexArray::GetIndexBuffer() const { return ExtractIndexBuffer(); }
};