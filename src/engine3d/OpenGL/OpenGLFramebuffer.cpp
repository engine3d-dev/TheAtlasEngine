#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/OpenGL/OpenGLFramebuffer.h>

namespace Engine3D{
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
        if(m_FramebufferMetaData.attachments.attachments.size() <= 0){
            assert(m_FramebufferMetaData.attachments.attachments.size() > 0);
        }

        for(auto metaData : m_FramebufferMetaData.attachments.attachments){
            m_AttachmentsFormats.push_back(metaData.textureFormat);

            if(!IsDepth(metaData.textureFormat)){
                m_ColorAttachments.emplace_back(metaData);
            }
            else{
                m_DepthAttachmentData = std::move(metaData);
            }
        }

        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer(){
        glDeleteFramebuffers(1, &m_Id);

        for(auto data : m_FramebufferMetaData.attachments.attachments){
            glDeleteTextures(1, &data.attachmentID);
        }

        glDeleteTextures(1, &m_DepthAttachmentData.attachmentID);
    }

    void OpenGLFramebuffer::ApplyCurrentBoundState(){
        glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
        Pipeline::SetViewport(m_FramebufferMetaData.width, m_FramebufferMetaData.height);
    }
    void OpenGLFramebuffer::ReleaseCurrentBoundState(){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Invalidate(){
        if(!m_Id){
            glDeleteFramebuffers(1, &m_Id);
        }

        glGenFramebuffers(1, &m_Id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_Id);

        if(m_DepthAttachmentData.textureFormat == TextureFormat::None){
            CoreLogError("Depth Texture Attachment is None");
            return;
        }

        bool multisample = m_FramebufferMetaData.samples > 1;

        if(m_ColorAttachments.size() > 0){
            for(size_t i = 0; i < m_ColorAttachments.size(); i++){
                glGenTextures(1, &m_ColorAttachments[i].attachmentID);
                glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[i].attachmentID);

                switch (m_ColorAttachments[i].textureFormat){
                case TextureFormat::RGBA8:
                    {
                        if(multisample){
                            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_FramebufferMetaData.samples, GL_RGBA8, m_FramebufferMetaData.width, m_FramebufferMetaData.height, GL_FALSE);
                        }
                        else{
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_FramebufferMetaData.width, m_FramebufferMetaData.height, 0, GL_RGBA, GL_UNSIGNED_INT, nullptr);

                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, TextureTarget(multisample), m_ColorAttachments[i].attachmentID, 0);
                        }
                    }
                    break;
                case TextureFormat::RED_INTEGER:
                    {
                        if(multisample){
                            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_FramebufferMetaData.samples, GL_RGBA8, m_FramebufferMetaData.width, m_FramebufferMetaData.height, GL_FALSE);
                        }
                        else{
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_FramebufferMetaData.width, m_FramebufferMetaData.height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);

                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, TextureTarget(multisample), m_ColorAttachments[i].attachmentID, 0);
                        }
                    }
                    break;
                default:
                    break;
                }
            }
        }

        if(m_DepthAttachmentData.textureFormat != TextureFormat::None){
            glGenTextures(1, &m_DepthAttachmentData.attachmentID);
            glBindTexture(GL_TEXTURE_2D, m_DepthAttachmentData.attachmentID);

            if(multisample){
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_FramebufferMetaData.samples, GL_DEPTH24_STENCIL8, m_FramebufferMetaData.width, m_FramebufferMetaData.height, GL_FALSE);
            }
            else{
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_FramebufferMetaData.width, m_FramebufferMetaData.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, TextureTarget(multisample), m_DepthAttachmentData.attachmentID, 0);
        }

        if(m_ColorAttachments.size() > 1){
            GLenum bufferAttachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
            glDrawBuffers(m_ColorAttachments.size(), bufferAttachments);
        }
        else if(m_ColorAttachments.empty()){
            glDrawBuffer(GL_NONE);
        }

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            CoreLogError("Framebuffer could not be initiated (from OpenGLFrameBuffer::Invalidate)");
            assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        }

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
        Pipeline::RefreshFrame();
        glClearBufferiv(m_ColorAttachments[attachmentIdx].attachmentID, 0, &data);
    }

    int OpenGLFramebuffer::GetMaxFramebufferSize(){
        return g_MaxFramebufferSize;
    }

};