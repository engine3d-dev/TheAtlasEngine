#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/OpenGL/OpenGLTexture.h>

namespace engine3d{
    OpenGLTexture2D::OpenGLTexture2D(uint32_t w, uint32_t h) : m_Image(w, h){
        m_Image.SetVerticalOnLoad(true);
        m_Width = w;
        m_Height = h;

        m_InternalFormat = GL_RGBA8;
        m_FormatData = GL_RGBA;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& filename) : m_Image(filename) {
        if(!m_Image.IsLoaded()){
            CoreLogWarn("Image was not able to load to be used as texture!");
        }

        m_Image.SetVerticalOnLoad(true);
        int channels = m_Image.GetPixelChannels();

        m_Width = m_Image.GetWidth();
        m_Height = m_Image.GetWidth();

        if(channels == 4){
            m_InternalFormat = GL_RGBA8;
            m_FormatData = GL_RGBA;
        }
        else if(channels == 3){
            m_InternalFormat = GL_RGB8;
            m_FormatData = GL_RGB;
        }

        //! @note In Windows change to this ---- glTextureStorage2D(GL_TEXTURE_2D, 0, _internalFormat, _width, _height);
        glGenTextures(1, &m_Id);

        ApplyCurrentBoundState();
        //! @note Setting our textures processing our image parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// Since we segfault using glTextureParameteri, we'll use this for now
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_FormatData, GL_UNSIGNED_BYTE, m_Image.GetImageData());
        glGenerateMipmap(GL_TEXTURE_2D);
        ReleaseCurrentBoundState();
    }

    bool OpenGLTexture2D::IsTextureLoaded() const {
        return m_Image.IsLoaded();
    }

    uint32_t OpenGLTexture2D::GetImageWidth() const{ return m_Width; }

    uint32_t OpenGLTexture2D::GetImageHeight() const{ return m_Height; }

    uint32_t OpenGLTexture2D::GetID() const{ return m_Id; }

    void OpenGLTexture2D::ApplyCurrentBoundState(GLenum slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_Id);
    }

    void OpenGLTexture2D::ReleaseCurrentBoundState() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};