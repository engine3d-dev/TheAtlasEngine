#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Graphics/Framebuffer.h>
#include <engine3d/OpenGL/OpenGLFramebuffer.h>

namespace engine3d{

    Ref<Framebuffer> Framebuffer::Create(const FramebufferMetaData& framebufferMetaData){
        switch(Pipeline::CurrentAPI()){
        case API::OPENGL: return CreateRef<OpenGLFramebuffer>(framebufferMetaData);
        default: break;
        }

        CoreLogError("In Framebuffer::Create() Pipeline::CurrentAPI() was enabled to an unsupported API!");
        assert(false);
        return nullptr;
    }

    void Framebuffer::ApplyResources(){
        ApplyCurrentBoundState();
    }

    void Framebuffer::ReleaseResources(){
        ReleaseCurrentBoundState();
    }

    uint32_t Framebuffer::GetColorAttachmentId() {
        return HasColorAttachmentID();
    }

    uint32_t Framebuffer::GetDepthAttachmentId(){
        return HasDepthAttachmentID();
    }

    int Framebuffer::ReadPixel(uint32_t attachmentIdx, int x, int y){
        return GetPixelData(attachmentIdx, x, y);
    }

    void Framebuffer::ClearAttachements(uint32_t attachmentIdx, int data) {
        ClearAttachmentData(attachmentIdx, data);
    }

    uint32_t Framebuffer::GetAttachmentID(uint32_t idx) const {
        return GetPixelAttachmentID(idx);
    }

    int Framebuffer::MaxFramebufferSize(){
        return OpenGLFramebuffer::GetMaxFramebufferSize();
    }

    void Framebuffer::Resize(uint32_t w, uint32_t h){
        // ResizeFramebuffer(w, h);
    }

};