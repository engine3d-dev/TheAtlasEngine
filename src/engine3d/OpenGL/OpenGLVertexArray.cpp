#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/OpenGL/OpenGLVertexArray.h>

namespace Engine3D{

    static GLenum ShaderDataTypeToBaseConversion(ShaderData_t type){
        switch(type){
            case ShaderData_t::None:
                CoreLogWarn("No datatype was entered in ShaderDataTypeToBaseConversion()");
                assert(false);
                break;
            case ShaderData_t::Float: return GL_FLOAT;
            case ShaderData_t::Float2: return GL_FLOAT;
            case ShaderData_t::Float3: return GL_FLOAT;
            case ShaderData_t::Float4: return GL_FLOAT;
            case ShaderData_t::Mat3: return GL_FLOAT;
            case ShaderData_t::Mat4: return GL_FLOAT;
            case ShaderData_t::Int: return GL_INT;
            case ShaderData_t::Int2: return GL_INT;
            case ShaderData_t::Int3: return GL_INT;
            case ShaderData_t::Int4: return GL_INT;
            case ShaderData_t::Bool: return GL_BOOL;
        }
        
        CoreLogWarn("No datatype was entered in ShaderDataTypeToBaseConversion()");
        assert(false);
        return 0;
    }
    
    OpenGLVertexArray::OpenGLVertexArray(){
        glGenVertexArrays(1, &m_Id);
        ApplyCurrentBoundState();
    }

    void OpenGLVertexArray::ApplyCurrentBoundState(){
        glBindVertexArray(m_Id);
    }

    void OpenGLVertexArray::ReleaseCurrentBoundState(){
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::PushVertexBuffer(Ref<VertexBuffer>& buffer) {
        //! @note In OpenGL we bound vao current state before writing our data to it
        m_Index = 0;
        ApplyCurrentBoundState();
        buffer->ApplyResource();
        
        const auto& layout = buffer->GetLayout();

        //! @note glEnableVertexAttribArray tells OpenGL where in our vertex array we want to access our buffer at which index.
        for(auto& element : layout){
            switch (element.type){
            case ShaderData_t::Float:
            case ShaderData_t::Float2:
            case ShaderData_t::Float3:
            case ShaderData_t::Float4:
                {
                    glVertexAttribPointer(
                        m_Index,
                        element.GetElementSize(),
                        ShaderDataTypeToBaseConversion(element.type),
                        element.isNormalized ? GL_TRUE : GL_FALSE,
                        layout.Stride(),
                        (void *)element.offset);
                    glEnableVertexAttribArray(m_Index);
                    m_Index++;
                }
                break;
            case ShaderData_t::Int:
            case ShaderData_t::Int2:
            case ShaderData_t::Int3:
            case ShaderData_t::Int4:
            case ShaderData_t::Bool:
                {
                    glVertexAttribIPointer(
                        m_Index,
                        element.GetElementSize(),
                        ShaderDataTypeToBaseConversion(element.type),
                        layout.Stride(),
                        (void *)element.offset);
                    glEnableVertexAttribArray(m_Index);
                    m_Index++;
                }
                break;
            default:
                break;
            }
        }

        m_VertexBuffers.push_back(buffer);
    }

    void OpenGLVertexArray::SetIndexBufferInternal(Ref<IndexBuffer>& buffer) {
        ApplyCurrentBoundState();
        buffer->ApplyResource();
        m_Ibo = buffer;
    }

    const std::vector<Ref<VertexBuffer>>& OpenGLVertexArray::ExtractBuffers() const { return m_VertexBuffers; }

    const Ref<IndexBuffer>& OpenGLVertexArray::ExtractIndexBuffer() const { return m_Ibo; }
};