#pragma once
#include <engine3d/Core/core.h>
#include <engine3d/Graphics/SwapChain.h>

namespace Engine3D{
    class OpenGLSwapChain : public SwapChains{
    public:
        OpenGLSwapChain();
    private:
        void CurrentSwapChainPresent() override;
    };
};