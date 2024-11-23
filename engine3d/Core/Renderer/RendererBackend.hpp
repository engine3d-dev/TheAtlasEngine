#pragma once

#include "Core.hpp"
namespace engine3d{
    /**
     * @name Renderer Backend
     * @note  that will represent our actual renderer to draw stuff
     * @note Graphics API-agnostic renderer that the main renderer will utilize.
    */
    class RendererBackend{
    public:
        static Ref<RendererBackend> InitializeRenderer(const std::string& p_DebugName);
        void BeginFrame();
        void EndFrame();

    private:
        virtual void StartFrame() = 0;
        virtual void FinishFrame() = 0;
    };
};