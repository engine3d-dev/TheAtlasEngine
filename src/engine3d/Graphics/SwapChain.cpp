#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Graphics/SwapChain.h>


namespace engine3d{
    Ref<SwapChains> SwapChains::InitiateSwapChain(){
        switch (Pipeline::CurrentAPI()){
        case API::OPENGL:
        default:
            ConsoleLogError("API enabled is not supported, returning nullptr for InitiateSwapChain()");
            Render_Core_Assert(false);
            return nullptr;
        }

        ConsoleLogError("API enabled is not supported, returning nullptr for InitiateSwapChain()");
        Render_Core_Assert(false);
        return nullptr;
    }

    void SwapChains::SwapChainPresent(){
        CurrentSwapChainPresent();
    }
};