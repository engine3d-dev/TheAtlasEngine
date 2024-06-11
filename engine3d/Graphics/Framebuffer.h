#pragma once
#include <engine3d/Core/core.h>
#include <vector>

namespace engine3d{

    enum class TextureFormat{
        None = 0,

        RGBA8,
        RGB,
        RED_INTEGER,

        // Depth/Stencil
		DEPTH24STENCIL8,
		DEPTH=DEPTH24STENCIL8
    };

    struct FramebufferTextureMetaData{
        FramebufferTextureMetaData() = default;
        FramebufferTextureMetaData(TextureFormat format) : textureFormat(format) {}

        TextureFormat textureFormat = TextureFormat::None;
        uint32_t attachmentID = 0;
    };

    struct FramebufferAttachments{
        FramebufferAttachments() = default;
        FramebufferAttachments(const std::initializer_list<FramebufferTextureMetaData>& list) : attachments(list) {}

        std::vector<FramebufferTextureMetaData> attachments;
    };

    struct FramebufferMetaData{
        uint32_t width, height;
        uint32_t samples = 1;

        bool swapchainTarget=false;
        FramebufferAttachments attachments;

    };

    class Framebuffer{
    public:
        virtual ~Framebuffer() = default;
        static Ref<Framebuffer> Create(const FramebufferMetaData&);

        void ApplyResources();
        void ReleaseResources();

        int ReadPixel(uint32_t attachmentIdx, int x, int y);

        void ClearAttachements(uint32_t attachmentIdx, int data);

        uint32_t GetAttachmentID(uint32_t idx = 0) const;

        // static void Resize(Ref<Framebuffer>&& framebuffer, int x, int y);

        static int MaxFramebufferSize();

    private:
        virtual void ApplyCurrentBoundState() = 0;
        virtual void ReleaseCurrentBoundState() = 0;
        virtual int GetPixelData(uint32_t attachmentIdx, int x, int y) = 0;
        virtual void ClearAttachmentData(uint32_t attachmentIdx, int data) = 0;
        virtual uint32_t GetPixelAttachmentID(uint32_t index = 0) const = 0;

        virtual const FramebufferMetaData& GetFramebufferData() const = 0;
    };
};