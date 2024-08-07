#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Graphics/Texture.h>
#include <engine3d/OpenGL/OpenGLTexture.h>

namespace engine3d{

    Ref<Texture2D> Texture2D::Create(const std::string& filename){
        switch (Pipeline::CurrentAPI()){
        case API::OPENGL: return CreateRef<OpenGLTexture2D>(filename);
        default: break;
        }

        ConsoleLogError("Texture2D::Create Error --- API enabled is unsupported!");
        assert(false);
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(uint32_t w, uint32_t h){
        switch(Pipeline::CurrentAPI()){
        case API::OPENGL: return CreateRef<OpenGLTexture2D>(w, h);
        default: break;
        }

        ConsoleLogError("Texture2D::Create Error --- API enabled is unsupported!");
        assert(false);
        return nullptr;
    }

    bool Texture2D::IsLoaded() const{
        return IsTextureLoaded();
    }

    uint32_t Texture2D::GetWidth() const {
        return GetImageWidth();
    }

    uint32_t Texture2D::GetHeight() const{
        return GetImageHeight();
    }

    uint32_t Texture2D::GetTextureID() const{
        return GetID();
    }

    void Texture2D::ApplyResources(){
        ApplyCurrentBoundState();
    }

    void Texture2D::ReleaseResources(){
        ReleaseCurrentBoundState();
    }

};