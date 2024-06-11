#pragma once
#include <engine3d/Core/core.h>
#include <engine3d/Graphics/Texture.h>
#include <engine3d/Image/Image.h>

namespace engine3d{
    class OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D(const std::string& filename);
        OpenGLTexture2D(uint32_t w, uint32_t h);

    private:

        bool IsTextureLoaded() const override;

        uint32_t GetImageWidth() const override;
        uint32_t GetImageHeight() const override;

        uint32_t GetID() const override;


        void ApplyCurrentBoundState(GLenum bind = GL_TEXTURE_2D) override;
        void ReleaseCurrentBoundState() override;

    private:
        Image m_Image;
        std::string m_Filepath;
        int m_Width, m_Height;
        uint32_t m_Id;
        uint32_t m_InternalFormat, m_FormatData;
    };
};