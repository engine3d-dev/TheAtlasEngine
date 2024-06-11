#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/OpenGL/OpenGLBuffer.h>

namespace engine3d{
    // **********************************************
    //               [ Vertex Buffer ]
    // **********************************************
    OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, size_t size){
        glGenBuffers(1, &m_Id);
        ApplyCurrentBoundState();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size){
        glGenBuffers(1, &m_Id);
        ApplyCurrentBoundState();
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer(){
        glDeleteBuffers(1, &m_Id);
    }

    void OpenGLVertexBuffer::ApplyCurrentBoundState(){
        glBindBuffer(GL_ARRAY_BUFFER, m_Id);
    }

    void OpenGLVertexBuffer::ReleaseCurrentBoundState(){
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::SetDataInternal(const void* data, uint32_t size){
        //! @note In OpenGL you must bind the buffer you want to write data too before writing to that data
        ApplyCurrentBoundState();
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    BufferLayout OpenGLVertexBuffer::BufferLayoutExtract() const {
        return m_Layout;
    }

    void OpenGLVertexBuffer::SetBufferLayout(const BufferLayout& layout) {
        m_Layout = layout;
    }



    // **********************************************
    //                 [ Index Buffer ]
    // **********************************************
    OpenGLIndexBuffer::OpenGLIndexBuffer(const void* indices, size_t count) : m_Count(count) {
        glGenBuffers(1, &m_Id);
        ApplyCurrentBoundState();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer(){
        glDeleteBuffers(1, &m_Id);
    }

    void OpenGLIndexBuffer::ApplyCurrentBoundState(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
    }

    void OpenGLIndexBuffer::ReleaseCurrentBoundState(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    size_t OpenGLIndexBuffer::ExtractCount() const{
        return m_Count;
    }

};