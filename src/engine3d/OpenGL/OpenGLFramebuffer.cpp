#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/OpenGL/OpenGLFramebuffer.h>

namespace engine3d{
    static const uint32_t g_MaxFramebufferSize = 8192;

    static GLenum TextureTarget(bool multisample){
        return multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    static bool IsDepth(TextureFormat format){
        if(format == TextureFormat::DEPTH24STENCIL8) return true;
        return false;
    }

    static GLenum FBTextureFormatToGL(TextureFormat format){
        switch(format){
        case TextureFormat::RGBA8: return GL_RGBA8;
        case TextureFormat::RGB: return GL_RGB;
        case TextureFormat::RED_INTEGER: return GL_RED_INTEGER;
        default: return 0;
        }

        return 0;
    }

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferMetaData& framebufferMetaData) : m_FramebufferMetaData(framebufferMetaData) {

        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer(){
        glDeleteFramebuffers(1, &m_Id);
    }

    void OpenGLFramebuffer::Invalidate(){
        glCreateFramebuffers(1, &m_Id);
        ApplyCurrentBoundState();

        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        //! @note Difference between image and storage is how they handle sampling
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_FramebufferMetaData.width, m_FramebufferMetaData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        //! @note Handling interpolation for textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
        //! @note Creating out color attachments
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

        // //! @note Creating our depth attachment
        // //! @note Supporting multisampling
        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_FramebufferMetaData.width, m_FramebufferMetaData.height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

        //! @note Checking framebuffer status if its working

        // static_assert(glCheckFramebufferStatus(GL_FRAMEBUFFER)==GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete!");
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE){
        }
        else{
            CoreLogFatal("Framebuffer status was incomplete!");
            assert(false);
        }

        ReleaseCurrentBoundState();
    }

    void OpenGLFramebuffer::ApplyCurrentBoundState(){
        glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
        // Pipeline::SetViewport(m_FramebufferMetaData.width, m_FramebufferMetaData.height);
    }
    void OpenGLFramebuffer::ReleaseCurrentBoundState(){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    int OpenGLFramebuffer::GetPixelData(uint32_t attachmentIdx, int x, int y){
        assert(attachmentIdx < m_ColorAttachments.size());

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIdx);

        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

        return pixelData;
    }

    uint32_t OpenGLFramebuffer::GetPixelAttachmentID(uint32_t index) const {
        assert(index < m_FramebufferMetaData.attachments.attachments.size());
        return m_FramebufferMetaData.attachments.attachments[index].attachmentID;
    }
    
    const FramebufferMetaData& OpenGLFramebuffer::GetFramebufferData() const{
        return m_FramebufferMetaData;
    }

    void OpenGLFramebuffer::ClearAttachmentData(uint32_t attachmentIdx, int data){
        // Pipeline::Clear();
        // Pipeline::RefreshFrame();
        // glClearBufferiv(m_ColorAttachments[attachmentIdx].attachmentID, 0, &data);
    }

    int OpenGLFramebuffer::GetMaxFramebufferSize(){
        return g_MaxFramebufferSize;
    }

    uint32_t OpenGLFramebuffer::HasColorAttachmentID() {
        return m_ColorAttachment;
    }

    uint32_t OpenGLFramebuffer::HasDepthAttachmentID(){
        return m_DepthAttachment;
    }

};