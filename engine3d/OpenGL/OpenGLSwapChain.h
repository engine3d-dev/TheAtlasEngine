#pragma once
#include <engine3d/Core/core.h>
#include <engine3d/Graphics/SwapChain.h>

namespace engine3d{
    class OpenGLSwapChain : public SwapChains{
    public:
        OpenGLSwapChain();
    private:
        void CurrentSwapChainPresent() override;
    };
};