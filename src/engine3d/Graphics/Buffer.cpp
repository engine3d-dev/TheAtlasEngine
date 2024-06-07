#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Graphics/Buffer.h>
#include <engine3d/OpenGL/OpenGLBuffer.h>

namespace Engine3D{
    // **********************************************
    //               [ Vertex Buffer ]
    // **********************************************

    Ref<VertexBuffer> VertexBuffer::Create(const void* data, size_t size){
        switch (Pipeline::CurrentAPI()){
        case API::OPENGL: return CreateRef<OpenGLVertexBuffer>(data, size);
        case API::VULKAN: CoreLogError("Vulkan not implemented Shader"); break;
        case API::DIRECTX11: CoreLogError("DirectX12 not implemented Shader"); break;
        case API::DIRECT12: CoreLogError("DirectX12 not implemented Shader"); break;
        default:
            break;
        }

        CoreLogWarn("API unsupported!");
        assert(false);
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(size_t size){
        switch (Pipeline::CurrentAPI()){
        case API::OPENGL: return CreateRef<OpenGLVertexBuffer>(size);
        case API::VULKAN: CoreLogError("Vulkan not implemented Shader"); break;
        case API::DIRECTX11: CoreLogError("DirectX12 not implemented Shader"); break;
        case API::DIRECT12: CoreLogError("DirectX12 not implemented Shader"); break;
        default:
            break;
        }

        CoreLogWarn("API unsupported!");
        assert(false);
        return nullptr;
    }

    void VertexBuffer::ApplyResource(){
        ApplyCurrentBoundState();
    }

    void VertexBuffer::ReleaseResource(){
        ReleaseCurrentBoundState();
    }

    void VertexBuffer::SetData(const void* data, uint32_t size){
        SetDataInternal(data, size);
    }

    BufferLayout VertexBuffer::GetLayout() const { return BufferLayoutExtract(); }

    void VertexBuffer::SetLayout(const BufferLayout& layout){ SetBufferLayout(layout); }

    // **********************************************
    //                 [ Index Buffer ]
    // **********************************************
    Ref<IndexBuffer> IndexBuffer::Create(const void* indices, size_t size){
        switch (Pipeline::CurrentAPI()){
        case API::OPENGL: return CreateRef<OpenGLIndexBuffer>(indices, size);
        case API::VULKAN: CoreLogError("Vulkan not implemented Shader"); break;
        case API::DIRECTX11: CoreLogError("DirectX12 not implemented Shader"); break;
        case API::DIRECT12: CoreLogError("DirectX12 not implemented Shader"); break;
        default:
            break;
        }

        assert(false);
        return nullptr;
    }

    void IndexBuffer::ApplyResource(){
        ApplyCurrentBoundState();
    }

    void IndexBuffer::ReleaseResource(){
        ReleaseCurrentBoundState();
    }

    size_t IndexBuffer::GetCount() const { return ExtractCount(); }
};