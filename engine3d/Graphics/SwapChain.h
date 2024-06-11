#pragma once
#include <engine3d/Core/core.h>

namespace engine3d{
    class SwapChains{
    public:

        static Ref<SwapChains> InitiateSwapChain();

        //! @note This will allow us to check if our current swap chain is provided images
        //! @note At least something to be rendered onto the screen
        void SwapChainPresent();
        
    private:
        virtual void CurrentSwapChainPresent() = 0;
    };
};