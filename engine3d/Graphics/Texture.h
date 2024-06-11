#pragma once
#include <engine3d/Core/core.h>
#include <GLFW/glfw3.h>

namespace engine3d{

    class Texture2D{
    public:
        static Ref<Texture2D> Create(const std::string& filename);
        static Ref<Texture2D> Create(uint32_t w, uint32_t h);

        bool IsLoaded() const;

        void ApplyResources();
        void ReleaseResources();

        uint32_t GetWidth() const;
        uint32_t GetHeight() const;

        uint32_t GetTextureID() const;

    private:
        virtual bool IsTextureLoaded() const = 0;

        virtual uint32_t GetImageWidth() const = 0;
        virtual uint32_t GetImageHeight() const = 0;

        virtual uint32_t GetID() const = 0;


        virtual void ApplyCurrentBoundState(GLenum bind = GL_TEXTURE_2D) = 0;
        virtual void ReleaseCurrentBoundState() = 0;
    };
};