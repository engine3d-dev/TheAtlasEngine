#pragma once
#include <engine3d/Core/core.h>
#include <cstring>
#include <string>
#include <vector>

namespace Engine3D{
    /**
     * @name Buffer.h
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
     * @name BufferElement
     * @note API Usage ----- { ShaderData_t::Float4, "a_Position", true } -> [equivalent to] -> layout (location = 0) vec4 a_Color;
     * @param index
     * @note In vertex array we handle the index
     * @note Index tells us where we want to configure. Remembering our "vertex" shader we specified layout(location = 0).
     * @note That sets out location of vertex attribute to zero (0), since we want to pass vertex attribute.
     * @note Therefore our index would be zero (0)
     * @note Initially when using layout(location = n), you use vertex attributes for these data (at least in OpenGL)
     * 
     * @param name
     * @note For debugging purpose for which data in our shader we are setting our glVertAttrib to
     * 
     * @param type
     * @note Setting what type our element is going to be set as
     * 
     * @param size
     * @note variable (uint32_t) that is for specifying the size of our vertex attribute.
     * @note Example is if we were to do, Float3. Which composes of 3 floats that are 4 bytes each float.
     * 
     * @param GetElementSize
     * @note Returns the size of our data type (in bytes)
     * 
     * @param stride
     * @note tells us space between vertex attributes. Since positional data rae exactly 3x size of float, then is value specified.
     *
     * @param offset
     * @note Requires some weird cast (at least for OpenGL). Represents our offset of where our data begins in our buffer
    */

    enum class ShaderData_t : uint8_t {
        None = 0,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderData_t type){
        switch (type){
        case ShaderData_t::None: return 0;
        case ShaderData_t::Float: return sizeof(float);
        case ShaderData_t::Float2: return sizeof(float) * 2;
        case ShaderData_t::Float3: return sizeof(float) * 3;
        case ShaderData_t::Float4: return sizeof(float) * 4;
        case ShaderData_t::Mat3: return sizeof(float) * 3 * 3;
        case ShaderData_t::Mat4: return sizeof(float) * 4 * 4;
        case ShaderData_t::Int: return sizeof(int);
        case ShaderData_t::Int2: return sizeof(int) * 2;
        case ShaderData_t::Int3: return sizeof(int) * 3;
        case ShaderData_t::Int4: return sizeof(int) * 4;
        case ShaderData_t::Bool: return 1;
        }

        // CoreLogError("Error because Type passed in ShaderDataTypeSize was invalid!");
        assert(false);
        return 0;
    }

    struct BufferElement{
        BufferElement() = default;
        BufferElement(ShaderData_t t, const std::string& n, bool normalized = false) : type(t), name(n), size(ShaderDataTypeSize(type)), offset(0), isNormalized(normalized) {}

        uint32_t GetElementSize() const {
            switch (type){
                case ShaderData_t::None: return 0;
                case ShaderData_t::Float:  return 1;
                case ShaderData_t::Float2: return 2;
                case ShaderData_t::Float3: return 3;
                case ShaderData_t::Float4: return 4;
                case ShaderData_t::Mat3:   return 3 * 3;
                case ShaderData_t::Mat4:   return 4 * 4;
                case ShaderData_t::Int:    return 1;
                case ShaderData_t::Int2:   return 2;
                case ShaderData_t::Int3:   return 3;
                case ShaderData_t::Int4:   return 4;
                case ShaderData_t::Bool:   return 1;
            }
        }

        std::string name;
        ShaderData_t type;
        uint32_t size;
        uint32_t offset;
        bool isNormalized;
    };

    class BufferLayout{
        using iterator = std::vector<BufferElement>::iterator;
        using const_iterator = std::vector<BufferElement>::const_iterator;
    public:
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement>& list) : m_Elements(list) {
            ComputeOffstetAndStride();
        }

        uint32_t Stride() const { return m_Stride; }

        std::vector<BufferElement> GetElements() const { return m_Elements; }

        iterator begin(){ return m_Elements.begin(); }
        iterator end(){ return m_Elements.end(); }

        const_iterator begin() const { return m_Elements.begin(); }
        const_iterator end() const { return m_Elements.end(); }




    private:
        //! @note This will help us fetch our offset and stride data for vertex attributes
        void ComputeOffstetAndStride(){
            uint32_t offset = 0;
            m_Stride = 0;

            for(auto& element : m_Elements){
                offset += element.size;
                m_Stride += element.size;
            }
        }

    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };


    class VertexBuffer{
    public:
        //! @note Will use API-specific implementation that is dependent on the current enabled API set by user.
        void ApplyResource();
        void ReleaseResource();
        void SetData(const void* data, uint32_t size);

        BufferLayout GetLayout() const;
        void SetLayout(const BufferLayout& layout);

        static Ref<VertexBuffer> Create(const void* data, size_t size);
        static Ref<VertexBuffer> Create(size_t size);

    private:
        virtual void ApplyCurrentBoundState() = 0;
        virtual void ReleaseCurrentBoundState() = 0;
        virtual void SetDataInternal(const void* data, uint32_t size) = 0;
        virtual BufferLayout BufferLayoutExtract() const = 0;
        virtual void SetBufferLayout(const BufferLayout& layout) = 0;
    };

    class IndexBuffer{
    public:
        void ApplyResource();
        void ReleaseResource();

        static Ref<IndexBuffer> Create(const void* data, size_t size);

        size_t GetCount() const;

    private:
        virtual void ApplyCurrentBoundState() = 0;
        virtual void ReleaseCurrentBoundState() = 0;
        virtual size_t ExtractCount() const = 0;
    };
};